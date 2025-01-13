#ifndef SDMANAGER_H
#define SDMANAGER_H

//#include <Arduino.h>

class SDManager {
private:    
    String results[500] = { };   
    int counter = 0;

public:
    // Constructor
    void Init(int SD_CHIP_SELECT);

    // Save data to SD card
    void WriteData(String sensorValue);

    // Read data from SD card
    char * ReadData();
};

#endif // SDMANAGER_H