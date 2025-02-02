// Define relay control pins (connected to appliances)
const int relayPins[] = {23, 25, 27, 29};

// Define ESP8266 input pins (to receive control signals)
const int espPins[] = {22, 24, 26, 28};

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging

  // Configure relay and ESP pins
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);  // Set relay pins as output (to control devices)
    pinMode(espPins[i], INPUT);     // Set ESP pins as input (to receive signals)
    digitalWrite(relayPins[i], LOW); // Keep all relays OFF initially
  }

  Serial.println("ESP Relay Control Initialized.");
}

void loop() {
  // Iterate through all relays and check ESP8266 signals
  for (int i = 0; i < 4; i++) {
    int espState = digitalRead(espPins[i]); // Read the state of the ESP pin

    if (espState == HIGH) { // If ESP8266 sends a HIGH signal
      digitalWrite(relayPins[i], HIGH); // Turn ON the corresponding relay
      Serial.print("Relay ");
      Serial.print(i + 1);
      Serial.println(" turned ON via ESP");
    } else { // If ESP8266 sends a LOW signal
      digitalWrite(relayPins[i], LOW); // Turn OFF the corresponding relay
    }
  }

  delay(500); // Short delay to reduce CPU load
}
