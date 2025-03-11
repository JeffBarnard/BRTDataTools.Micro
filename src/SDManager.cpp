#include "Utilities.h"
#include "SDManager.h"

/// @brief Initialize SD card and RTC
/// Don't forget to format the SD card before using it
void SDManager::Init(int sd_chip_select)
{
    // make sure that the default chip select pin is set to output, even if you don't use it:
    pinMode(sd_chip_select, OUTPUT);  

    if (!this->SD.begin(sd_chip_select))
    //if (!card.init(SPI_HALF_SPEED, sdchipselect)) 
    {
        Serial.println("SD Initialization failed.");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");        
        while (1);
    }

    Serial.println("SD card initialized");
    
    // Find the new datalog file name
    int logiterator = FindNextAvailableNumber();
    char cstr[4];
    // Int to array
    itoa(logiterator, cstr, 10);
    str_replace(dataLogFile, "000", cstr);

    Serial.print("Logging to ");
    Serial.println(dataLogFile);

    // if (sd.exists(dataLogFile))
    // {
    //     Serial.print("Removing ");
    //     Serial.println(dataLogFile);
    //     sd.remove(dataLogFile);
    // }
    // else
    // {
    //     Serial.print(dataLogFile);
    //     Serial.println(" does not exist");
    // }  

    //InitializeRTC();
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

    File dataFile = this->SD.open(dataLogFile, FILE_WRITE);
    
    // if the file is available write to it:
    if (dataFile) 
    {
        dataFile.print(sensorValue);
        dataFile.print(SEPERATOR);
        dataFile.close();
    }
    else 
    {
        Serial.println("error opening dataLog file in loop");
        while (1);
    }
}

int SDManager::FindNextAvailableNumber() 
{
    int maxNum = 0;
    File root = this->SD.open("/"); // Open root directory
        
    while (true) 
    {
        File entry = root.openNextFile();
        if (!entry) {
            break; // No more files
        }

        String fileName = entry.name();      
        Serial.print("File exits ");
        Serial.println(fileName);

        // Extract the number from the filename
        if (fileName.startsWith("DATA"))
        {
            // 7 is the length of "data" and 4 is the length of ".log"
            String numStr = fileName.substring(4, fileName.length() - 4); 
            int num = numStr.toInt();
            if (num > maxNum) {
                maxNum = num;
            }
        }

        entry.close();
    }
    root.close();

    return maxNum + 1;
}

void SDManager::WriteCardInfo()
{
    Sd2Card card;
    SdVolume volume;
    SdFile root;

    // print the type of card
    Serial.println();
    Serial.print("Card type:         ");
    switch (card.type()) {
        case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        break;
        case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        break;
        case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        break;
        default:
        Serial.println("Unknown");
    }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1);
  }

  Serial.print("Clusters:          ");
  Serial.println(volume.clusterCount());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(volume.blocksPerCluster());

  Serial.print("Total Blocks:      ");
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is:    FAT");
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1 KB)
  Serial.print("Volume size (KB):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (MB):  ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (GB):  ");
  Serial.println((float)volumesize / 1024.0);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  root.close();
}

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
