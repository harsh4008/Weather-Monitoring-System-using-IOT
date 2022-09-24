#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


LiquidCrystal_I2C lcd(0x3F,16,2);


char auth[] = "nPEajIXroKZQ31aCMEao5K32VZTem7wE";
char ssid[] = "OnePlus 7 Pro";
char pass[] = "jashshahh";

DHT dht(D3, DHT11);
BlynkTimer timer;

#define rain A0
#define light D0



void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(light, INPUT);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Weather Monitor");
  lcd.setCursor(4, 1);
  lcd.print("System");
  delay(4000);
  lcd.clear();

  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L, rainSensor);
  timer.setInterval(100L, LDRsensor);
}

void DHT11sensor() 
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);

}

void rainSensor() 
{
  int value = analogRead(rain);
  value = map(value, 0, 1024, 0, 100);
  Blynk.virtualWrite(V2, value);

  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(value);
  lcd.print(" ");
}




void LDRsensor() 
{
  bool value = digitalRead(light);
  if (value == 0) 
  {
    WidgetLED LED(V3);
    LED.on();
  } 
  else 
  {
    WidgetLED LED(V3);
    LED.off();
  }

}

void loop() 
{
  Blynk.run();
  timer.run();
}