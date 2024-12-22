#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

#include "AnalogReader.h"

// Read pot data
void AnalogReader::ReadData()
{
    // Read pot data
    //_potData = analogRead(POT_PIN);
    //Serial.println(_potData);

}

int AnalogReader::GetLastData()
{
    return 69;
    // Get last pot data
    //Serial.println(_potData);
}

// Map pot data to range (0-150mm)
void AnalogReader::MapData()
{
    // Map pot data to range (0-150mm)
    //_mappedData = map(_potData, POT_MIN_V, POT_MAX_V, PLOT_BOUNDS, POT_TRAVEL_MM);
    //Serial.println(_mappedData);
}

// Sample rate
void AnalogReader::SampleRate()
{

}

    // Start reading when motion/speed exceeds threshold
void AnalogReader::StartReading()
{
    // Start reading when motion/speed exceeds threshold
    //if (_mappedData > 0)
    //{
        //ReadData();
        //MapData();
    //}
}

    // Stop reading when motion/speed is below threshold
void AnalogReader::StopReading()
{
    // Stop reading when motion/speed is below threshold
    //if (_mappedData == 0)
    //{
        //ReadData();
        //MapData();
    //}
}