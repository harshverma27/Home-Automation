#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

// Define hardware pins
const int fireSensor = A0;
const int servopin = 8;
const int smokeSensor = A1;
const int buzzer = 6;
const int Relay = 44;                        // Door relay pin for RFID system
const int SS_PIN = 53;                        // RFID SS pin
const int RST_PIN = 49;                      // RFID reset pin
const int ACCESS_DELAY = 2000;               // Delay for authorized access
const int DENIED_DELAY = 1000;               // Delay for access denied
const int relayPins[] = { 23, 25, 27, 29 };  // Relay pins
const int espPins[] = { 22, 24, 26, 28 };    // ESP GPIO pins
const int soundSensorPin = A2;               // Sound sensor pin (use an analog pin for intensity)             // Sound threshold to detect loud noise

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Servo s1;

void setup() {
  s1.attach(servopin);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(Relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(Relay, LOW);
  noTone(buzzer);
  pinMode(fireSensor, INPUT);
  pinMode(smokeSensor, INPUT);
  pinMode(soundSensorPin, INPUT);
  Serial.println("RFID System Initialized. Ready for scanning.");

  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    pinMode(espPins[i], INPUT);
    digitalWrite(relayPins[i], LOW);  // Turn relays OFF initially
  }
}

void loop() {
  handleRFIDAccess();
  controlRelaysWithESP();
  detectFireAndSmoke();
  detectSoundAndControlRelay();
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

// Function to detect fire and smoke
void detectFireAndSmoke() {
  int smokeValue = analogRead(smokeSensor);
  int fireValue = analogRead(fireSensor);

  if (fireValue < 250 || smokeValue > 300 ) {
    digitalWrite(buzzer, HIGH);  // Activate buzzer
    Serial.println("Alert: Fire or smoke detected!");
  } else {
    digitalWrite(buzzer, LOW);  // Deactivate buzzer
  }
}

// Function to detect sound intensity and control relay
void detectSoundAndControlRelay() {
  int soundValue = analogRead(soundSensorPin);  // Read sound sensor value (0-1)

  if (soundValue > 75) {  // If sound is above threshold
    digitalWrite(relayPins[0], HIGH);  // Turn on Relay 1 (connected to pin 23)
    Serial.println("Relay 1 turned ON due to loud sound.");
    delay(3000);  // Keep Relay 1 ON for 2 seconds
    digitalWrite(relayPins[0], LOW);  // Turn off Relay 1 after 2 seconds
    Serial.println("Relay 1 turned OFF.");
  }
}
void controlRelaysWithESP() {
  for (int i = 0; i < 4; i++) {
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