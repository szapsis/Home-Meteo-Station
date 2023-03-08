#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bme;

#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

#include <MHZ19_uart.h>
int rxPin = 2;
int txPin = 3;
MHZ19_uart mhz19;

void setup() {
  Serial.begin(9600);
  mhz19.begin(rxPin, txPin);
  mhz19.setAutoCalibration(false);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Home Meteo Station");
  lcd.setCursor(0, 2);
  lcd.print("T= ");
  lcd.setCursor(0, 3);
  lcd.print("HR= ");
  lcd.setCursor(8, 2);
  lcd.print("Pat= ");
  lcd.setCursor(9, 3);
  lcd.print("CO2= ");

  if (!bme.begin(0x76)) Serial.println("Error!");
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    //Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

}
void loop() {
  DateTime now = rtc.now();

  static uint32_t tmr;
  if (millis() - tmr >= 5000) {
    tmr = millis();

    if (now.hour() < 10) {
      lcd.setCursor(0, 1);
      lcd.print("0");
      lcd.setCursor(1, 1);
      lcd.print(now.hour(), DEC);
    } else {
      lcd.setCursor(0, 1);
      lcd.print(now.hour(), DEC);
    }

    lcd.setCursor(2, 1);
    lcd.print(":");

    if (now.minute() < 10) {
      lcd.setCursor(3, 1);
      lcd.print("0");
      lcd.setCursor(4, 1);
      lcd.print(now.minute(), DEC);
    } else {
      lcd.setCursor(3, 1);
      lcd.print(now.minute(), DEC);
    }

    if (now.day() < 10) {
      lcd.setCursor(6, 1);
      lcd.print("0");
      lcd.setCursor(7, 1);
      lcd.print(now.day(), DEC);
    } else {
      lcd.setCursor(6, 1);
      lcd.print(now.day(), DEC);
    }
    lcd.setCursor(8, 1);
    lcd.print("/");

    if (now.month() < 10) {
      lcd.setCursor(9, 1);
      lcd.print("0");
      lcd.setCursor(10, 1);
      lcd.print(now.month(), DEC);
    } else {
      lcd.setCursor(9, 1);
      lcd.print(now.month(), DEC);
    }

    lcd.setCursor(11, 1);
    lcd.print("/");

    lcd.setCursor(12, 1);
    lcd.print(now.year());

    lcd.setCursor(17, 1);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);

    lcd.setCursor(2, 2);
    lcd.print(bme.readTemperature());
    lcd.setCursor(6, 2);
    lcd.write(223);


    if (round(bme.readPressure() / 100.0F) < 1000) {
      lcd.setCursor(13, 2);
      lcd.print(round(bme.readPressure() / 100.0F));
    } else {
      lcd.setCursor(12, 2);
      lcd.print(round(bme.readPressure() / 100.0F));
    }
    lcd.setCursor(16, 2);
    lcd.print("mBar");

    lcd.setCursor(3, 3);
    lcd.print(bme.readHumidity());
    lcd.setCursor(7, 3);
    lcd.write(37);

    if (mhz19.getPPM() < 1000) {
      lcd.setCursor(14, 3);
      lcd.print(mhz19.getPPM());
    } else {
      lcd.setCursor(13, 3);
      lcd.print(mhz19.getPPM());
    }
    lcd.setCursor(17, 3);
    lcd.print("ppm");

  }

}
