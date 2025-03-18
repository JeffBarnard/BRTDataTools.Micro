#include "WiFiManager.h"

#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

#define R 26
#define G 25
#define B 27

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

    // wait for connection:
    //delay(10000);
    
    // Start the web server on port 80
    //webserver.begin();
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
}

void WiFiManager::PrintWiFiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // Print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("Signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print where to go in a browser:
    Serial.print("To see this page in action, open a browser to http://");
    Serial.println(ip);
}