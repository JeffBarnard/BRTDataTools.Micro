#include <Arduino_BuiltIn.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>

#include "SDManager.h"

// for the data logging shield, we use digital pin 10 for the SD cs line
//Sd2Card card;
//SdVolume volume;
//SdFile root;

RTC_DS1307 rtc;

/// @brief Initialize SD card and RTC
/// Don't forget to format the SD card before using it
/// Windows> format /FS:FAT32 x:
void SDManager::Init(int SD_CHIP_SELECT)
{
    SPI.begin();
    // make sure that the default chip select pin is set to
    // output, even if you don't use it:
    pinMode(SD_CHIP_SELECT, OUTPUT);
    
    if (!SD.begin(SD_CHIP_SELECT))
    //if (!card.init(SPI_HALF_SPEED, SD_CHIP_SELECT)) 
    {
        Serial.println("SD Initialization failed. Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");        
        while (1);
    }

    //Serial.println("SD card initialized.");
    //WriteCardInfo();
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.   
    // if (SD.exists("DATALOG.TXT"))
    // {
    //     Serial.println("Removing DATALOG.TXT");
    //     //SD.remove("DATALOG.TXT");
    // }
    // else
    // {
    //     Serial.println("DATALOG.TXT does not exist.");
    // }

    File dataFile = SD.open("DATALOG.TXT", FILE_WRITE);
     
    // if the file is available, write to it:
    if (dataFile) 
    {
        dataFile.println("hi");
        dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else
    {
        Serial.println("error opening datalog.txt init");
        while (1);
    }

    // Initialize RTC
    // if (!rtc.begin()) {
    //     Serial.println("RTC initialization failed!");
    //     while (1);
    // }
    // if (!rtc.isrunning()) {
    //     Serial.println("RTC is not running. Setting time...");
    //     rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile time
    // }
}

void SDManager::WriteData(String sensorValue)
{
    // Get current timestamp
    // DateTime now = rtc.now();
    // String timestamp = String(now.year()) + "/" + 
    //                     String(now.month()) + "/" + 
    //                     String(now.day()) + " " + 
    //                     String(now.hour()) + ":" + 
    //                     String(now.minute()) + ":" + 
    //                     String(now.second());

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another
    File dataFile = SD.open("DATALOG.TXT", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile)
    {
        dataFile.println(sensorValue);
        dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else 
    {
        Serial.println("error opening datalog.txt");
        while (1);
    }
}

// void SDManager::WriteCardInfo()
// {
//     // print the type of card
//   Serial.println();
//   Serial.print("Card type:         ");
//   switch (card.type()) {
//     case SD_CARD_TYPE_SD1:
//       Serial.println("SD1");
//       break;
//     case SD_CARD_TYPE_SD2:
//       Serial.println("SD2");
//       break;
//     case SD_CARD_TYPE_SDHC:
//       Serial.println("SDHC");
//       break;
//     default:
//       Serial.println("Unknown");
//   }

//   // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
//   if (!volume.init(card)) {
//     Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
//     while (1);
//   }

//   Serial.print("Clusters:          ");
//   Serial.println(volume.clusterCount());
//   Serial.print("Blocks x Cluster:  ");
//   Serial.println(volume.blocksPerCluster());

//   Serial.print("Total Blocks:      ");
//   Serial.println(volume.blocksPerCluster() * volume.clusterCount());
//   Serial.println();

//   // print the type and size of the first FAT-type volume
//   uint32_t volumesize;
//   Serial.print("Volume type is:    FAT");
//   Serial.println(volume.fatType(), DEC);

//   volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
//   volumesize *= volume.clusterCount();       // we'll have a lot of clusters
//   volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1 KB)
//   Serial.print("Volume size (KB):  ");
//   Serial.println(volumesize);
//   Serial.print("Volume size (MB):  ");
//   volumesize /= 1024;
//   Serial.println(volumesize);
//   Serial.print("Volume size (GB):  ");
//   Serial.println((float)volumesize / 1024.0);

//   Serial.println("\nFiles found on the card (name, date and size in bytes): ");
//   root.openRoot(volume);

//   // list all files in the card with date and size
//   root.ls(LS_R | LS_DATE | LS_SIZE);
//   root.close();
// }

// char * SDManager::ReadData()
// {
//     // Read data from SD card
//     // File dataFile = SD.open("log.txt");
//     // if (dataFile) {
//     //     while (dataFile.available()) {
//     //         Serial.write(dataFile.read());
//     //     }
//     //     dataFile.close();
//     // } else {
//     //     Serial.println("Error opening log.txt");
//     // }

//     char * buf;
//     results->toCharArray(buf, 500);
//     return buf;
// }