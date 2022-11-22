
  
#include <Servo.h>
 
// Define the number of servos
#define SERVOS 5
#define SIDE_POS 100
 
// Create the servo object to control a servo.
Servo myservo[SERVOS];  
 
// Attach servo to digital pin on the Arduino
int servo_pins[SERVOS] = {2, 3, 4, 5, 6}; 
// 
//// Default angle for the servo in degrees
//int default_pos[SERVOS] = {0, 0, 0, 0, 0}; 

int offset[SERVOS] = {-3,0,2,5,-8};
 
void setup() {
   
  for(int i = 0; i < SERVOS; i++) {
     
    // Attach the servo to the servo object 
    myservo[i].attach(servo_pins[i]);
   
    // Make all the servos go to the default position
    myservo[i].write(0);
  }
   
  // Wait 15 milliseconds for the servo to get to the position
  delay(12);   
}
  
void loop() {
  setAngle(120);
}      

void setAngle(int angle) {
   for(int i = 0; i < SERVOS; i++) {
      myservo[i].write(angle + offset[i]);     
    }           
}

void sweep() {
    
  // Go from 0 degrees to 180 degrees
  // Move in steps of 1 degree
  for (int angle = 0; angle <= 180; angle += 1) { 
 
    // Update the angle for all servos
    for(int i = 0; i < SERVOS; i++) {
   
      // Tell servo to go to the position in variable 'angle'
      // where 'angle' is in degrees
      myservo[i].write(angle);  
 
      // Wait 15 milliseconds for the servo to get to the position
      delay(1);    
    }                   
  }
 
  // Go from 180 degrees to 0 degrees
  // Move in steps of 1 degree   
  for (int angle = 180; angle >= 0; angle -= 1) { 
 
    // Update the angle for all servos
    for(int i = 0; i < SERVOS; i++) {
   
      // Tell servo to go to the position in variable 'angle'
      // where 'angle' is in degrees
      myservo[i].write(angle);  
 
      // Wait 15 milliseconds for the servo to get to the position
      delay(12);    
     }
  }
}
