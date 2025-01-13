/*
BRT Racing Data Tools microcontroller code

TODO:


*/
#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

#include "src/WebServer.h"
#include "src/WiFiManager.h"
#include "src/AnalogReader.h"
#include "src/SDManager.h"

// AiM Eclipse 5V spec
// 500mV = 150mm, 4500mV = 0mm 

// Observed with 9V battery input
// 0.50 = 150mm, 4.05 = 0mm  

// Observed with 12V input
// 0.52 = 150mm, 4.06 = 0mm  

#define POT_TRAVEL_MM 150.0
#define POT_MIN_V 0.52
#define POT_MAX_V 4.06

#define POT_PIN (A0)
#define V_OUT 6
#define SD_CHIP_SELECT 10 // SD chip select pin for Adafruit Logging Shield

#define BOARD_DELAY (20) // 20Hz Default
#define PLOT_BOUNDS 0

#define R 26
#define G 25
#define B 27

AnalogReader analogReader;
SDManager sdManager;
WebServer webServer;
WiFiManager wifiManager;

void setup() 
{  
  Serial.begin(9600);           // initialize serial communication
  
  // Sensor power
  pinMode(V_OUT, OUTPUT);  
  digitalWrite(V_OUT, HIGH); // set 5v output
      
  WiFiDrv::pinMode(R, OUTPUT);
  WiFiDrv::pinMode(G, OUTPUT);
  WiFiDrv::pinMode(B, OUTPUT);
  
  WiFiDrv::analogWrite(R, 16);  //RED
  WiFiDrv::analogWrite(G, 0);   //GREEN
  WiFiDrv::analogWrite(B, 0);   //BLUE

  sdManager.Init(SD_CHIP_SELECT);
  wifiManager.InitWiFiAccessPoint();
  webServer.InitWebServer();
}

void loop() 
{ 
  analogReader.ReadData();

  analogReader.GetLastData();

  int sensorvalue = analogRead(POT_PIN);
  float voltage = sensorvalue * (5.0 / 1023.0);
  // Analog -> Digital transformation
  // Map potentiometer min/max values to 0-150mm range
  int mmtravel = mapf(voltage, POT_MAX_V, POT_MIN_V, (float)0, POT_TRAVEL_MM);
  
  sdManager.WriteData(String(mmtravel));
  
  //OutputToSerial(mmtravel, voltage, sensorvalue);

  //char * buf = sdManager.ReadData();
  webServer.SetOutput(mmtravel);

  //Serial.println(buf);

  wifiManager.CheckWiFiStatus();
  webServer.ListenClients();

  delay(BOARD_DELAY);
}

// map function for float fractions
long mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void OutputToSerial(int mmtravel, float raw_v, float sensorvalue)
{
    // Plotter range
    if (PLOT_BOUNDS)
    {
      Serial.print("Min:0,Max:");
      Serial.println(POT_MAX_V + 100);
    }

    // serial output
    Serial.print("Travel:");
    Serial.print(mmtravel);

    Serial.print("  V:");
    Serial.print(raw_v);
    Serial.print("  Raw:");
    Serial.print(sensorvalue);

    // New line
    Serial.println();    
}

void DataPoint()
{
  // 
  // 
  // Time
}
