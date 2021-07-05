"""
Simple script to test and program RTC DS3231 via an Arduino controller.

Script prints constantly reported RTC time. P key syncs machine time with
RTC and Q or ESC key exit the script.

Nicolás Hasbún A.
nhasbun@gmail.com
"""

from datetime import datetime
from threading import Event
from time import sleep

from clear_screen import clear
from keyboard import add_hotkey
from serial import Serial


# General parameters
com_port = "COM6"
serial_baud = 115200
serial_timeout = 5


# Global variables
stop_signal = Event()


# Auxiliary methods
def _print_serial_rx(_serial):
    """
    Pretty print for incoming serial data from arduino ds3231 controller.
    """
    while not stop_signal.wait(0.1):

        bytes_waiting = _serial.in_waiting

        if bytes_waiting > 0:
            print(_serial.read(bytes_waiting).decode()
                  .replace("\n", "\r")
                  .replace("...", "...\n"), end="")


def _sync_time(_serial):
    """
    Activates programming mode on arduino and sync time between machine and
    ds3231 rtc.
    """

    current_date = datetime.now()
    year = current_date.year % 100  # extract 2 last digits
    month = current_date.month
    day = current_date.day
    hour = current_date.hour
    minute = current_date.minute
    second = current_date.second

    _serial.write(b'p')  # Enabling programming mode

    # Formatting data
    data = f"{year},{month},{day},{hour},{minute},{second}"
    data_b = data.encode('ascii')
    _serial.write(data_b)


def _quit():
    """
    Exit script...
    """
    stop_signal.set()


# Main routine
if __name__ == '__main__':

    serial = Serial(port=com_port,
                    baudrate=serial_baud,
                    timeout=serial_timeout)
    sleep(2)  # Arduino resets itself when uart is enabled, time to boot needed

    add_hotkey('p', _sync_time, args=(serial,))
    add_hotkey('q', _quit, args=())
    add_hotkey('esc', _quit, args=())

    clear()
    print("== DS3231 Test Tool ==\n")
    print("Press p to sync RTC with machine time...")
    print("Press q or esc to exit...\n")

    _print_serial_rx(serial)
