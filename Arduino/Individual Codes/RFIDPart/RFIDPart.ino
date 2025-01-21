#include <SoftwareSerial.h>

const int relayPins[] = {23, 25, 27, 29, 31, 33, 35, 37}; // Relay pins
SoftwareSerial BTSerial(19, 18); // Bluetooth module on pins 19 (TX) and 18 (RX)

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  for (int i = 0; i < 8; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW); // Turn relays OFF initially
  }
  
  Serial.println("Bluetooth Relay Control Initialized.");
}

void loop() {
  controlRelaysWithBluetooth();
}

// Function to control relays using Bluetooth commands
void controlRelaysWithBluetooth() {
  if (BTSerial.available()) {
    char command = BTSerial.read(); // Read command from Bluetooth

    if (command >= 'A' && command <= 'H') {
      int relayIndex = command - 'A'; // Map 'A'-'H' to 0-7
      digitalWrite(relayPins[relayIndex], LOW); // Turn relay OFF
      Serial.print("Relay ");
      Serial.print(relayIndex + 1);
      Serial.println(" turned OFF via Bluetooth");
    } else if (command >= 'a' && command <= 'h') {
      int relayIndex = command - 'a'; // Map 'a'-'h' to 0-7
      digitalWrite(relayPins[relayIndex], HIGH); // Turn relay ON
      Serial.print("Relay ");
      Serial.print(relayIndex + 1);
      Serial.println(" turned ON via Bluetooth");
    } else {
      Serial.println("Invalid Bluetooth command received.");
    }
  }
}
