#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include "arduino_secrets.h"

class WiFiManager 
{
private:    
    int keyIndex = 0;               // your network key index number (needed only for WEP)
    int wifistatus = WL_IDLE_STATUS;    // the Wifi radio's status
    char ssid_access_point[24] = "BRT_DataTools";  
    char ssid[24] = SECRET_SSID;      // your network SSID (name)    
    char pass[24] = SECRET_PASS;      // network password (use for WPA, or use as key for WEP)

public:
    void InitWiFiAccessPoint();
    void ConnectWiFiNetwork();
    void CheckWiFiStatus(int output);    
    void PrintWiFiStatus();
};

#endif // WIFIMANAGER_H