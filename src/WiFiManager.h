#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include "arduino_secrets.h"

/// @brief Creates local wifi access point
class WiFiManager 
{
private:    
    int keyIndex = 0;                   // network key index number (needed only for WEP)
    int wifistatus = WL_IDLE_STATUS;    // the Wifi radio's status
    char ssid_access_point[24] = "BRT_DataTools";  
    char ssid[24] = SECRET_SSID;      // network SSID (name)    
    char pass[24] = SECRET_PASS;      // network password (use for WPA, or use as key for WEP)

    //WiFiServer server(80);

public:
    WiFiManager()
    {

    }

    void InitWiFiAccessPoint();
    void ConnectWiFiNetwork();
    void CheckWiFiStatus(int output);    
    void PrintWiFiStatus();
};

#endif // WIFIMANAGER_H