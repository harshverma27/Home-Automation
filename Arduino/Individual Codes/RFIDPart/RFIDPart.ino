#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// Define RFID pins
const int SS_PIN = 53;
const int RST_PIN = 49;
const int servopin = 8;  // Servo motor pin for door control
const int Relay = 44;    // Relay pin for door lock
const int buzzer = 6;    // Buzzer for access denied
const int ACCESS_DELAY = 2000;
const int DENIED_DELAY = 1000;

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo s1;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  s1.attach(servopin);
  pinMode(Relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(Relay, LOW);
  noTone(buzzer);

  Serial.println("RFID System Initialized. Scan a card...");
}

void loop() {
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
  Serial.println(cardUID);

  if (cardUID == "0185C626") {  // Replace with your card's UID
    Serial.println("Access Granted");
    for (int i = 0; i < 91; i++) {
      s1.write(i);
    }
    digitalWrite(Relay, HIGH);
    delay(ACCESS_DELAY);
    for (int i = 0; i < 91; i++) {
      s1.write(91 - i);
    }
    digitalWrite(Relay, LOW);
    delay(500);
  } else {
    Serial.println("Access Denied");
    tone(buzzer, 300);
    delay(DENIED_DELAY);
    noTone(buzzer);
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
