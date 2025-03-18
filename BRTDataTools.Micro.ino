/*
BRT Racing Data Tools microcontroller code

TODO:
RTC timestamping
Volume freespace monitoring
GPS integration
Accelerometer integration
Autostart recording when moving

*/
#include <Arduino_BuiltIn.h>
#include <string.h>
#include <SPI.h>

#include "src/Utilities.h"
#include "src/AnalogReader.h"
#include "src/SDManager.h"
#include "src/WiFiManager.h"
#include "src/WebServer.h"

#define R 26
#define G 25
#define B 27

const int BOARD_DELAY = 20; // 20Hz
const int BAUD_RATE = 9600;
const int ADC_RESOLUTION = 1023; // 8 bit analog->digital ADC
const bool PLOT_BOUNDS = false;
const bool PLOT_SERIAL = false;

// AiM Eclipse 5V spec
// 500mV = 150mm, 4500mV = 0mm 
// Observed with 12V input
// 0.55 = 150mm, 4.35 = 0mm 
const float POT_VOLTAGE = 5.0;
const float POT_TRAVEL_MM = 150.0;
const float POT_MIN_V = 0.55;
const float POT_MAX_V = 4.35;
const uint8_t POT_PIN = A0;
const int V_OUT = 6;

// SD chip select pin for Adafruit Logging Shield
const int SD_CHIP_SELECT = 10;

AnalogReader* analogReader = new AnalogReader();
SDManager* sdManager = new SDManager();
WiFiManager* wifiManager = new WiFiManager();
WebServer* webServer = new WebServer(80);

/// @brief Arduino setup function
/// Initialize serial communication, SPI, sensor power, WiFi, SD card, web server and WiFi access point
void setup() 
{  
  // Initialize serial communication
  Serial.begin(BAUD_RATE);
  SPI.begin();

  // Sensor power
  pinMode(V_OUT, OUTPUT);  
  // Set 5v output
  digitalWrite(V_OUT, HIGH); 
  // Set LED outputs
  WiFiDrv::pinMode(R, OUTPUT);
  WiFiDrv::pinMode(G, OUTPUT);
  WiFiDrv::pinMode(B, OUTPUT);
  
  WiFiDrv::analogWrite(R, 16);
  WiFiDrv::analogWrite(G, 0);
  WiFiDrv::analogWrite(B, 0);
  
  sdManager->Init(SD_CHIP_SELECT);

  WiFiDrv::analogWrite(R, 0);  //RED
  WiFiDrv::analogWrite(G, 0);  //GREEN
  WiFiDrv::analogWrite(B, 16); //BLUE

  wifiManager->InitWiFiAccessPoint();   
  webServer->InitWebServer();
}

/// @brief Arduino loop function
/// Read sensor data, map sensor data to mm, write data to SD card, output to serial, listen for clients
void loop() 
{ 
  analogReader->ReadData();
  analogReader->GetLastData();

  // Analog -> Digital transformation
  int sensorvalue = analogRead(POT_PIN);
  // Convert sensor value to voltage
  float voltage = sensorvalue * (POT_VOLTAGE / ADC_RESOLUTION);
  // Map potentiometer min/max values to 0-150mm range
  int mmtravel = Mapfloat(voltage, POT_MAX_V, POT_MIN_V, (float)0, POT_TRAVEL_MM);
  
  OutputToSerial(mmtravel, voltage, sensorvalue);
      
  sdManager->WriteData(String(mmtravel));
  wifiManager->CheckWiFiStatus(mmtravel);
  webServer->ListenClients(sdManager);

  delay(BOARD_DELAY);

  WiFiDrv::analogWrite(R, 0);
  WiFiDrv::analogWrite(G, 16);
  WiFiDrv::analogWrite(B, 0);
}

// map function for float fractions
long Mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void OutputToSerial(int mmtravel, float raw_v, float sensorvalue)
{
    if (!PLOT_SERIAL)
      return;

    // Plotter range
    if (PLOT_BOUNDS)
    {
      Serial.print("Min:0,Max:");
      Serial.println(POT_MAX_V + 100);
    }

    // Serial output
    Serial.print("Travel:");
    Serial.print(mmtravel);

    Serial.print("  V:");
    Serial.print(raw_v);
    Serial.print("  Raw:");
    Serial.print(sensorvalue);

    // New line
    Serial.println();
}
