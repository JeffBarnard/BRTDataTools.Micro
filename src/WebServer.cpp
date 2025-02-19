#include "WebServer.h"

#include <WiFiNINA.h>

#define R 26
#define G 25
#define B 27

void WebServer::InitWebServer() 
{
    WiFiDrv::pinMode(R, OUTPUT);
    WiFiDrv::pinMode(G, OUTPUT);
    WiFiDrv::pinMode(B, OUTPUT);

    // // Check for the WiFi module:
    // if (WiFi.status() == WL_NO_MODULE) {
    //   Serial.println("Communication with WiFi module failed!");
    //   // don't continue
    //   while (true);
    // }

    // String fv = WiFi.firmwareVersion();
    // if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    //     Serial.println("Please upgrade the firmware");
    // }

    WiFiDrv::analogWrite(R, 16);  //RED
    WiFiDrv::analogWrite(G, 0);   //GREEN
    WiFiDrv::analogWrite(B, 0);   //BLUE

    delay(500);
    //ConnectWifiNetwork();

   
        
    WiFiDrv::analogWrite(R, 0);    //RED
    WiFiDrv::analogWrite(G, 0);    //GREEN
    WiFiDrv::analogWrite(B, 16);   //BLUE
}

void WebServer::SetOutput(int output)
{
    _output = output;
}

void WebServer::ListenClients() 
{
    
}
