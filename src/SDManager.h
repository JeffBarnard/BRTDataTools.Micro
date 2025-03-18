#ifndef SDMANAGER_H
#define SDMANAGER_H

#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>

#include "Utilities.h"

/// @brief SDCard Manager class for Adafruit Data Logger Shield
/// @details This class is responsible for managing the SD card and writing data to it.
class SDManager
{
  private:
    //RTC_DS1307 rtc;
    char dataLogFile[16] = "data000.log";
    RTC_DS1307 rtc;

    void InitializeRTC()
    {
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

  public:
    SDClass SD;
    const char SEPERATOR = ',';

    SDManager()
    {
      SD = SDLib::SDClass();
    }

    /// @brief Initialize SD card
    /// @param SD_CHIP_SELECT 
    void Init(int SD_CHIP_SELECT);
    
    /// @brief Find the next available data log file number
    /// @return 
    int FindNextAvailableNumber() ;

    /// @brief Get the current data log file
    /// @return 
    char * GetCurrentDataLogFile()
    {
        return dataLogFile;
    }
    
    /// @brief Write a single datapoint to the current data log file
    /// @param sensorValue 
    void WriteData(String sensorValue);

    /// @brief Write volume and card info to serial
    void WriteCardInfo();
};

#endif // SDMANAGER_H