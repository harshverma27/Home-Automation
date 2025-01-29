// Fire and Smoke Detection
const int buzzer = 6;  
const int smokeSensor = A0;
const int fireSensor = 9;

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(fireSensor, INPUT);
  pinMode(smokeSensor, INPUT);
  noTone(buzzer);
}

void loop() {
  detectFireAndSmoke();
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
