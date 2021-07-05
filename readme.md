# DS3231 Test Tool

Arduino controller and Python 3 script to test and synchronize a DS3231 RTC with current machine time.

## Requirements

- Python 3.6+
- PlatformIO Core (CLI) installed
- Arduino Nano with USB connected
- RTC DS3231
- Wiring:
    - Arduino A4 => SDA
    - Arduino A5 => SCL
    - 5V voltage and ground connected

## Quick usage

1. Upload controller firmware to arduino,
   ```
   cd ArduinoController && pio run -e nanoatmega328 -t upload
   ```

1. Create a Python virtual environment,
   ```
   python -m venv venv
   venv\Scripts\activate.bat
   pip install -r requirements.txt
   ```

1. Edit script with proper serial port and run, press **P** key to trigger sync time from machine to rtc,
   ```python
   python time_sync.py
   ```