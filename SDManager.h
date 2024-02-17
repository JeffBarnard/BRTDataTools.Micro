#ifndef SDMANAGER_H
#define SDMANAGER_H

#include <Arduino.h>

class SDManager {
public:
    // Save data to SD card
    void SaveData();

    // Read data from SD card
    void ReadData();    
};

#endif // SDMANAGER_H