/*
This is a sample script to make two Arduino Nano 33IOTs talk to each other over
WiFi WiFi logic:
* Both Arduino Nano 33IOTs need to be connected to the same network
* One Arduino Nano 33IOT acts as a server, the other as a client
* The server listens for incoming connections
*/

// Code for the sender (server)
// Works on XIAO ESP32

// libraries
#include <WiFi.h>

// credentials
const char* ssid = "escape";
const char* password = "tempdvdw24";

// Set Static IP address
IPAddress local_IP(192,168,244,71);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Create a server object that listens on port 80
WiFiServer server(80);

// Variable to store the main reading (random dummy data)
int data = 0;  

void setup() {
  // high baud rates are better for accuracy
  Serial.begin(115200);
  delay(10);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Configure the static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to the WiFi network (while loop)
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // confirm if connected (while loop escaped)
  Serial.println("");
  Serial.print("Connected to: ");
  Serial.println(ssid);
  Serial.print("With IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server online.");

}
void loop() {

  // Update sample data
  data = random(1, 100);

  // Listen for incoming clients
  WiFiClient client = server.available();

  // If you get a client
  if (client) {               
    // Make a String to hold incoming data from the client    
    String currentLine = "";      
    // Hold the while loop as long as request data is incoming  
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          // If the current line is blank, you got two newline characters in a row.
          // That's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/plain");
            client.println();
            Serial.println("Got a request!");  

            // Send the current value of the data variable
            client.print(data);  
            // Break out of the while loop:
            break;

          } else {  // If you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // If you got anything else but a carriage return character,
          currentLine += c;      // Add it to the end of the currentLine
        }
      }
    }
    // Close the connection:
    client.stop();
    Serial.print("Response sent: ");
    Serial.println(data);
  }
}