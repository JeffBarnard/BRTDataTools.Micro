#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>

class WebServer {
public:
    void InitWebServer();
    void ListenClients();
};
#endif // WEBSERVER_H