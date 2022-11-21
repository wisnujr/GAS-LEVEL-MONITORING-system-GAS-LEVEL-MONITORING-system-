#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h> // include library
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "wVkwRQu8-_IMn-WtLtLXHnJ-NISyLkjW";// token
char ssid[] = "srituhobby";//WIFI USER
char pass[] = "1234";//Kata Sandi Wifi
BlynkTimer timer;
int pinValue = 0;

#define Buzzer D5
#define Green D6
#define Red D7
#define Sensor A0

void setup() {
  Serial.begin(9600);
  lcd.backlight();
  lcd.init();
  pinMode(Green, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Sensor, INPUT);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, notifiaction);
}
BLYNK_WRITE(V0) {
  pinValue = param.asInt();
}

void notifiaction() {
  int sensor = analogRead(Sensor);
  Serial.println(sensor);
  sensor = map(sensor, 0, 1024, 0, 100);
  if (pinValue == 1) {
    if (sensor <= 50) {
      digitalWrite(Green, HIGH);
      digitalWrite(Red, LOW);
      digitalWrite(Buzzer, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Gas value:Normal");
    } else if (sensor > 50) {
      Blynk.notify("Warning! Gas leak detected");
      digitalWrite(Green, LOW);
      digitalWrite(Red, HIGH);
      digitalWrite(Buzzer, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Gas value:High  ");
    }
    lcd.setCursor(0, 0);
    lcd.print("Value : ");
    lcd.print(sensor);
    Blynk.virtualWrite(V1, sensor);
  } else {
    digitalWrite(Red, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Green, LOW);
    lcd.clear();
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
