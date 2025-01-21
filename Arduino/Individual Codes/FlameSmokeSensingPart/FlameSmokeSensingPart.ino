#include <SPI.h>
#include <MFRC522.h>

const int buzzer = 6;  
const int Relay = 44; // Door relay pin for RFID system
const int SS_PIN = 53; // RFID SS pin
const int RST_PIN = 49; // RFID reset pin
const int ACCESS_DELAY = 2000; // Delay for authorized access
const int DENIED_DELAY = 1000; // Delay for access denied

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
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
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  cardUID.toUpperCase();
  Serial.println();

  if (cardUID == "ABCD1234") { // Replace with your card's UID
    Serial.println("Access Granted");
    digitalWrite(Relay, HIGH); // Unlock door
    delay(ACCESS_DELAY);
    digitalWrite(Relay, LOW); // Lock door again
  } else {
    Serial.println("Access Denied");
    tone(buzzer, 300);
    delay(DENIED_DELAY);
    noTone(buzzer);
  }
}
