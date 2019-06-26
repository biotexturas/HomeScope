#define DISTANCE 1  // Define the discrete length of a step

int StepCounter = 0;
bool Stepping = false;

/* Declare flag pin to send digital output to the RASPBERRY PI */
const int signal_pin = 12;

const int top_range_pin = 6;
const int bottom_range_pin = 7;

/* Easy Driver pins */
const int driver_dir = 8;
const int driver_stp = 9;

/* Physical buttons */
const int button_up = 2;
const int button_down = 3;

/* GUI buttons */
const int gui_up = 4;
const int gui_down = 5;

void setup() 
  {

  /* Report state of automata: listening or stepping? */
  pinMode(signal_pin, OUTPUT);  
  digitalWrite(signal_pin, LOW);
  
  /* Set range pins as inputs to read range sensors. 
  Stage will move only if sensors are high */
  pinMode(top_range_pin, INPUT);
  pinMode(bottom_range_pin, INPUT);
  
  /* Pull up both UP and DOWN range pins */
  digitalWrite(top_range_pin, HIGH);
  digitalWrite(bottom_range_pin, HIGH);
  
  /* Easy Driver
  To control direction. Set as LOW for UP direction and HIGH for DOWN direction */
  pinMode(driver_dir, OUTPUT);
  /* To step one step */
  pinMode(driver_stp, OUTPUT); 
  digitalWrite(driver_dir, LOW);     // Starting direction is UP
  digitalWrite(driver_stp, LOW);     //  No-step

  /* Up */
  pinMode(button_up, INPUT);         // physical button step-up
  pinMode(gui_up, INPUT);            // GUI button step-up
  
  /* Down */
  pinMode(button_down, INPUT);       // physical button step-down
  pinMode(gui_down, INPUT);          // GUI button step-down
    
  }

void loop() 
  {
  /* Read range sensors */
  bool value_top = digitalRead(top_range_pin);
  bool value_bottom = digitalRead(bottom_range_pin);

  if (!value_top || !value_bottom) 
    {
    Serial.println("Stage out of range");
    }

  /* Check up direction with */
  if ((digitalRead(button_up) == LOW || digitalRead(gui_up) == HIGH) 
      && (Stepping == false && value_top))
    {
    digitalWrite(driver_dir, LOW);
    Stepping = true;
    digitalWrite(signal_pin, HIGH);  
    }
    
  
  /* Check down direction with */
  if ((digitalRead(button_down) == LOW || digitalRead(gui_down) == HIGH) 
      && (Stepping == false && value_bottom))
    {
     digitalWrite(driver_dir, HIGH);
     Stepping = true;
     digitalWrite(signal_pin, HIGH); 
    }
  
  /* If wished, go ahead and step */
  if (Stepping == true)
    {
    digitalWrite(driver_stp, HIGH);
    delay(1);
    digitalWrite(driver_stp, LOW);
    delay(1);
    StepCounter += 1;
    if (StepCounter == DISTANCE)
       {
        StepCounter = 0;
        Stepping = false;
        digitalWrite(signal_pin, LOW); 
        }
    }
  }
