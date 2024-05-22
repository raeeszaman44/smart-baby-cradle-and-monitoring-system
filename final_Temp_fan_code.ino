const int lm35Pin = A2;
const int motorPin = 13; 

void setup() {
  Serial.begin(9600); 
  pinMode(motorPin, OUTPUT); 
}

void loop() {
  int sensorValue = analogRead(lm35Pin); 
  float temperature = (sensorValue * 5.0) / 1023.0;  
  temperature = temperature * 100.0;
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  if (temperature > 25) {
    digitalWrite(motorPin, HIGH); 
  } else {
    digitalWrite(motorPin, LOW);
  }
  delay(2000);
}
