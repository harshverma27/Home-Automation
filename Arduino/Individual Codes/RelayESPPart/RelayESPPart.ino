const int relayPins[] = {23, 25, 27, 29, 31, 33, 35, 37}; // Relay pins
const int espPins[] = {22, 24, 26, 28, 30, 32, 34, 36};   // ESP GPIO pins

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 8; i++) {
    pinMode(relayPins[i], OUTPUT);
    pinMode(espPins[i], INPUT);
    digitalWrite(relayPins[i], LOW); // Turn relays OFF initially
  }
  
  Serial.println("ESP GPIO Relay Control Initialized.");
}

void loop() {
  controlRelaysWithESP();
}

// Function to control relays using ESP GPIO pins
void controlRelaysWithESP() {
  for (int i = 0; i < 8; i++) {
    int espState = digitalRead(espPins[i]); // Read ESP GPIO pin state
    if (espState == HIGH) {
      digitalWrite(relayPins[i], HIGH); // Turn relay ON if ESP pin is HIGH
      Serial.print("Relay ");
      Serial.print(i + 1);
      Serial.println(" turned ON via ESP pin");
    } else {
      digitalWrite(relayPins[i], LOW); // Turn relay OFF if ESP pin is LOW
    }
  }
}
