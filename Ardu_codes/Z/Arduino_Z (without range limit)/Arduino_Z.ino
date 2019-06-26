#define DISTANCE 1

int StepCounter = 0;
int Stepping = false;

// declare flag pin to send digital output to the RASPBERRY PI
int signal_pin = 12;

 
void setup() 
  {
  pinMode(signal_pin, OUTPUT);  
  
  pinMode(8, OUTPUT);  // to cntrl direction
  pinMode(9, OUTPUT);  // to step one step 
  
  digitalWrite(8, LOW);     // no-step
  digitalWrite(9, LOW);     //  direction

  pinMode(2, INPUT);  // physical button step-up
  pinMode(4, INPUT);  // GUI button step-up
  
  
  pinMode(3, INPUT);  // physical button step-down
  pinMode(5, INPUT);   // GUI button step-down
  digitalWrite(signal_pin, LOW);
}

void loop() 
  { 
  if ((digitalRead(2) == LOW || digitalRead(4) == HIGH) && Stepping == false)
    {
    digitalWrite(8, LOW);
    Stepping = true;
    digitalWrite(signal_pin,HIGH);  
    }
    
  
  if ((digitalRead(3) == LOW || digitalRead(5) == HIGH) && Stepping == false)
    {
     digitalWrite(8, HIGH);
      Stepping = true;
      digitalWrite(signal_pin,HIGH); 
    }


  if (Stepping == true)
    {
    digitalWrite(9, HIGH);
    delay(1);
    digitalWrite(9, LOW);
    delay(1);
    StepCounter = StepCounter + 1;
    if (StepCounter == DISTANCE)
       {
        StepCounter = 0;
        Stepping = false;
        digitalWrite(signal_pin,LOW); 
        }
    }
  }
