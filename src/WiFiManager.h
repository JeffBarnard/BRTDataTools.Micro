#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>

// TODO
// Transfer data to PC via WiFi

class WiFiManager {
public:
    void InitWiFiAccessPoint();
    void ConnectWiFiNetwork();
    void CheckWiFiStatus();    
};

#endif // WIFIMANAGER_H