volatile unsigned int ints;

byte blade_count = 4;
unsigned int rpm;
unsigned long timeold;
int pot = 0;
int oldRPM = 0;

void setup()
{
  pinMode(8, OUTPUT);   //podlączony silnik dc przez mostek H
  pinMode(9, OUTPUT);   //podlączony silnik dc przez mostek H
  Serial.begin(9600);
  pinMode(2,INPUT);   //czujnik odbiciowy podlączony przez komparator
  digitalWrite(2,HIGH);
  attachInterrupt(0, tachoInt, FALLING);

  ints = 0;
  rpm = 0;
  timeold = 0;
  Serial.println("tachometr");  
}

void loop(){
  if (timeold+1000<millis()){
    rpm = (((float)ints/blade_count)/((millis() - timeold)/1000))*60;
    Serial.print(rpm);Serial.println("rpm");
    ints = 0;
    timeold = millis();   //wyliczenie z impulsów predkosci obrotowej
  }

  if (rpm >= 2000){       // serwo otwiera zawór o dana wartość
    bool Zawor = true;
    digitalWrite(9,HIGH);
    digitalWrite(8,LOW);

    while (Zawor){
      pot = analogRead(A5);

      if (pot >= 400){
        Zawor=false;
        digitalWrite(9,LOW);
        digitalWrite(8,LOW);
      }
    }
  }
  delay(500);

  if (rpm >= 1500){   // serwo otwiera zawór o dana wartość
    bool Zawor = true;
    digitalWrite(9,HIGH);
    digitalWrite(8,LOW);

    while (Zawor){
      pot = analogRead(A5);

      if (pot >= 600){
        Zawor = false;
        digitalWrite(8,LOW);
        digitalWrite(9,LOW);
      }
    }
  }
  delay(500);

  if (abs(rpm-oldRPM) > 200){   //cofanie serwa gdy rpm spada
    bool Zawor = true;
    digitalWrite(8,HIGH);
    digitalWrite(9,LOW);

    while (Zawor){
      pot=analogRead(A5);

      if (pot <= 250){
        Zawor = false;
        digitalWrite(9,LOW);
        digitalWrite(8,LOW);
        oldRPM = rpm;
      }
    }
  }
  delay(500);
}

void tachoInt()
{
  PORTB^=_BV(5);
  ints++;
}
