#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

// Constants for access delays
const int ACCESS_DELAY = 2000; // Delay for authorized access
const int DENIED_DELAY = 1000; // Delay for access denied

// Pin Definitions
const int buzzer = 6;  
const int smokeSensor = A0;
const int fireSensor = 9;
const int relayPins[] = {23, 25, 27, 29, 31, 33, 35, 37}; // Relay pins
const int espPins[] = {22, 24, 26, 28, 30, 32, 34, 36};   // ESP GPIO pins
const int Relay = 44; // Door relay pin for RFID system
const int SS_PIN = 53; // RFID SS pin
const int RST_PIN = 49; // RFID reset pin

// Bluetooth Communication
SoftwareSerial BTSerial(19, 18); // Bluetooth module on pins 19 (TX) and 18 (RX)

// RFID Communication
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  BTSerial.begin(9600); // Bluetooth module

  // Initialize SPI bus and RFID reader
  SPI.begin();
  mfrc522.PCD_Init();

  // Print startup message
  Serial.println("System Initialized. Ready for RFID scanning, Bluetooth, and ESP control.");

  // Pin configurations
  pinMode(Relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fireSensor, INPUT);
  pinMode(smokeSensor, INPUT);

  // Set relay pins and ESP pins as output/input
  for (int i = 0; i < 8; i++) {
    pinMode(relayPins[i], OUTPUT);
    pinMode(espPins[i], INPUT);
    digitalWrite(relayPins[i], LOW); // Turn relays OFF initially
  }

  // Ensure door relay and buzzer are OFF
  digitalWrite(Relay, LOW);
  noTone(buzzer);
}

void loop() {
  // Fire and Smoke Detection
  detectFireAndSmoke();

  // Bluetooth Relay Control
  controlRelaysWithBluetooth();

  // ESP GPIO Pin Relay Control
  controlRelaysWithESP();

  // RFID Authentication for Door Access
  handleRFIDAccess();
}

// Function to detect fire or smoke and sound a buzzer
void detectFireAndSmoke() {
  int smokeValue = analogRead(smokeSensor);
  int fireValue = digitalRead(fireSensor);

  if (smokeValue > 200 || fireValue == 0) {  
    analogWrite(buzzer, 50); // Activate buzzer
    Serial.println("Alert: Fire or smoke detected!");
  } else {
    analogWrite(buzzer, 0); // Deactivate buzzer
  }
}

// Function to control relays using Bluetooth commands
void controlRelaysWithBluetooth() {
  if (BTSerial.available()) {
    char command = BTSerial.read(); // Read command from Bluetooth

    // Map commands to relay control
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

// Function to handle RFID card scanning and access control
void handleRFIDAccess() {
  // Check if a new RFID card is present
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return; // Exit if no card is detected
  }

  // Print UID of the detected card
  Serial.print("UID tag: ");
  String cardUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  cardUID.toUpperCase(); // Convert UID to uppercase
  Serial.println();

  // Check if the card is authorized
  if (cardUID == "ABCD1234") { // Replace with your card's UID
    Serial.println("Access Granted");
    digitalWrite(Relay, HIGH); // Unlock door
    delay(ACCESS_DELAY);
    digitalWrite(Relay, LOW); // Lock door again
  } else {
    Serial.println("Access Denied");
    tone(buzzer, 300); // Sound access denied buzzer
    delay(DENIED_DELAY);
    noTone(buzzer);
  }
}

