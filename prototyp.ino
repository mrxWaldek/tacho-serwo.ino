volatile unsigned int ints;
int pot = 0;    
byte blade_count=4;   //liczba czarnych pól
unsigned int rpm;
unsigned long timeold;

void setup()
{
  Serial.begin(9600);
  pinMode(2,INPUT);  // czujnik
  digitalWrite(2,HIGH);
  attachInterrupt(0, tachoInt, FALLING);
  pinMode(8, OUTPUT); //silnik
  pinMode(9, OUTPUT); //silnik
  ints = 0;
  rpm = 0;
  timeold = 0;
  Serial.println("tachometr");
}

void loop()
{
  
  if (timeold+1000<millis())
  {//obrotomierz
    rpm = (((float)ints/blade_count)/((millis() - timeold)/1000))*60;
    Serial.print(rpm);Serial.println("rpm");
    ints = 0;
    timeold = millis();
    digitalWrite(8, LOW);//silnik otwiera zawor
    digitalWrite(9, HIGH);
    pot = analogRead(A5); //potencjometr sprzeżony z serwem
  }

  switch(rpm){
    
    case 900:
      if (pot >=325)
    {
      digitalWrite(8, LOW);
      digitalWrite(9, LOW); 
    }
      break;
    case 1500:
      if (pot >=660)
    {
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);  
    }
      break;
     default:   //zamkniecie zaworu 
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
  
  }
}

void tachoInt()
{
  PORTB^=_BV(5);
  ints++;
}
