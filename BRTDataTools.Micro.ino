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
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include <SPI.h>

// #include "src/WebServer.h"
#include "src/Utilities.h"
#include "src/AnalogReader.h"
#include "src/SDManager.h"
#include "src/WiFiManager.h"

#define R 26
#define G 25
#define B 27

const int BOARD_DELAY = 20; // 20Hz
const int BAUD_RATE = 9600;
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
const int POT_RESOLUTION = 1023;
const uint8_t POT_PIN = A0;
const int V_OUT = 6;

// SD chip select pin for Adafruit Logging Shield
const int SD_CHIP_SELECT = 10;

WiFiServer server(80);

AnalogReader* analogReader = new AnalogReader();
SDManager* sdManager = new SDManager();
WiFiManager* wifiManager = new WiFiManager();

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
      
  WiFiDrv::pinMode(R, OUTPUT);
  WiFiDrv::pinMode(G, OUTPUT);
  WiFiDrv::pinMode(B, OUTPUT);
  
  WiFiDrv::analogWrite(R, 16);
  WiFiDrv::analogWrite(G, 0);
  WiFiDrv::analogWrite(B, 0);
  
  sdManager->Init(SD_CHIP_SELECT);      
  wifiManager->InitWiFiAccessPoint(); 
  
  //webServer.InitWebServer();
  // start the web server on port 80
  server.begin();
  // server is connected now, so print out the status
  //PrintWiFiStatus();
}

/// @brief Arduino loop function
/// Read sensor data, map sensor data to mm, write data to SD card, output to serial, listen for clients
void loop() 
{ 
  analogReader->ReadData();
  analogReader->GetLastData();

  int sensorvalue = analogRead(POT_PIN);
  float voltage = sensorvalue * (POT_VOLTAGE / POT_RESOLUTION);
  // Analog -> Digital transformation
  // Map potentiometer min/max values to 0-150mm range
  int mmtravel = Mapfloat(voltage, POT_MAX_V, POT_MIN_V, (float)0, POT_TRAVEL_MM);
  
  sdManager->WriteData(String(mmtravel));  
    
  if (PLOT_SERIAL)
    OutputToSerial(mmtravel, voltage, sensorvalue);

  //webServer.SetOutput(mmtravel);
  wifiManager->CheckWiFiStatus(mmtravel);
  //webServer.ListenClients();

  WiFiClient client = server.available();   // listen for incoming clients

  // if we get a client
  if (client) 
  {                             
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected()) 
    {   
      if (client.available()) 
      {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out to the serial monitor
         // if the byte is a newline character
        if (c == '\n') 
        {
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) 
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/plain");
            client.println();            
            // the content of the HTTP response follows the header

            String ch;
            File dataFile = sdManager->SD.open(sdManager->GetCurrentDataLogFile(), FILE_READ);
            
            if (dataFile) 
            {
                while (dataFile.available())
                {
                  ch = dataFile.readStringUntil(sdManager->SEPERATOR);
                  client.print(ch);
                  client.print(sdManager->SEPERATOR);
                }
                dataFile.close();
                // null terminate string
                //dataline += '\0';
            }
            // if the file isn't open, pop up an error:
            else 
            {
                Serial.println("error reading dataLoggerFile from webserver ");
                while (1);
            }

            // The HTTP response ends with another blank line
            client.println();
            break;
          }
          else 
          {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') 
        {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        //if (currentLine.endsWith("GET /H")) {       
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

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
