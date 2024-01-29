#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>

class WiFiManager {
public:
    void InitWiFiAccessPoint();
    void ConnectWiFiNetwork();
    void CheckWiFiStatus();    
};

#endif // WIFIMANAGER_H