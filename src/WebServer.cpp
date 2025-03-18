#include "WebServer.h"

#include <WiFiNINA.h>

#define R 26
#define G 25
#define B 27

void WebServer::InitWebServer() 
{
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

    // let the network to initialize
    delay(5000);
    // start the web server on port 80
    begin();
}

void WebServer::SetOutput(int output)
{
    _output = output;
}

void WebServer::ListenClients(SDManager* sdManager) 
{
    // listen for incoming clients
    WiFiClient client = available();
    // if we get a client
    if (client) 
    {                             
        Serial.println("new client");           // print a message out the serial port
        String currentLine = "";                // make a String to hold incoming data from the client

        while (client.connected()) 
        {   
            if (client.available()) 
            {             // if there's bytes to read from the client,
                char c = client.read();             // read a byte, then
                Serial.write(c);                    // print it out to the serial monitor
                    // if the byte is a newline character
                if (c == '\n') 
                {
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0) 
                    {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/plain");
                        client.println();            
                        // the content of the HTTP response follows the header

                        String ch;
                        File dataFile = sdManager->SD.open(sdManager->GetCurrentDataLogFile(), FILE_READ);
                        
                        if (dataFile) 
                        {
                            while (dataFile.available())
                            {
                                ch = dataFile.readStringUntil(sdManager->SEPERATOR);
                                client.print(ch);
                                client.print(sdManager->SEPERATOR);
                            }
                            dataFile.close();
                            // null terminate string
                            //dataline += '\0';
                        }
                        // if the file isn't open, pop up an error:
                        else 
                        {
                            Serial.println("error reading dataLoggerFile from webserver ");
                            while (1);
                        }

                        // The HTTP response ends with another blank line
                        client.println();
                        break;
                    }
                    else 
                    {      
                        // if we get a newline, then clear currentLine:
                        currentLine = "";
                    }
                }
                else if (c != '\r') 
                {    
                    // if we get anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                }

                // Check to see if the client request was "GET /H" or "GET /L":
                //if (currentLine.endsWith("GET /H")) {       
            }
        }
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }
}
