// Robotics with the BOE Shield - RoamingWithWhiskers
// Go forward.  Back up and turn if whiskers indicate BOE Shield bot bumped
// into something. 

#include <Servo.h>                           // Include servo library

#define NOTE_C4  262
 
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;


static const int step_dur = 50;                 // In milliseconds
static const int honk_dur = 1000 + 1000 + 250;  // In milliseconds


// static const uint8_t rght_whskr_pin = 7;
// static const uint8_t left_whskr_pin = 5;


static const uint8_t rght_servo_pin = 12;
static const uint8_t left_servo_pin = 13;
static const uint8_t speaker_pin    = 9;


static const uint8_t pin_shift_f_l = 8;
static const uint8_t left_btn_pin  = pin_shift_f_l - 1;
static const uint8_t bkwd_btn_pin  = pin_shift_f_l - 2;
static const uint8_t frwd_btn_pin  = pin_shift_f_l - 3;
static const uint8_t rght_btn_pin  = pin_shift_f_l - 4;
static const uint8_t honk_btn_pin  = pin_shift_f_l - 5;


void honk(unsigned int n = 2);

 
void setup()                                 // Built-in initialization block
{
  Serial.begin(9600); 

  // Set up input from receiver:
  pinMode(left_btn_pin, INPUT_PULLUP);
  pinMode(bkwd_btn_pin, INPUT_PULLUP);
  pinMode(frwd_btn_pin, INPUT_PULLUP);
  pinMode(rght_btn_pin, INPUT_PULLUP);
  pinMode(honk_btn_pin, INPUT_PULLUP);

  servoLeft.attach(left_servo_pin);          // Attach left signal to pin 13 
  servoRight.attach(rght_servo_pin);         // Attach right signal to pin 12

  // Set up speaker
  pinMode(speaker_pin,  OUTPUT);
  noTone(speaker_pin);
  honk(1);
}  
 
void loop()                                  // Main loop auto-repeats
{
  // byte wLeft = digitalRead(5);               // Copy left result to wLeft  
  // byte wRight = digitalRead(7);              // Copy right result to wRight

  if        (digitalRead(honk_btn_pin) == LOW) {
    halt();
    honk(2);
    // delay(step_dur);
  } else if (digitalRead(frwd_btn_pin) == LOW) {
    while (digitalRead(frwd_btn_pin) == LOW) forward(step_dur);
    halt();
  } else if (digitalRead(bkwd_btn_pin) == LOW) {
    while (digitalRead(bkwd_btn_pin) == LOW) backward(step_dur);
    halt();
  } else if (digitalRead(left_btn_pin) == LOW) {
    while (digitalRead(left_btn_pin) == LOW) turnLeft(step_dur);
    halt();
  } else if (digitalRead(rght_btn_pin) == LOW) {
    while (digitalRead(rght_btn_pin) == LOW) turnRight(step_dur);
    halt();
  }

  // if((wLeft == 0) && (wRight == 0))          // If both whiskers contact
  // {
  //   backward(1000);                          // Back up 1 second
  //   turnLeft(800);                           // Turn left about 120 degrees
  // }
  // else if(wLeft == 0)                        // If only left whisker contact
  // {
  //   backward(1000);                          // Back up 1 second
  //   turnRight(400);                          // Turn right about 60 degrees
  // }
  // else if(wRight == 0)                       // If only right whisker contact
  // {
  //   backward(1000);                          // Back up 1 second
  //   turnLeft(400);                           // Turn left about 60 degrees
  // }
  // else                                       // Otherwise, no whisker contact
  // {
  //   forward(20);                             // Forward 1/50 of a second
  // }
}

void halt()                                  // Halt function
{
  servoLeft.writeMicroseconds(1500);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1500);        // Right wheel clockwise
}

void forward(int time)                       // Forward function
{
  servoLeft.writeMicroseconds(1700);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(time);                               // Maneuver for time ms
}

void turnLeft(int time)                      // Left turn function
{
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(time);                               // Maneuver for time ms
}

void turnRight(int time)                     // Right turn function
{
  servoLeft.writeMicroseconds(1700);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
  delay(time);                               // Maneuver for time ms
}

void backward(int time)                      // Backward function
{
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
  delay(time);                               // Maneuver for time ms
}

void honk(unsigned int n) {
  for (int i = 0; i < n; i++) {
    tone(speaker_pin, NOTE_C4, 1000);        // Play tone for 1 second
    delay(1000);                             // Delay to finish tone
    noTone(speaker_pin);
    if (i+1 < n) delay(250);                 // Only delay between honks
  }
}
