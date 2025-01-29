#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>

#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11);  // Bluetooth module on pins 10 (RX) and 11 (TX)
Servo s1;
const int fireSensor = 2;
const int smokeSensor = A0;
const int buzzer = 6;
const int Relay = 44;                        // Door relay pin for RFID system
const int SS_PIN = 53;                        // RFID SS pin
const int RST_PIN = 49;                      // RFID reset pin
const int ACCESS_DELAY = 2000;               // Delay for authorized access
const int DENIED_DELAY = 1000;               // Delay for access denied
const int relayPins[] = { 23, 25, 27, 29 };  // Relay pins
const int espPins[] = { 22, 24, 26, 28 };    // ESP GPIO pins

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  s1.attach(8);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(Relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(Relay, LOW);
  noTone(buzzer);
  pinMode(fireSensor, INPUT);
  pinMode(smokeSensor, INPUT);
  Serial.println("RFID System Initialized. Ready for scanning.");
  BTSerial.begin(9600);
  Serial.println("Bluetooth & ESP Relay Control Initialized.");

  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    pinMode(espPins[i], INPUT);
    digitalWrite(relayPins[i], LOW);  // Turn relays OFF initially
  }
}

void loop() {
  controlRelays();
}

// Function to handle RFID card scanning and access control
void handleRFIDAccess() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID tag: ");
  String cardUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  cardUID.toUpperCase();
  Serial.println(cardUID);  // Print detected UID

  if (cardUID == "0185C626") {  // Replace with your card's UID
    Serial.println("Access Granted");
    for (int i = 0; i < 91; i++) {
      s1.write(i);
    }
    digitalWrite(Relay, HIGH);  // Unlock door
    delay(ACCESS_DELAY);
    for (int i = 0; i < 91; i++) {
      s1.write(91 - i);
    }
    digitalWrite(Relay, LOW);  // Lock door again
    delay(500);                // Give time for servo to move
  } else {
    Serial.println("Access Denied");
    tone(buzzer, 300);
    delay(DENIED_DELAY);
    noTone(buzzer);
  }

  mfrc522.PICC_HaltA();  // Stop reading card
  mfrc522.PCD_StopCrypto1();
}

// Function to handle Bluetooth & ESP control together
bool relayStates[4] = {false, false, false, false};  // Track relay states (ON or OFF)
int espStateLast[4] = {LOW, LOW, LOW, LOW};  // Track last state of ESP pins to avoid repeated checks

void controlRelays() {
  // Read Bluetooth input if available
  char command = '\0';  // Default: No command
  if (BTSerial.available()) {
    command = BTSerial.read();
    Serial.print("Received Bluetooth Command: ");
    Serial.println(command);
  }

  for (int i = 0; i < 4; i++) {
    int espState = digitalRead(espPins[i]);  // Read ESP GPIO pin state

    bool desiredState = relayStates[i];  // Default to current relay state

    // Check for Bluetooth command
    if (command == ('a' + i)) {
      desiredState = true;  // Turn ON via Bluetooth
    } 
    else if (command == ('A' + i)) {
      desiredState = false;  // Turn OFF via Bluetooth
    }

    // Check for ESP state change
    if (espState != espStateLast[i]) {
      // Update the ESP state
      espStateLast[i] = espState;

      // If ESP state is HIGH, relay should be ON; if LOW, relay should be OFF
      if (espState == HIGH) {
        desiredState = true;  // Turn ON via ESP
      } else {
        desiredState = false;  // Turn OFF via ESP
      }
    }

    // Change relay state if necessary
    if (relayStates[i] != desiredState) {
      relayStates[i] = desiredState;  // Update internal state

      if (desiredState) {
        digitalWrite(relayPins[i], HIGH);  // Turn relay ON
        Serial.print("Relay ");
        Serial.print(i + 1);
        Serial.println(" turned ON");
      }
      else {
        digitalWrite(relayPins[i], LOW);  // Turn relay OFF
        Serial.print("Relay ");
        Serial.print(i + 1);
        Serial.println(" turned OFF");
      }
    }
  }
}



// Function to detect fire and smoke
void detectFireAndSmoke() {
  int smokeValue = analogRead(smokeSensor);
  int fireValue = digitalRead(fireSensor);

  if (smokeValue > 200 || fireValue == 0) {
    analogWrite(buzzer, 50);  // Activate buzzer
    Serial.println("Alert: Fire or smoke detected!");
  } else {
    analogWrite(buzzer, 0);  // Deactivate buzzer
  }
}
