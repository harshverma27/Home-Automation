#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

Servo s1;
const int buzzer = 6;  
const int Relay = 44; // Door relay pin for RFID system
const int SS_PIN = 53; // RFID SS pin
const int RST_PIN = 49; // RFID reset pin
const int ACCESS_DELAY = 2000; // Delay for authorized access
const int DENIED_DELAY = 1000; // Delay for access denied

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  s1.attach(8);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(Relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(Relay, LOW);
  noTone(buzzer);
  Serial.println("RFID System Initialized. Ready for scanning.");
}

void loop() {
  handleRFIDAccess();
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

  if (cardUID == "0185C626") { // Replace with your card's UID
    Serial.println("Access Granted");
    for(int i = 0; i < 91; i++){
      s1.write(i);
    }
    digitalWrite(Relay, HIGH); // Unlock door
    delay(ACCESS_DELAY);
    for(int i = 0; i < 91; i++){
      s1.write(91-i);
    }
    digitalWrite(Relay, LOW); // Lock door again
    delay(500);     // Give time for servo to move
  } else {
    Serial.println("Access Denied");
    tone(buzzer, 300);
    delay(DENIED_DELAY);
    noTone(buzzer);
  }

  mfrc522.PICC_HaltA(); // Stop reading card
  mfrc522.PCD_StopCrypto1();
}
