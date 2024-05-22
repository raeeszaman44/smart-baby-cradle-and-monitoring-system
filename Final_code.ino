#include <Servo.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleStream.h>
#include <DHT.h>

#define BLYNK_TEMPLATE_ID "TMPL68NZqdz_L"
#define BLYNK_TEMPLATE_NAME "LED"
#define BLYNK_AUTH_TOKEN "coGs4EVJ7hA27DC4PA45SyySAVXbKOQk"
#define BLYNK_PRINT SwSerial

SoftwareSerial SwSerial(10, 11);

const int rainSensorPin = 4;
const int micSensorPin = A0;
const int buzzerPin = 6;
const int servoPin = 9;
Servo myServo;
bool isRainDetected = false;
unsigned long startTime = 0;
const unsigned long alarmDuration = 10000;

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    SwSerial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V0, t);
}

void setup()
{
  pinMode(rainSensorPin, INPUT);
  pinMode(micSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(servoPin, OUTPUT);
  myServo.attach(servoPin);

  SwSerial.begin(115200);
  Serial.begin(9600);
  Blynk.begin(Serial, BLYNK_AUTH_TOKEN);

  dht.begin();

  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  int rainValue = digitalRead(rainSensorPin);
  int micValue = analogRead(micSensorPin);

  if (rainValue == LOW)
  {
    if (!isRainDetected)
    {
      isRainDetected = true;
      startTime = millis();
    }
  }
  else
  {
    isRainDetected = false;
  }

  if (isRainDetected && (millis() - startTime <= alarmDuration))
  {
    digitalWrite(buzzerPin, HIGH);
    Blynk.logEvent("urinate");
  }
  else
  {
    digitalWrite(buzzerPin, LOW);
  }

  if (micValue > 490)
  {
    myServo.write(90);
    delay(1000);
    myServo.write(0);
    Blynk.logEvent("crying");
  }

  delay(100);

  Blynk.run();
  timer.run();
}
