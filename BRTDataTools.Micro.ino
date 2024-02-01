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

#define R 26
#define G 25
#define B 27

WebServer webServer;
WiFiManager wifiManager;

#define V_OUT 3
#define POT_PIN (A0)

int potValue;
int angle;

void setup() 
{
  Serial.begin(9600);           // initialize serial communication
  
  // power
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(V_OUT, OUTPUT);      // set the LED pin mode
  
  digitalWrite(V_OUT, HIGH);   // set 5v output
  //analogWrite(V_OUT, 25);  

  WiFiDrv::pinMode(25, OUTPUT);
  WiFiDrv::pinMode(26, OUTPUT);
  WiFiDrv::pinMode(27, OUTPUT);
  
  WiFiDrv::analogWrite(26, 16);  //RED
  WiFiDrv::analogWrite(25, 0);   //GREEN
  WiFiDrv::analogWrite(27, 0);   //BLUE

  // attempt to connect to WiFi network:
  // while (status != WL_CONNECTED) {
  //   Serial.print("Attempting to connect to Network named: ");
  //   Serial.println(ssid);                   // print the network name (SSID);

  //   // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  //   status = WiFi.begin(ssid, pass);
  //   // wait 5 seconds for connection:
  //   delay(5000);
  // }
  //server.begin();                           // start the web server on port 80
  //printWifiStatus();                        // you're connected now, so print out the status

  digitalWrite(V_OUT, HIGH);          // set 5v output
  //analogWrite(V_OUT, 25);

  WiFiDrv::analogWrite(R, 0);    //RED
  WiFiDrv::analogWrite(G, 0);    //GREEN
  WiFiDrv::analogWrite(B, 16);   //BLUE

  wifiManager.InitWiFiAccessPoint();
  webServer.InitWebServer();
}

void loop() 
{ 
  potValue = analogRead(POT_PIN);
  // Analog -> Digital transformation
  // Map potentiometer min/max values to 0-150mm range
  int mmtravel = map(potValue, 0, 1023, 0, 150);

  OutputToSerial(mmtravel);

  wifiManager.CheckWiFiStatus();
  webServer.ListenClients();

  delay(50);
}

void OutputToSerial(int mmtravel)
{
    // Plotter range
    //Serial.println("Min:-2,Max:151");

    // plotter output
    Serial.print("Travel: ");
    Serial.print(mmtravel);
    Serial.println("mm");
}
