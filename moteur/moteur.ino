#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Number of pulses to use while computing the RPM.
#define PULSE_COUNT   16

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Count of pulses from the engine.
static uint32_t pulseCount = 0;

// Current RPM value computed with the elapsed time of the last 16 pulses. 
static uint16_t rpm = 0;

// Get the count of pulses from the engine.
uint32_t getPulseCount()
{
  // Disable interrupts while we read the count value. This way we will be sure that the ISR will not change it while we are reading it.
  cli();
  uint32_t count = pulseCount;
  sei();
  return count;
}

// Get the current RPM value.
uint16_t getRPM()
{
  // Disable interrupts while we read the RPM value. This way we will be sure that the ISR will not change it while we are reading it.
  cli();
  uint16_t value = rpm;
  sei();
  return value;
}

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  // We count engine pulse on pin 8.
  pinMode(8, INPUT);

  // Allow pin change interrupts for PB0 (digital pin 8).  
  PCMSK0 = (1 << PINB0);

  // Enable pin change interrupt 0.
  PCICR = (1 << PCIE0);
}

// Interrupt service routine for the interrupt 0.
// This function is called every time the state of pin 8 changes.
ISR(PCINT0_vect)
{
  static unsigned long previousMicros[PULSE_COUNT] = {0};   // Time in microseconds for the last PULSE_COUNT pulses.
  static uint8_t index = 0;                                 // Current index in previousMicros.
  static uint8_t previousState = 0;                         // Previous state of the pin 8.
  uint8_t state = (PINB >> PINB0) & 0x01;                   // Current state of the pin 8.
  
  if (state != previousState) {
    // Count only changes from 0 to 1.
    if (state != 0) {
      ++pulseCount;

      // Increment the index in our array of previous times in microseconds.
      if (++index >= PULSE_COUNT) {
        index = 0;      
      }

      // Compute the elapsed time in microseconds for the last PULSE_COUNT pulses.
      unsigned long currentMicros = micros();
      unsigned long elapsedMicros = currentMicros - previousMicros[index];
      previousMicros[index] = currentMicros;

      // Compute the RPM using the elapsed time in microseconds for the last PULSE_COUNT pulses.
      rpm = (uint16_t)((60 * 1000000UL * PULSE_COUNT + elapsedMicros / 2) / elapsedMicros);
    }
    previousState = state;
  }
}

void loop()
{
  uint16_t rpm = getRPM();
  unsigned long pulseCount = getPulseCount();
  
  Serial.print(rpm);
  Serial.print(" ");
  Serial.println(pulseCount);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(rpm);
  lcd.setCursor(0,1);
  lcd.print(pulseCount);
  
  delay(500);
}
