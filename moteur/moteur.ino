#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
static int counts;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(8,INPUT);
  PCICR = (1 << PCIE0);
  PCMSK0 = (1 << PINB0);
}

ISR(PCINT0_vect)
{
  static uint8_t previousState = 0;
  uint8_t state = (PINB >> PINB0) & 0x01;
  if (state != previousState)
  {
    counts += 1;
    previousState = state;
  }
}

int RPM()
{
  int nb = counts;
  delay (250);
  int nb2 = counts;
  return (nb2 - nb)*2;
}

void loop()
{
  int rpm = RPM();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(rpm);
  lcd.setCursor(0,1);
  lcd.print(counts);

}
