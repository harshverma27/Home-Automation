#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

// Define hardware pins
const int fireSensor = A0;             // Analog pin for fire sensor
const int smokeSensor = A1;            // Analog pin for smoke sensor
const int soundSensorPin = A2;         // Analog pin for sound sensor
const int servopin = 8;                // Servo motor pin (for door control)
const int buzzer = 6;                  // Buzzer pin (used for alarms)
const int SS_PIN = 53;                 // RFID module SS (Slave Select) pin
const int RST_PIN = 49;                // RFID module RST (Reset) pin
const int ACCESS_DELAY = 2000;         // Delay for authorized access (in ms)
const int DENIED_DELAY = 1000;         // Delay for access denied (in ms)
const int smokeLimit = 300;            // Threshold Limit Of Smoke Detection
const int fireLimit = 250;             // Threshold Limit Of Fire Detection
const int soundLimit = 75;             // Threshold Limit Of Soud Detection

// Relay control pins (for appliances or other systems)
const int relayPins[] = {23, 25, 27, 29};  
// ESP8266 input pins for controlling relays
const int espPins[] = {22, 24, 26, 28};

// Create RFID and Servo instances
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Servo s1;  // Servo motor instance

void setup() {
  s1.attach(servopin);   // Attach servo to the defined pin
  Serial.begin(9600);    // Initialize serial communication at 9600 baud rate
  SPI.begin();           // Initialize SPI communication (required for RFID module)
  mfrc522.PCD_Init();    // Initialize RFID module
  

  pinMode(buzzer, OUTPUT);
  noTone(buzzer);            // Ensure buzzer is off at startup
  
  // Set up sensor pins
  pinMode(fireSensor, INPUT);
  pinMode(smokeSensor, INPUT);
  pinMode(soundSensorPin, INPUT);

  Serial.println("RFID System Initialized. Ready for scanning.");

  // Set relay and ESP pins
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);  // Set relay pins as output
    pinMode(espPins[i], INPUT);     // Set ESP pins as input (to receive signals)
    digitalWrite(relayPins[i], LOW);  // Keep all relays OFF initially
  }
}

void loop() {
  handleRFIDAccess();         // Handle RFID-based access control
  controlRelaysWithESP();     // Control relays based on ESP8266 input signals
  detectFireAndSmoke();       // Detect fire and smoke
}

// Function to handle RFID card scanning and access control
void handleRFIDAccess() {
  // Check if a new card is present and if it can be read
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;  // Exit function if no card is detected
  }

  Serial.print("UID tag: ");
  String cardUID = "";  // Variable to store the UID of the scanned card

  // Read the card's UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  cardUID.toUpperCase();  // Convert UID to uppercase
  Serial.println(cardUID);  // Print detected UID

  // Check if scanned card matches the predefined authorized card UID
  if (cardUID == "0185C626") {  // Replace with your actual RFID card UID
    Serial.println("Access Granted");

    // Open door using servo motor
    for (int i = 0; i < 91; i++) {
      s1.write(i);
    }
    delay(ACCESS_DELAY);        // Keep door open for some time

    // Close the door after delay
    for (int i = 0; i < 91; i++) {
      s1.write(91 - i);
    }
    delay(500);  // Small delay for servo to complete motion
  } else {
    Serial.println("Access Denied");
    tone(buzzer, 300);   // Sound buzzer for denied access
    delay(DENIED_DELAY);
    noTone(buzzer);      // Stop buzzer after delay
  }

  mfrc522.PICC_HaltA();  // Stop reading the card
  mfrc522.PCD_StopCrypto1();  // End communication with RFID card
}

// Function to detect fire and smoke
void detectFireAndSmoke() {
  int smokeValue = analogRead(smokeSensor);  // Read smoke sensor value
  int fireValue = analogRead(fireSensor);    // Read fire sensor value

  // If fire or smoke is detected, trigger an alarm
  if (fireValue < fireLimit || smokeValue > smokeLimit) {
    digitalWrite(buzzer, HIGH);  // Turn ON buzzer
    Serial.println("Alert: Fire or smoke detected!");
  } else {
    digitalWrite(buzzer, LOW);  // Turn OFF buzzer
  }
}

// Function to detect sound intensity and control a relay
void detectSoundAndControlRelay() {
  int soundValue = analogRead(soundSensorPin);  // Read sound sensor value

  // If sound intensity crosses threshold, turn ON a relay
  if (soundValue > soundLimit) {  
    digitalWrite(relayPins[0], HIGH);  // Activate Relay 1 (connected to pin 23)
    Serial.println("Relay 1 turned ON due to loud sound.");
    delay(3000);  // Keep relay ON for 3 seconds
    digitalWrite(relayPins[0], LOW);  // Turn OFF relay after delay
    Serial.println("Relay 1 turned OFF.");
  }
}

// Function to control relays using ESP8266 signals
void controlRelaysWithESP() {
  for (int i = 0; i < 4; i++) {
    int espState = digitalRead(espPins[i]); // Read ESP8266 input pin state

    // If ESP8266 sends a HIGH signal, turn ON corresponding relay
    if (espState == HIGH) {
      digitalWrite(relayPins[i], HIGH); // Turn ON relay
      Serial.print("Relay ");
      Serial.print(i + 1);
      Serial.println(" turned ON via ESP pin");
    } else {
      digitalWrite(relayPins[i], LOW); // Turn OFF relay
    }
  }
}