#ifndef ANALOGREADER_H
#define ANALOGREADER_H

#include <Arduino.h>

class AnalogReader {
public:
    // Read pot data
    void ReadData();

    // Map pot data to range (0-150mm)
    void MapData();

    // Sample rate
    void SampleRate();

    // Start reading when motion/speed exceeds threshold
    void StartReading();

    // Stop reading when motion/speed is below threshold
    void StopReading();    
};

#endif // ANALOGREADER_H