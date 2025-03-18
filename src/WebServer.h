#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <SD.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

#include "SDManager.h"

class WebServer : public WiFiServer
{
private:    
    int _output;
    //WiFiServer server;

public:
    WebServer(int port) : WiFiServer(port) 
    { 

    }

    void InitWebServer();
    void SetOutput(int output);    
    void ListenClients(SDManager* sdManager);
};

#endif // WEBSERVER_H