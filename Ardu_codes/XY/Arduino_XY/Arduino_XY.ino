#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

/* Declare pins for digital input from the RASPBERRY PI */
const int x_up_pin = 2;
const int x_down_pin = 3 ;
const int y_up_pin = 4;
const int y_down_pin = 5 ;

/* Declare pins for servo output */
const int servo_x_Pin = 9; 
const int servo_y_Pin = 10;

/* Analog input pins for Joystick XY values */
const int JoyStick_X = 0; // pin number for x
const int JoyStick_Y = 1; // pin number for y

/* Declare flag pin to send digital output to the RASPBERRY PI */
const int signal_pin = 12;

/* Declare 2 servo objects to control de degrees of freedom of the XY Stage */
Servo servo_x;
Servo servo_y;  

/* Set the LCD address to 0x27 for a 16 chars and 2 lines display */
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* Set up the default position of the servos: in the middle */
int servo_x_Angle = 90; 
int servo_y_Angle = 90;

/* Actuation flag: not moving */
bool actuating = false;
int moving_x = 0; 
int moving_y = 0;  

/* Joystick sensitivity */
const int threshold = 64;

 
void setup() 
{
  /* Communicate to the RASPBERRY PI if the Arduino is
  actuating (the stage is moving) or listening */
  pinMode(signal_pin, OUTPUT);   // to send 1 bit message to pi
  digitalWrite(signal_pin, LOW); // not actuating

  /* Prepare the servos */
  Serial.begin(9600); // 9600 bps
  servo_x.attach(servo_x_Pin);
  servo_y.attach(servo_y_Pin);

  /* Prepare the LCD */
  lcd.begin();
  /* Turn the blacklight on and print a message */
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("HomeScope");
  lcd.setCursor(0,1);
  lcd.print("  by Biotexturas");delay(5000);

  /* Position at the center of the scanning area 
  (x,y) = (90,90) */
  servo_x.write(servo_x_Angle); 
  servo_y.write(servo_y_Angle);  

  /* Set up the RASPBERRY PI communication pins (4 bits) */
  pinMode(x_up_pin, INPUT);      // GUI button x.step-up
  pinMode(x_down_pin, INPUT);    // GUI button x.step-down
  pinMode(y_up_pin, INPUT);      // GUI button y.step-up
  pinMode(y_down_pin, INPUT);    // GUI button y.step-down
         
  delay(50); 
}


void loop() 
{ 
  /* X movement (signal detection) */
  /* X.UP */
  if ((analogRead(JoyStick_X) > (512 + threshold) && actuating == false) 
      || (digitalRead(x_up_pin) == HIGH && actuating == false)) 
        {
        if(servo_x_Angle < 180) servo_x_Angle++;
         actuating = true;  
        }        
  /* X.DOWN */
  if ((analogRead(JoyStick_X) < (512 - threshold) && actuating==false) 
      || (digitalRead(x_down_pin) == HIGH && actuating == false)) 
        {
         if(servo_x_Angle > 0)servo_x_Angle--;
         actuating = true;
        }
  /* Y movement (signal detection) */
  /* Y.UP */
  if ((analogRead(JoyStick_Y) > (512 + threshold) && actuating==false) 
      || (digitalRead(y_up_pin) == HIGH && actuating==false)) 
        {
         if (servo_y_Angle < 180 ) {servo_y_Angle++;}
         actuating= true;  
        }
  /* Y.DOWN */
  if ((analogRead(JoyStick_Y) < (512 - threshold) &&  actuating==false) 
      || (digitalRead(y_down_pin) == HIGH  &&  actuating==false))  
        {
         if (servo_y_Angle > 0) {servo_y_Angle--;}
         actuating = true;
        }
        
  /* Move */
  if (actuating == true)
        {  
         digitalWrite(signal_pin, HIGH);     //  Actuating          
         servo_x.write(servo_x_Angle); 
         servo_y.write(servo_y_Angle);        
         delay(500);
         actuating = false;
         digitalWrite(signal_pin, LOW);     // Not actuating
        }
  /* Display position info */
  lcd.setCursor(0,1);
  lcd.print("position:"); 
  lcd.print(servo_x_Angle); 
  lcd.print(",");
  lcd.print(servo_y_Angle);
  lcd.print("    ");      
  delay(50);
}
