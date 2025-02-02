#include <ESP8266WiFi.h>       // Library to enable Wi-Fi functionality on ESP8266
#include <ESP8266WebServer.h>  // Library to create a simple web server

// Wi-Fi credentials (Replace with actual SSID and password)
const char* ssid = "Samsung A15 5g";  // Name of the Wi-Fi network
const char* password = "ABCD1234567890";  // Wi-Fi password

// Static IP Configuration (Useful when integrating with a mobile app that relies on a fixed IP)
IPAddress local_ip(192, 168, 137, 138);  // Assign a fixed IP to ESP8266
IPAddress gateway(192, 168, 137, 1);     // Router's Gateway IP (Default access point for devices)
IPAddress subnet(255, 255, 255, 0);      // Subnet mask to define the network range

// ESP8266 GPIO pins that are connected to relays (to control appliances)
const int espPins[] = {D1, D2, D3, D4};  // Define which GPIO pins control the relays

// Create an instance of the ESP8266WebServer class to run a web server on port 80
ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
  Serial.println("Setting up ESP8266...");

  WiFi.mode(WIFI_STA);  // Set ESP8266 as a Station (STA) mode to connect to a router
  WiFi.disconnect();    // Ensure previous Wi-Fi connections are disconnected

  WiFi.begin(ssid, password);  // Attempt to connect to the specified Wi-Fi network
  Serial.print("Connecting to Wi-Fi");
  
  // Wait until ESP8266 successfully connects to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); // Print dots to indicate ongoing connection attempt
  }

  // Once connected, assign the predefined static IP settings
  WiFi.config(local_ip, gateway, subnet);
  Serial.println("\nWi-Fi connected.");
  Serial.print("Assigned IP: ");
  Serial.println(WiFi.localIP());  // Print assigned static IP address

  // Configure the relay control pins as OUTPUT and turn them OFF initially
  for (int i = 0; i < 4; i++) {
    pinMode(espPins[i], OUTPUT);
    digitalWrite(espPins[i], LOW); // Ensure relays are OFF at startup
  }

  // Define web routes to control the relays remotely
  server.on("/", []() {
    server.send(200, "text/plain", "Welcome to the ESP8266 Relay Control Web Server");
  });

  // Loop to define ON and OFF routes for each relay
  for (int i = 0; i < 4; i++) {
    // Create an ON route for each relay
    server.on(String("/relay" + String(i + 1) + "/on").c_str(), [i]() {
      digitalWrite(espPins[i], HIGH);  // Turn ON the relay
      server.send(200, "text/plain", "Relay " + String(i + 1) + " turned ON");
      Serial.println("Relay " + String(i + 1) + " turned ON");
    });

    // Create an OFF route for each relay
    server.on(String("/relay" + String(i + 1) + "/off").c_str(), [i]() {
      digitalWrite(espPins[i], LOW);  // Turn OFF the relay
      server.send(200, "text/plain", "Relay " + String(i + 1) + " turned OFF");
      Serial.println("Relay " + String(i + 1) + " turned OFF");
    });
  }

  // Start the web server to listen for incoming requests
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();  // Continuously listen for client requests and process them
}
