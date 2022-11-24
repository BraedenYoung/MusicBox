
#include <Servo.h>
 
#define SERVOS 30
#define DEFAULT_POS 75
 
// Create the servo object to control a servo.
Servo myservo[SERVOS];  
 
// Servos arranged in two rows.
// One row mirroring the other
int servo_pins[SERVOS] = {
  2, 3, 4, 5, 6,
  7, 8, 9, 10, 11,
  12, 13, 14, 15, 16,

  // Separation

  17, 18, 19, 20, 21,
  22, 23, 24, 25, 26,
  27, 28, 29, 30, 31
}; 

// Measured offset based on the spindle position
int offset[SERVOS] = {
  4,  2,  8, -4, -2,
  6,  6,  8,  -8,  8,
  12, 4, 12, 1, 4,

  // Separation

  -14, -8, 12, -12, -4,
  -12, -2, -2, -4, -4, 
  -10, 2, -2, -2, 6 
};
 
void setup() {
   
  for(int i = 0; i < SERVOS; i++) {
    // Attach the servo to the servo object 
    myservo[i].attach(servo_pins[i]);
    myservo[i].write(DEFAULT_POS);  

    // wait for servo to get to position before moving on to next
    delay(100);
  }
}
  
void loop() {
  setAngle(75);
}      

void setAngle(int angle) {
   for(int i = 0; i < SERVOS; i++) {
      myservo[i].write(angle + offset[i]);   
      delay(100);     
    }           
}

int getAngleForPin(int pin, int angle, int change) {

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
