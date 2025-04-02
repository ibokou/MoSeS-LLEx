# Based on ReachView code from Egor Fedorov (egor.fedorov@emlid.com)
# Updated for Python 3.6.8 on a Raspberry  Pi


import time
import pexpect
import subprocess
import sys
import logging


logger = logging.getLogger("btctl")


class Bluetoothctl:
    """A wrapper for bluetoothctl utility."""

    def __init__(self):
        subprocess.check_output("rfkill unblock bluetooth", shell=True)
        self.process = pexpect.spawnu("bluetoothctl", echo=True)

    def send(self, command, pause=0):
        self.process.send(f"{command}\n")
        time.sleep(pause)
        #if self.process.expect(["bluetooth", pexpect.EOF]):
        #    raise Exception(f"failed after {command}")

    def start_ble_scan(self):
        """Start bluetooth scanning process."""
        try:
            self.send("menu scan")
            self.send("transport le")
            self.send("back")
            self.send("scan on")
        except Exception as e:
            logger.error(e)