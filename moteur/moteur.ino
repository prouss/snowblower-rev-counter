#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(3,INPUT);
}

int RPM()
{
  unsigned long gap = pulseIn(3,LOW) + pulseIn(3,HIGH);
  int rpm = 0;
  if (gap != 0)
  {
    rpm = 60000000/gap;
  }
  return rpm;
}

void loop()
{
  int rpm = RPM();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(rpm);
  delay(500);
}
