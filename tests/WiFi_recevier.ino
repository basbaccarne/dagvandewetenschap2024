/*
This is a sample script to make two Arduino Nano 33IOTs talk to each other over
WiFi WiFi logic:
* Both Arduino Nano 33IOTs need to be connected to the same network
* One Arduino Nano 33IOT acts as a server, the other as a client
* The server listens for incoming connections
*/

// Code for the receiver (client)
// Works on XIAO ESP32

// libraries
#include <WiFi.h>

// credentials
const char* ssid = "escape";
const char* password = "tempdvdw24";

