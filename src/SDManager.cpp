#include <Arduino_BuiltIn.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>

#include "SDManager.h"

// SD chip select pin for Adafruit type Data Logging Shield
#define SD_CHIP_SELECT 10

// for the data logging shield, we use digital pin 10 for the SD cs line
Sd2Card card;
RTC_DS1307 rtc;

/// @brief Initialize SD card and RTC
/// Don't forget to format the SD card before using it
/// Windows> format /FS:FAT32 x:
void SDManager::Init()
{
    // make sure that the default chip select pin is set to
    // output, even if you don't use it:
    //pinMode(SD_CHIP_SELECT, OUTPUT);

    if (!card.init(SPI_HALF_SPEED, SD_CHIP_SELECT)) 
    {
        Serial.println("initialization failed. Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");
        Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
        while (1);
    } else {
        Serial.println("Wiring is correct and a card is present.");
    }

    // Initialize SD card
    if (!SD.begin(SD_CHIP_SELECT)) 
    {
        Serial.println("SD card initialization failed!");
        while (1); // Stop program if initialization fails
    }
    Serial.println("SD card initialized.");

    // Initialize RTC
    if (!rtc.begin()) {
        Serial.println("RTC initialization failed!");
        while (1);
    }
    if (!rtc.isrunning()) {
        Serial.println("RTC is not running. Setting time...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile time
    }
}

void SDManager::WriteData(String sensorValue)
{
    // Get current timestamp
    DateTime now = rtc.now();
    String timestamp = String(now.year()) + "/" + 
                        String(now.month()) + "/" + 
                        String(now.day()) + " " + 
                        String(now.hour()) + ":" + 
                        String(now.minute()) + ":" + 
                        String(now.second());

    // Write data to SD card
    File dataFile = SD.open("log.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.print(timestamp);
        dataFile.print(", ");
        dataFile.println(sensorValue);
        dataFile.close();
        Serial.println("Logged: " + timestamp + ", " + String(sensorValue));
    } else {
        Serial.println("Error opening log.txt");
    }
}