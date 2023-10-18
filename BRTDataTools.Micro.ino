/*
BRT Racing Data Tools microcontroller code

TODO LIST:

    Read pot data
    Map pot data to range (0-150mm)
    Sample rate
    Start reading when motion/speed exceeds threshold
    Stop reading when motion/speed is below threshold

    Save data to SD card

    Open WiFi access point
    Transfer data to PC via WiFi

*/
#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include <LiquidCrystal.h>
//#include <avr_debugger.h>

#include "arduino_secrets.h"

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);      // put your pin numbers here
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define R 26
#define G 25
#define B 27

char ssid[] = SECRET_SSID; 
char ssid_access_point[] = "GPDataTools";      
char pass[] = SECRET_PASS;      // network password (use for WPA, or use as key for WEP)

int keyIndex = 0;               // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

#define V_OUT 3
#define POT_PIN (A0)

int potValue;
int angle;

void setup() {

  Serial.begin(9600);           // initialize serial communication
  
  // power
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(V_OUT, OUTPUT);      // set the LED pin mode
  
  digitalWrite(V_OUT, HIGH);   // set 5v output
  //analogWrite(V_OUT, 25);  

  WiFiDrv::pinMode(25, OUTPUT);
  WiFiDrv::pinMode(26, OUTPUT);
  WiFiDrv::pinMode(27, OUTPUT);
  
  WiFiDrv::analogWrite(26, 16);  //RED
  WiFiDrv::analogWrite(25, 0);   //GREEN
  WiFiDrv::analogWrite(27, 0);   //BLUE

  // attempt to connect to WiFi network:
  // while (status != WL_CONNECTED) {
  //   Serial.print("Attempting to connect to Network named: ");
  //   Serial.println(ssid);                   // print the network name (SSID);

  //   // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  //   status = WiFi.begin(ssid, pass);
  //   // wait 5 seconds for connection:
  //   delay(5000);
  // }
  //server.begin();                           // start the web server on port 80
  //printWifiStatus();                        // you're connected now, so print out the status

  digitalWrite(V_OUT, HIGH);          // set 5v output
  //analogWrite(V_OUT, 25);

  WiFiDrv::analogWrite(R, 0);    //RED
  WiFiDrv::analogWrite(G, 0);    //GREEN
  WiFiDrv::analogWrite(B, 16);   //BLUE

  InitWiFiAccessPoint();
  InitWebServer();

  // set the cursor to column 0, line 1
  // line 1 is the second row, since counting begins with 0
  lcd.setCursor(0, 1);

  lcd.begin(16, 2);                          // put your LCD parameters here
  lcd.print("Travel     Avg");
  lcd.setCursor(4,1);
  lcd.print("mm");
}

void loop() {  
  
  potValue = analogRead(POT_PIN);
  // Analog -> Digital transformation
  // Map potentiometer min/max values to 0-150mm range
  int mmtravel = map(potValue, 0, 1023, 0, 150);
  
  //OutputToSerial(mmtravel);
  OutputToLCD(mmtravel);

  CheckWiFiStatus();
  WebServerListenClients();

  delay(50);
}

void OutputToSerial(int mmtravel)
{
    // Plotter init
    Serial.println("Min:-2,Max:151");
    // plotter output
    Serial.print("Travel: ");
    Serial.print(mmtravel);
    Serial.println("mm");
}

void OutputToLCD(int mmtravel)
{
    // output to LCD
    lcd.setCursor(0,1);
    lcd.print("          ");
    lcd.setCursor(0,1);
    lcd.print(mmtravel);
    lcd.setCursor(11,1);
    lcd.print("80 mm");
    lcd.setCursor(4,1);
    lcd.print("mm");
}

void InitWiFiAccessPoint(){
  Serial.println("Access Point Web Server");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid_access_point);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid_access_point);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(5000);

  // start the web server on port 80
  //server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();  
}

void InitWebServer() {

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

  WiFiDrv::analogWrite(26, 16);  //RED
  WiFiDrv::analogWrite(25, 0);   //GREEN
  WiFiDrv::analogWrite(27, 0);   //BLUE

  //ConnectWifiNetwork();
  server.begin();                           // start the web server on port 80
    
  WiFiDrv::analogWrite(R, 0);    //RED
  WiFiDrv::analogWrite(G, 0);    //GREEN
  WiFiDrv::analogWrite(B, 16);   //BLUE
}

void ConnectWiFiNetwork(){
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 5 seconds for connection:
    delay(5000);
  }
}

void CheckWiFiStatus(){
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }
}

void WebServerListenClients() {

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

void printWiFiStatus() {
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
}


