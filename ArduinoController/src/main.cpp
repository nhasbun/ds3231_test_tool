#include <Arduino.h>
#include <DS3231.h>


// Parameters
#define SERIAL_BAUD 115200


// Global Variables
DS3231 clock;
bool century = false;  // Fine for ~79 years
bool programming_mode = false;
bool dummy_flags;


// Timestamp struct
typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} timestamp_t;


// Auxiliary methods
void set_programming_mode(bool v);
void pretty_print_date(timestamp_t date);
void format_print(uint8_t number);


// Arduino setup method
void setup() {
    Wire.begin();
	Serial.begin(SERIAL_BAUD);
	clock.setClockMode(false);  // Setting 24h format
}


// Arduino loop routine
void loop() {

    if (programming_mode) {

        // Waiting for incoming date format:
        // yy,m,d,h,m,s
        while (Serial.available() < 12) {

            delay(100);
        }

        timestamp_t date = {};

        date.year = Serial.parseInt();
        date.month = Serial.parseInt();
        date.day = Serial.parseInt();
        date.hour = Serial.parseInt();
        date.minute = Serial.parseInt();
        date.second = Serial.parseInt();

        Serial.println("== NEW DATE ==");
        pretty_print_date(date);

        clock.setYear(date.year);
        clock.setMonth(date.month);
        clock.setDate(date.day);
        clock.setHour(date.hour);
        clock.setMinute(date.minute);
        clock.setSecond(date.second);

        set_programming_mode(false);

    } else {

        timestamp_t date = {};

        date.year = clock.getYear();
        date.month = clock.getMonth(century);
        date.day = clock.getDate();
        date.hour = clock.getHour(dummy_flags, dummy_flags);
        date.minute = clock.getMinute();
        date.second = clock.getSecond();

        // Serial.println("New reading...");
        pretty_print_date(date);
    }

    if (Serial.available() >= 1) {

        char cmd = (char) Serial.read();

        if (cmd == 'p') {
            set_programming_mode(true);

        } else {
            set_programming_mode(false);
        }
    }

    delay(1000);
}


void pretty_print_date(timestamp_t date) {

    format_print(date.hour); Serial.print(":");
    format_print(date.minute); Serial.print(":");
    format_print(date.second); Serial.print("   ");

    format_print(date.day); Serial.print("-");
    format_print(date.month); Serial.print("-");
    format_print(date.year); Serial.println("");
}


void set_programming_mode(bool v) {
    if (v == true) {
        programming_mode = true;
        Serial.println("Programming mode enabled...");

    } else {
        programming_mode = false;
        Serial.println("Programming mode disabled...");
    }
}

void format_print(uint8_t number) {
    if (number < 10) Serial.print(0);
    Serial.print(number);
}
