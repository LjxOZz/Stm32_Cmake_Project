from typing import List
import logging

import nmxrt
import nmxrdk

import serial
import modbus_tk
import modbus_tk.modbus_rtu as modbus_rtu

class NmxVacuumDevice(nmxrdk.VacuumDevice):
    def __init__(self, config, usb_dev):
        self.config = config
        self.handler = None
        self.feedback = [0]
        self.req_set_brightness = None
        self.usb_dev = usb_dev

    def init(self):
        try:
            self.handler = modbus_rtu.RtuMaster(
                self.usb_dev
            )
            self.handler.set_timeout(1.0)
        except Exception as e:
            logging.error(f"Failed to initialize Modbus RTU: {e}")
            raise e

        self.req_set_brightness = nmxrt.Querable(
            f"nmx/hal/{self.config.robot.id}/light/set_brightness",
            nmxrdk.LightCmd,
            nmxrdk.Status,
            self.set_brightness
        )

    def set_brightness(self, cmd: nmxrdk.LightCmd) -> nmxrdk.Status:
        try:
            brightness = int(
                cmd.brightness
                * (self.config.light.max_brightness - self.config.light.min_brightness)
                / 100.0
                + self.config.light.min_brightness
            )
            self.handler.execute(
                1, modbus_tk.defines.WRITE_SINGLE_REGISTER, starting_address=0x01F5, output_value=brightness
            )
            return nmxrdk.Status(code=0)
        except Exception as e:
            logging.warning(f"Error setting brightness: {e}")

        return nmxrdk.Status(code=1)
  
    def open(self, index_list: List[int] = [0, 0, 0, 0], force: float = 0.0):
        try:
            self.handler.execute(
                1,
                modbus_tk.defines.WRITE_MULTIPLE_COILS,
                starting_address=0,
                quantity_of_x=4,
                output_value=index_list,
            )
            force = int(self.config.vacuum.force_span * force / 100.0)

            self.handler.execute(
                1, modbus_tk.defines.WRITE_SINGLE_REGISTER, starting_address=0x01F4, output_value=force
            )
        except Exception as e:
            logging.warning(f"Error opening vacuum: {e}")


    def close(self, index_list: List[int] = [1, 1, 1, 1], force: float = 30.0):
        try:
            self.handler.execute(
                1,
                modbus_tk.defines.WRITE_MULTIPLE_COILS,
                starting_address=0,
                quantity_of_x=4,
                output_value=index_list,
            )
            force = int(self.config.vacuum.force_span * force / 100.0)

            self.handler.execute(
                1, modbus_tk.defines.WRITE_SINGLE_REGISTER, starting_address=0x01F4, output_value=force
            )
        except Exception as e:
            logging.warning(f"Error closing vacuum: {e}")

    def reach_target(self) -> bool:
        return self.feedback[0] > self.config.vacuum.force_reach

    def get_force(self) -> float:
        try:
            self.feedback = self.handler.execute(
                1, modbus_tk.defines.READ_INPUT_REGISTERS, starting_address=0, quantity_of_x=1
            )
            return self.feedback[0]
        except Exception as e:
            logging.warning(f"Error getting force: {e}")
        
        return 0.0