/*
BRT Racing Data Tools microcontroller code

TODO
RTC timestamping
Space monitoring
*/
#include <Arduino_BuiltIn.h>
#include <string.h>
#include <SPI.h>
#include <SD.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

#include "src/WebServer.h"
#include "src/WiFiManager.h"
#include "src/AnalogReader.h"
#include "src/SDManager.h"

#define R 26
#define G 25
#define B 27

const int BOARD_DELAY = 20; // 20Hz
const int BAUD = 9600;
const bool PLOT_BOUNDS = false;
const bool PLOT_SERIAL = false;

// AiM Eclipse 5V spec
// 500mV = 150mm, 4500mV = 0mm 
// Observed with 12V input
// 0.55 = 150mm, 4.35 = 0mm 
const float POT_VOLTAGE = 5.0;
const float POT_TRAVEL_MM = 150.0;
const float POT_MIN_V = 0.55;
const float POT_MAX_V = 4.35;
const int POT_RESOLUTION = 1023;
const uint8_t POT_PIN = A0;
const int V_OUT = 6;

// SD chip select pin for Adafruit Logging Shield
const int SD_CHIP_SELECT = 10;
const char SEPERATOR = ',';

AnalogReader analogReader;
//SDManager sdManager;
WebServer webServer;
WiFiManager wifiManager;

char ssid[] = "BRT_DataTools";   // network SSID (name)
int status = WL_IDLE_STATUS;
WiFiServer server(80);
char dataLogFile[] = "data000.log";

void setup() 
{  
  // Initialize serial communication
  Serial.begin(BAUD);
  SPI.begin();

  // Sensor power
  pinMode(V_OUT, OUTPUT);  
  digitalWrite(V_OUT, HIGH); // set 5v output
      
  WiFiDrv::pinMode(R, OUTPUT);
  WiFiDrv::pinMode(G, OUTPUT);
  WiFiDrv::pinMode(B, OUTPUT);
  
  WiFiDrv::analogWrite(R, 16);  //RED
  WiFiDrv::analogWrite(G, 0);   //GREEN
  WiFiDrv::analogWrite(B, 0);   //BLUE

  // make sure that the default chip select pin is set to output, even if you don't use it:
  //sdManager.Init(SD_CHIP_SELECT);
  pinMode(SD_CHIP_SELECT, OUTPUT);
  
  if (!SD.begin(SD_CHIP_SELECT))
  //if (!card.init(SPI_HALF_SPEED, SD_CHIP_SELECT)) 
  {
      Serial.println("SD Initialization failed. Things to check:");
      Serial.println("* is a card inserted?");
      Serial.println("* is your wiring correct?");
      Serial.println("* did you change the chipSelect pin to match your shield or module?");        
      while (1);
  }

  Serial.println("SD card initialized.");

  //WriteCardInfo();

  // Create the new datalog file name
  int logiterator = FindNextAvailableNumber();
  char cstr[4];
  itoa(logiterator, cstr, 10);
  str_replace(dataLogFile, "000", cstr);
  Serial.print("Logging to ");
  Serial.println(dataLogFile);

  // if (SD.exists(dataLogFile))
  // {
  //     Serial.print("Removing ");
  //     Serial.println(dataLogFile);
  //     SD.remove(dataLogFile);
  // }
  // else
  // {
  //     Serial.print(dataLogFile);
  //     Serial.println(" does not exist");
  // }  
    
  //webServer.InitWebServer();
  //wifiManager.InitWiFiAccessPoint();  

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) 
  {
    Serial.println("Communication with WiFi module failed.");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) 
  {
    Serial.println("Please upgrade the firmware.");
  }

  // by default the local IP address will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid);
  if (status != WL_AP_LISTENING) 
  {
    Serial.println("Creating access point failed");
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();
  // server is connected now, so print out the status
  PrintWiFiStatus();
}

void loop() 
{ 
  analogReader.ReadData();
  analogReader.GetLastData();

  int sensorvalue = analogRead(POT_PIN);
  float voltage = sensorvalue * (POT_VOLTAGE / POT_RESOLUTION);
  // Analog -> Digital transformation
  // Map potentiometer min/max values to 0-150mm range
  int mmtravel = Mapfloat(voltage, POT_MAX_V, POT_MIN_V, (float)0, POT_TRAVEL_MM);
  
  //sdManager.WriteData(String(mmtravel));
  File dataFile = SD.open(dataLogFile, FILE_WRITE);
     
  // if the file is available write to it:
  if (dataFile) 
  {
      dataFile.print(mmtravel);
      dataFile.print(SEPERATOR);
      dataFile.close();
  }
  else 
  {
      Serial.println("error opening dataLog file in loop");
      while (1);
  }
  
  if (PLOT_SERIAL)
    OutputToSerial(mmtravel, voltage, sensorvalue);

  //webServer.SetOutput(mmtravel);
  //wifiManager.CheckWiFiStatus(mmtravel);
  //webServer.ListenClients();

  // compare the previous status to the current status
  if (status != WiFi.status()) 
  {
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
  
  WiFiClient client = server.available();   // listen for incoming clients

  // if we get a client
  if (client) 
  {                             
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected()) 
    {            
      //delayMicroseconds(10);              // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
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

            File dataFile = SD.open(dataLogFile, FILE_READ);
            String ch;
            if (dataFile) 
            {
                while (dataFile.available())
                {
                  ch = dataFile.readStringUntil(SEPERATOR);
                  client.print(ch);
                  client.print(SEPERATOR);
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
          {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') 
        {    // if you got anything else but a carriage return character,
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

  delay(BOARD_DELAY);
}

// map function for float fractions
long Mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void OutputToSerial(int mmtravel, float raw_v, float sensorvalue)
{
    // Plotter range
    if (PLOT_BOUNDS)
    {
      Serial.print("Min:0,Max:");
      Serial.println(POT_MAX_V + 100);
    }

    // Serial output
    Serial.print("Travel:");
    Serial.print(mmtravel);

    Serial.print("  V:");
    Serial.print(raw_v);
    Serial.print("  Raw:");
    Serial.print(sensorvalue);

    // New line
    Serial.println();
}

void DataPoint()
{
  // 
  // 
  // Time
}

void PrintWiFiStatus() 
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

int FindNextAvailableNumber() 
{
  int maxNum = 0;
  //Sd2Card  card;
  File root = SD.open("/"); // Open root directory
  //SdVolume volume;

  //card.init(SPI_HALF_SPEED, SD_CHIP_SELECT);

   // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  //  if (!volume.init(card)) {
  //   Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
  //   while (1);
  // }

  // root.openRoot(volume);
  // root.ls(LS_R | LS_DATE | LS_SIZE);

  while (true) 
  {
      File entry = root.openNextFile();
      if (!entry) {
          break; // No more files
      }

      String fileName = entry.name();      
      Serial.print("File exits ");
      Serial.println(fileName);

      // Extract the number from the filename
      if (fileName.startsWith("DATA"))
      {
          // 7 is the length of "data" and 4 is the length of ".log"
          String numStr = fileName.substring(4, fileName.length() - 4); 
          int num = numStr.toInt();
          if (num > maxNum) {
              maxNum = num;
          }
      }

      entry.close();
  }
  root.close();

  return maxNum + 1;
}

// utility string function
void str_replace(char *src, char *oldchars, char *newchars) 
{ 
  char *p = strstr(src, oldchars);
  char buf[99];
  do 
  {
    if (p) 
    {
      memset(buf, '\0', strlen(buf));
      if (src == p) 
      {
        strcpy(buf, newchars);
        strcat(buf, p + strlen(oldchars));
      } 
      else 
      {
        strncpy(buf, src, strlen(src) - strlen(p));
        strcat(buf, newchars);
        strcat(buf, p + strlen(oldchars));
      }
      memset(src, '\0', strlen(src));
      strcpy(src, buf);
    }
  } while (p && (p = strstr(src, oldchars)));
}