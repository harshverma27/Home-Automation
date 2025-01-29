#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi credentials
const char* ssid = "Samsung A15 5g"; // Replace with your Wi-Fi SSID
const char* password = "ABCD1234567890";   // Replace with your Wi-Fi password
// ESP8266 pins connected to Arduino Mega
const int espPins[] = {D1, D2, D3, D4};  // GPIO pins for relays

// Create a web server on port 80
ESP8266WebServer server(80);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Setting up ESP8266...");

  // Set ESP pins as OUTPUT
  for (int i = 0; i < 4; i++) {
    pinMode(espPins[i], OUTPUT);
    digitalWrite(espPins[i], LOW); // Ensure pins are LOW initially
  }

  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWi-Fi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Define routes for the web server
  server.on("/", []() {
    server.send(200, "text/plain", "Welcome to the ESP8266 Relay Control Web Server");
  });

  // Relay control routes
  for (int i = 0; i < 4; i++) {
    // Create ON route for each relay
    server.on(String("/relay" + String(i + 1) + "/on").c_str(), [i]() {
      digitalWrite(espPins[i], HIGH);
      server.send(200, "text/plain", "Relay " + String(i + 1) + " turned ON");
      Serial.println("Relay " + String(i + 1) + " turned ON");
    });

    // Create OFF route for each relay
    server.on(String("/relay" + String(i + 1) + "/off").c_str(), [i]() {
      digitalWrite(espPins[i], LOW);
      server.send(200, "text/plain", "Relay " + String(i + 1) + " turned OFF");
      Serial.println("Relay " + String(i + 1) + " turned OFF");
    });
  }

  // Start the server
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
}
