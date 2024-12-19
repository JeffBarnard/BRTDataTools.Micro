#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>

class WebServer {
private:    
    int _output;
public:
    void InitWebServer();
    void SetOutput(int output);    
    void ListenClients();
};

#endif // WEBSERVER_H