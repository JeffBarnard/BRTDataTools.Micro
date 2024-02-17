/*
BRT Racing Data Tools microcontroller code

TODO:


*/
#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

#include "WebServer.h"
#include "WiFiManager.h"
#include "AnalogReader.h"
#include "SDManager.h"

#define POT_TRAVEL 150
#define POT_MIN_V 100
#define POT_MAX_V 850

#define R 26
#define G 25
#define B 27

WebServer webServer;
WiFiManager wifiManager;

#define V_OUT 13
#define POT_PIN (A1)
#define BOARD_DELAY (50)
#define PLOT 0

void setup() 
{
  Serial.begin(115200);           // initialize serial communication
  
  // power
  pinMode(V_OUT, OUTPUT);
  digitalWrite(V_OUT, HIGH); // set 5v output

  WiFiDrv::pinMode(R, OUTPUT);
  WiFiDrv::pinMode(G, OUTPUT);
  WiFiDrv::pinMode(B, OUTPUT);
  
  WiFiDrv::analogWrite(R, 16);  //RED
  WiFiDrv::analogWrite(G, 0);   //GREEN
  WiFiDrv::analogWrite(B, 0);   //BLUE

  WiFiDrv::analogWrite(R, 0);    //RED
  WiFiDrv::analogWrite(G, 0);    //GREEN
  WiFiDrv::analogWrite(B, 16);   //BLUE

  wifiManager.InitWiFiAccessPoint();
  webServer.InitWebServer();
}

void loop() 
{ 
  int pot_raw = analogRead(POT_PIN);
  // Analog -> Digital transformation
  // Map potentiometer min/max values to 0-150mm range
  int mmtravel = map(pot_raw, POT_MAX_V, POT_MIN_V, 0, POT_TRAVEL);
  
  OutputToSerial(mmtravel, pot_raw);

  //wifiManager.CheckWiFiStatus();
  //webServer.ListenClients();

  delay(BOARD_DELAY);
}

void OutputToSerial(int mmtravel, int raw_v)
{
    // Plotter range
    if (PLOT)
      Serial.println("Min:-2,Max:151");

    // plotter output
    Serial.print("Travel:");
    Serial.print(mmtravel);

    Serial.print("  v:");
    Serial.println(raw_v);
    //Serial.println("mm");
}
