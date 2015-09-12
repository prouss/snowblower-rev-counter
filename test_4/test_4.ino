#include "TimerOne.h"

int defaultRPM = 2000;

void setupTimer(unsigned long timeInUs)
{
  Timer1.initialize(timeInUs);       // Initialize timer1, and set a timeInUs micro second period.
  Timer1.attachInterrupt(timerISR);  // Attaches timerISR() as a timer overflow interrupt.
}

void setRPM(int RPM)
{
  unsigned long timeInUs = 60000000 / RPM;
  setupTimer(timeInUs);
  Serial.println(timeInUs);
}

void timerISR()
{
  sei();
  digitalWrite(3, HIGH);
  delay(1); // Stay high for 1 ms.
  digitalWrite(3, LOW);
}

void setup()
{
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  setRPM(defaultRPM);
}

void loop()
{
  // Read a new RPM value.
  String line = Serial.readStringUntil('\n');
  int RPM = line.toInt();
  
  if (line != "") {
   if (RPM == 0) {
    Serial.println("error");
   } else {
    Serial.print("setting RPM to ");
    Serial.println(RPM);
    
    // Set timer to generate pulses corresponding to the given RPM.
    setRPM(RPM);
   }
  }
}
