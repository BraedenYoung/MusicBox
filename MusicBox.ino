
#include <Servo.h>
 
#define SERVOS 30
#define DEFAULT_POS 75

#define TONE_HOLD 100
#define DETACH_HOLD 50
 
// Create the servo object to control a servo.
Servo myservo[SERVOS];  
 
// Servos arranged in two rows.
// One row mirroring the other
int servo_pins[SERVOS] = {
   2,  3,  4,  5,  6,
   7,  8,  9, 10, 11,
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

// int activeServos[SERVOS] = {};

// At 0 the servo should be detached, 50 we should return to default
long servoActivationTimer[SERVOS] = {
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,

  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0
};


void setup() {
   
  setAllToAngle(75);
  for(int i = 0; i < SERVOS; i++) {
    // Attach the servo to the servo object 
    myservo[i].attach(servo_pins[i]);
    // wait for servo to get to position before moving on to next 
  }
  delay(1000);
  
  
  for(int i = 0; i < SERVOS; i++) {
    // Attach the servo to the servo object 
    myservo[i].detach();
    // wait for servo to get to position before moving on to next
  }

  delay(500);
}
  
void loop() {
  for(int i = 0; i < SERVOS; i++) {
    delay(500);
    onTone(i);

    delay(500);

    offTone(i);
  }
 
}      

void attachAndWrite(int index, int angle){
    myservo[index].attach(servo_pins[index]);
    myservo[index].write(angle);
}

void detachAll() {
  for(int i = 0; i < SERVOS; i++) {
    myservo[i].detach();
  }
}

void setAllToAngle(int angle) {
   for(int i = 0; i < SERVOS; i++) {
      myservo[i].write(angle + offset[i]);   
    }           
}

int checkServoActivationTimer(){
  for(int i = 0; i < SERVOS; i++) {
    if (servoActivationTimer[i] == 0) {
      continue;
    }

    servoActivationTimer[i] = servoActivationTimer[i] - 1;
    if (servoActivationTimer[i] == 0) {
      offTone(i);
    }    
  }   
}


// void updateServoState(){
  
//   unsigned long currentMillis = millis();

//   activeServos
//   for (int i=0; i<sizeof activeServos/sizeof activeServos[0]; i++) {
//     if (currentMillis - servoActivationTimer[i] >= toneDuration) {

//     }
//   }
// }

void onTone(int index) {
  int angle = 30;
  // If the pin is over half way reverse the direction
  if (SERVOS/2 <= index) {
    angle = angle * -1;
  }
  
  attachAndWrite(index, DEFAULT_POS + angle + offset[index]);
  servoActivationTimer[index] = millis();
}

void offTone(int index) {
  // myservo[index].write(DEFAULT_POS +  offset[index]);   
  
  attachAndWrite(index, DEFAULT_POS + offset[index]);
  delay(50);
  myservo[index].detach();
}
