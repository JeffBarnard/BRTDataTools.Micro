#include "WiFiManager.h"
#include "arduino_secrets.h"

#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

char ssid_access_point[] = "BRT_DataTools";  
char ssid[] = SECRET_SSID;      // your network SSID (name)    
char pass[] = SECRET_PASS;      // network password (use for WPA, or use as key for WEP)
int keyIndex = 0;               // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;    // the Wifi radio's status

void WiFiManager::ConnectWiFiNetwork(){
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

void WiFiManager::CheckWiFiStatus(){
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

void WiFiManager::InitWiFiAccessPoint(){
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
        
        
        
        //printWiFiStatus();  
    }

// void printWiFiStatus() {
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
// }
