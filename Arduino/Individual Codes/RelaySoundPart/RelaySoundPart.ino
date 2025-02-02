const int soundSensorPin = A2;
const int relayPin = 23;  
const int soundLimit = 75;

void setup() {
  Serial.begin(9600);
  pinMode(soundSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  Serial.println("Sound Detection System Initialized.");
}

void loop() {
  int soundValue = analogRead(soundSensorPin);

  Serial.print("Sound Level: ");
  Serial.println(soundValue);

  if (soundValue > soundLimit) {
    digitalWrite(relayPin, HIGH);
    Serial.println("🔊 Loud sound detected! Relay ON.");
    delay(3000);
    digitalWrite(relayPin, LOW);
    Serial.println("Relay OFF.");
  }
  delay(500);
}
