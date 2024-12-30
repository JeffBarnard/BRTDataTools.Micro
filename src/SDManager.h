#ifndef SDMANAGER_H
#define SDMANAGER_H

//#include <Arduino.h>

class SDManager {
public:
    // Constructor
    void Init(int SD_CHIP_SELECT);

    // Save data to SD card
    void WriteData(String sensorValue);
};

#endif // SDMANAGER_H