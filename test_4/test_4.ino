void setup()
{
  pinMode(3,OUTPUT);
  Serial.begin(9600);
}

int rpm = 2000;
unsigned long gap = 60000/rpm;
  
void loop()
{
  digitalWrite(3,LOW);
  delay(gap);
  digitalWrite(3,HIGH);
  delay(1);
}
