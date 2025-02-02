const int fireSensor = A0;
const int smokeSensor = A1;
const int buzzer = 6;  
const int smokeLimit = 300;
const int fireLimit = 250;

void setup() {
  Serial.begin(9600);
  pinMode(fireSensor, INPUT);
  pinMode(smokeSensor, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.println("Fire & Smoke Detection System Started.");
}

void loop() {
  int smokeValue = analogRead(smokeSensor);
  int fireValue = analogRead(fireSensor);

  Serial.print("Smoke Level: ");
  Serial.print(smokeValue);
  Serial.print(" | Fire Level: ");
  Serial.println(fireValue);

  if (fireValue < fireLimit || smokeValue > smokeLimit) {
    digitalWrite(buzzer, HIGH);
    Serial.println("🔥 Alert: Fire or smoke detected!");
  } else {
    digitalWrite(buzzer, LOW);
  }
  delay(1000);
}
