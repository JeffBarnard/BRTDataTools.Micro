#include "WebServer.h"

#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

#define R 26
#define G 25
#define B 27

WiFiServer server(80);

void WebServer::InitWebServer() {

    WiFiDrv::pinMode(25, OUTPUT);
    WiFiDrv::pinMode(26, OUTPUT);
    WiFiDrv::pinMode(27, OUTPUT);

    // // check for the WiFi module:
    // if (WiFi.status() == WL_NO_MODULE) {
    //   Serial.println("Communication with WiFi module failed!");
    //   // don't continue
    //   while (true);
    // }

    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
        Serial.println("Please upgrade the firmware");
    }

    WiFiDrv::analogWrite(R, 16);  //RED
    WiFiDrv::analogWrite(G, 0);   //GREEN
    WiFiDrv::analogWrite(B, 0);   //BLUE

    delay(500);
    //ConnectWifiNetwork();
    server.begin();                           // start the web server on port 80

    //void printWiFiStatus() 
    //{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");

    
    Serial.print("Open a browser to http://");
    Serial.println(ip);
    //}
        
    WiFiDrv::analogWrite(R, 0);    //RED
    WiFiDrv::analogWrite(G, 0);    //GREEN
    WiFiDrv::analogWrite(B, 16);   //BLUE
}

void WebServer::ListenClients() {
    WiFiClient client = server.available();   // listen for incoming clients

    if (client) {                             // if you get a client,
      Serial.println("new client");           // print a message out the serial port
      String currentLine = "";                // make a String to hold incoming data from the client

      while (client.connected()) {            // loop while the client's connected

        WiFiDrv::analogWrite(R, 0);   //RED
        WiFiDrv::analogWrite(G, 16);  //GREEN      
        WiFiDrv::analogWrite(B, 0);   //BLUE

        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              // the content of the HTTP response follows the header:
              client.print("Click <a href=\"/H\">here</a> turn the LED on pin 9 on<br>");
              client.print("Click <a href=\"/L\">here</a> turn the LED on pin 9 off<br>");

              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            } else {    // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }


          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("GET /H")) {
            digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
            //digitalWrite(V_OUT, HIGH);      // set the LED pin mode
          }
          if (currentLine.endsWith("GET /L")) {
            digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
            //digitalWrite(V_OUT, LOW);      // set the LED pin mode
          }
        }
      }

      // close the connection:
      client.stop();
      Serial.println("client disconnected");

      WiFiDrv::analogWrite(R, 0);    //RED
      WiFiDrv::analogWrite(G, 0);    //GREEN
      WiFiDrv::analogWrite(B, 16);   //BLUE
    } // if client

    delay(50);
}
