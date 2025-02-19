#include "WiFiManager.h"

#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

#define R 26
#define G 25
#define B 27

WiFiServer webserver(80);

void WiFiManager::InitWiFiAccessPoint()
{
    Serial.println("Access Point Web Server");

    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) 
    {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true);
    }

    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) 
    {
        Serial.println("Please upgrade the firmware");
    }

    // print the network name (SSID);
    Serial.print("Creating access point named: ");
    Serial.println(ssid_access_point);

    // Create open network. Change this line if you want to create an WEP network:
    wifistatus = WiFi.beginAP(ssid_access_point);
    if (wifistatus != WL_AP_LISTENING) 
    {
        Serial.println("Creating access point failed");
        // don't continue
        while (true);
    }

    // Print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // Print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("Signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");

    // wait for connection:
    delay(10000);
    
    // Start the web server on port 80
    webserver.begin();
    PrintWiFiStatus();
}

void WiFiManager::ConnectWiFiNetwork()
{
    // attempt to connect to WiFi network:
    while (wifistatus != WL_CONNECTED) 
    {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid);                   // print the network name (SSID);

        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        wifistatus = WiFi.begin(ssid, pass);
        // wait 5 seconds for connection:
        delay(5000);
    }
}

void WiFiManager::CheckWiFiStatus(int output)
{
    // compare the previous status to the current status
    if (wifistatus != WiFi.status()) 
    {
        // it has changed update the variable
        wifistatus = WiFi.status();

        if (wifistatus == WL_AP_CONNECTED) {
            // a device has connected to the AP
            Serial.println("Device connected to AP");
        } else {
            // a device has disconnected from the AP, and we are back in listening mode
            Serial.println("Device disconnected from AP");
        }
    }

    WiFiClient client = webserver.available();   // listen for incoming clients

    if (client)
    {                             // if you get a client,
        Serial.println("new client");           // print a message out the serial port
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client.connected()) {            // loop while the client's connected
             delayMicroseconds(10);                // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
             if (client.available()) {             // if there's bytes to read from the client,
                 char c = client.read();             // read a byte, then
                 Serial.write(c);                    // print it out to the serial monitor
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
                        client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
                        client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");

                        // The HTTP response ends with another blank line:
                        client.println();
                        // break out of the while loop:
                        break;
                    }
                    else {      // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                }
                else if (c != '\r') {    // if you got anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                }
             }
        }
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }
   
    // if (client) {                             // if you get a client,
    //   Serial.println("New client");           // print a message out the serial port
    //   // WiFiDrv::analogWrite(R, 0);   //RED
    //   // WiFiDrv::analogWrite(G, 16);  //GREEN
    //   // WiFiDrv::analogWrite(B, 0);   //BLUE

    //   String currentLine = "";                // make a String to hold incoming data from the client
    //   String request = "";

    //   while (client.connected()) {            // loop while the client's connected
    //     //delayMicroseconds(10);                // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.

    //     // loop while the client's connected
    //     // while (client.available()) {
    //     //   char c = client.read();       // read a byte
    //     //   request += c;
    //     // }

    //     if (client.available()) {             // if there's bytes to read from the client,
    //       char c = client.read();             // read a byte, then
    //       Serial.write(c);                    // print it out to the serial monitor
    //       if (c == '\n') {                    // if the byte is a newline character

    //         // if the current line is blank, you got two newline characters in a row.
    //         // that's the end of the client HTTP request, so send a response:
    //         if (currentLine.length() == 0) {
    //           // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    //           // and a content-type so the client knows what's coming, then a blank line:
    //           client.println("HTTP/1.1 200 OK");
    //           client.println("Content-type:text/html");
    //           client.println();

    //           // the content of the HTTP response follows the header:
    //           client.print(output);
              
    //           // The HTTP response ends with another blank line:
    //           client.println();
    //           // break out of the while loop:
    //           break;
    //         }
    //         else {      // if you got a newline, then clear currentLine:
    //           currentLine = "";
    //         }
    //       }
    //       else if (c != '\r') {    // if you got anything else but a carriage return character,
    //         currentLine += c;      // add it to the end of the currentLine
    //       }
    //     }
        

    //     // String path = request.substring(request.indexOf(' ') + 1, request.indexOf(' ', request.indexOf(' ') + 1));
    //     // if (path.endsWith("/sse")) {
    //     //   // Send SSE headers
    //     //   client.println("HTTP/1.1 200 OK");
    //     //   client.println("Content-type:text/event-stream");
    //     //   client.println("Cache-Control: no-cache");
    //     //   client.println("Connection: keep-alive");
    //     //   client.println();

    //     //   // Send data
    //     //   //while (client.connected()) {                  
    //     //     client.print("data: ");
    //     //     client.print(_output);  // Send sensor data
    //     //     client.println();  // End of event
    //     //     client.println("retry: 5\n\n");  // retry
    //     //     client.println();  // End of event
    //     //     //delay(100);  // Update every second
    //     //     //break;
    //     //   //}
    //     // }
    //     // else{
        
    //     //   Serial.println("Serving default page");              // print it out the serial monitor
    //     //   // Serve the HTML page
    //     //   client.println("HTTP/1.1 200 OK");
    //     //   client.println("Content-type:text/html");
    //     //   client.println("Connection: close");
    //     //   client.println();

    //     //   client.println(_output);

    //     //   // the content of the HTTP response follows the header:
    //     //   // client.println("<!DOCTYPE html>");
    //     //   // client.println("<html>");
    //     //   // client.println("<head>");
    //     //   // client.println("<title>Live Data</title>");
    //     //   // client.println("</head>");
    //     //   // client.println("<body>");
    //     //   // client.println("<h1>Live Data</h1>");
    //     //   // client.println("<p id='data'>Loading...</p>");
    //     //   // // client.println("<script>");
    //     //   // // client.println("const eventSource = new EventSource('/sse');");
    //     //   // // client.println("eventSource.onmessage = function(event) {");
    //     //   // // client.println("  document.getElementById('data').textContent = 'Travel:' + event.data + 'mm';");
    //     //   // // client.println("};");
    //     //   // // client.println("</script>");
    //     //   // client.println("</body>");
    //     //   // client.println("</html>");
    //     //   //client.print("Click <a href=\"/H\">here</a> turn the LED on pin 9 on<br>");
    //     //   //client.print("Click <a href=\"/L\">here</a> turn the LED on pin 9 off<br>");

    //     //   // The HTTP response ends with another blank line:
    //     //   client.println();
    //     // }    
    //   } // while connected

    //   // close the connection:
    //   client.stop();      
    //   Serial.println("client disconnected");

    //   // WiFiDrv::analogWrite(R, 0);    //RED
    //   // WiFiDrv::analogWrite(G, 0);    //GREEN
    //   // WiFiDrv::analogWrite(B, 16);   //BLUE
    // } // if client
}

void WiFiManager::PrintWiFiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}