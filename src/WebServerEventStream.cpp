// //#include "WebServer.h"

// #include <WiFiNINA.h>
// #include <utility/wifi_drv.h>

// WiFiServer server(80);

// void InitWebServer() {

//     WiFiDrv::pinMode(25, OUTPUT);
//     WiFiDrv::pinMode(26, OUTPUT);
//     WiFiDrv::pinMode(27, OUTPUT);

//     // // check for the WiFi module:
//     // if (WiFi.status() == WL_NO_MODULE) {
//     //   Serial.println("Communication with WiFi module failed!");
//     //   // don't continue
//     //   while (true);
//     // }

//     String fv = WiFi.firmwareVersion();
//     if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
//         Serial.println("Please upgrade the firmware");
//     }

//     WiFiDrv::analogWrite(R, 16);  //RED
//     WiFiDrv::analogWrite(G, 0);   //GREEN
//     WiFiDrv::analogWrite(B, 0);   //BLUE

//     delay(500);
//     //ConnectWifiNetwork();
//     server.begin();                           // start the web server on port 80
   
//     // print the SSID of the network you're attached to:
//     Serial.print("SSID: ");
//     Serial.println(WiFi.SSID());

//     // print your board's IP address:
//     IPAddress ip = WiFi.localIP();
//     Serial.print("IP Address: ");
//     Serial.println(ip);

//     // print the received signal strength:
//     long rssi = WiFi.RSSI();
//     Serial.print("signal strength (RSSI):");
//     Serial.print(rssi);
//     Serial.println(" dBm");
    
//     Serial.print("Open a browser to http://");
//     Serial.println(ip);
        
//     WiFiDrv::analogWrite(R, 0);    //RED
//     WiFiDrv::analogWrite(G, 0);    //GREEN
//     WiFiDrv::analogWrite(B, 16);   //BLUE
// }

// void SetOutput(int output)
// {
//   //_output = output;
// }

// void ListenClients() {
//     WiFiClient client = server.available();   // listen for incoming clients

//     if (client) {                             // if you get a client,
//       Serial.println("new client");           // print a message out the serial port
//       String currentLine = "";                // make a String to hold incoming data from the client
//       String request = "";

//       // loop while the client's connected
//       while (client.connected() && client.available()) {
//         WiFiDrv::analogWrite(R, 0);   //RED
//         WiFiDrv::analogWrite(G, 16);  //GREEN
//         WiFiDrv::analogWrite(B, 0);   //BLUE

//         char c = client.read();       // read a byte
//         request += c;
//       }

//       Serial.print(request);                    // print it out the serial monitor        

//       String path = request.substring(request.indexOf(' ') + 1, request.indexOf(' ', request.indexOf(' ') + 1));
//       if (path.endsWith("/sse")) {
//         // Send SSE headers
//         client.println("HTTP/1.1 200 OK");
//         client.println("Content-type:text/event-stream");
//         client.println("Cache-Control: no-cache");
//         client.println("Connection: keep-alive");
//         client.println();

//         // Send data
//         //while (client.connected()) {                  
//           client.print("data: ");
//           //client.print(_output);  // Send sensor data
//           client.println();  // End of event
//           client.println("retry: 5\n\n");  // retry
//           client.println();  // End of event
//           //delay(100);  // Update every second
//           //break;
//         //}
//       }
//       else{
      
//         Serial.println("Serving default page");              // print it out the serial monitor
//         // Serve the HTML page
//         client.println("HTTP/1.1 200 OK");
//         client.println("Content-type:text/html");
//         client.println("Connection: close");
//         client.println();

//         // the content of the HTTP response follows the header:
//         client.println("<!DOCTYPE html>");
//         client.println("<html>");
//         client.println("<head>");
//         client.println("<title>Live Data</title>");
//         client.println("</head>");
//         client.println("<body>");
//         client.println("<h1>Live Data</h1>");
//         client.println("<p id='data'>Loading...</p>");
//         client.println("<script>");
//         client.println("const eventSource = new EventSource('/sse');");
//         client.println("eventSource.onmessage = function(event) {");
//         client.println("  document.getElementById('data').textContent = 'Travel:' + event.data + 'mm';");
//         client.println("};");
//         client.println("</script>");
//         client.println("</body>");
//         client.println("</html>");
//         //client.print("Click <a href=\"/H\">here</a> turn the LED on pin 9 on<br>");
//         //client.print("Click <a href=\"/L\">here</a> turn the LED on pin 9 off<br>");

//         // The HTTP response ends with another blank line:
//         client.println();
//       }      

//       // close the connection:
//       client.stop();      
//       Serial.println("client disconnected");

//       WiFiDrv::analogWrite(R, 0);    //RED
//       WiFiDrv::analogWrite(G, 0);    //GREEN
//       WiFiDrv::analogWrite(B, 16);   //BLUE
//     } // if client

//     delay(10);
// }
