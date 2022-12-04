#include <MIDI.h>
#include <Servo.h> 
 
#define RELAY_PIN 52
#define MOTOR_PIN 45
#define MOTOR_REVERSE_PIN 44
#define MOTOR_TRIM_POT_PIN A1

#define SERVOS 30
#define DEFAULT_POS 75

#define ON_TONE_ANGLE 40

#define DEFAULT_OFFSET 20

#define TONE_HOLD 100
#define DETACH_HOLD 250
 
#define UPPER_PITCH 90
#define LOWER_PITCH 60 
 
//Create an instance of the library with default name, serial port and settings
MIDI_CREATE_DEFAULT_INSTANCE();

struct MySettings : public midi::DefaultSettings {
  static const long BaudRate = 115200;
};

int motor_trim_pot = 0;
int motor_setting = 0;


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
  3,  4,  7, -8, 0,
  7,  7,  10,  -6,  10,
  13, 4, 14, 3, 6,

  // Separation

  -14, -8, 12, -12, -4,
  -12, -2, -2, -4, -4, 
  -10, 2, 0, -2, 6 
};

bool activeServos[SERVOS] = {false};

// At 0 the servo should be detached, 50 we should return to default
long servoActivationTimer[SERVOS] = {0};


void setup() {
  pinMode(RELAY_PIN, OUTPUT);    


  MIDI.begin(MIDI_CHANNEL_OMNI); 
    
  // OMNI sets it to listen to all channels.. MIDI.begin(2) would set it 
  // to respond to notes on channel 2 only.

  // This command tells the Midi Library which function you want to call when a NOTE ON command
  // is received. In this case it's "MyHandleNoteOn".
  MIDI.setHandleNoteOn(MyHandleNoteOn); 

  setAllDefault();
  delay(500);
  for(int i = 0; i < SERVOS; i++) {
    // Attach the servo to the servo object 
    myservo[i].attach(servo_pins[i]);
    // wait for servo to get to position before moving on to next 
  }
  delay(500);
  
  for(int i = 0; i < SERVOS; i++) {
    myservo[i].detach();
  }
  delay(500);
}
  
void loop() {
  motor_trim_pot = analogRead(MOTOR_TRIM_POT_PIN);
  
  // map it to the range of the analog out:
  motor_setting = map(motor_trim_pot, 0, 1023, 0, 255);
  
  digitalWrite(MOTOR_REVERSE_PIN, LOW);
  analogWrite(MOTOR_PIN, motor_setting);

   // Handle midi input 
  MIDI.read(); // Continuously check if Midi data has been received.
  updateServoState();
}      

// MyHandleNoteON is the function that will be called by the Midi Library
// when a MIDI NOTE ON message is received.
// It will be passed bytes for Channel, Pitch, and Velocity
void MyHandleNoteOn(byte channel, byte pitch, byte velocity) {

  // C Major, starting at middle C
  // 60, 62, 64, 65, 67, 69, 71, 72

  // CDGA....CDE
  //  44, 74 

  if (pitch > UPPER_PITCH || LOWER_PITCH > pitch) {
    return;
  }

  int pitchPin = map(pitch, LOWER_PITCH, UPPER_PITCH, 2, 31);
  onTone(pitchPin);
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

int getDefault(int index) {
  if (SERVOS/2 <= index) {
    return DEFAULT_POS + (DEFAULT_OFFSET * -1);
  }
  return DEFAULT_POS + DEFAULT_OFFSET;
}

void setAllDefault() {
   for(int i = 0; i < SERVOS; i++) {
      myservo[i].write(getDefault(i) + offset[i]);   
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


void updateServoState(){
  unsigned long currentMillis = millis();
  int timeDiff;
   for(int i = 0; i < SERVOS; i++) {
     
    if(servoActivationTimer[i] == 0) {
      continue;
    }

    timeDiff = currentMillis - servoActivationTimer[i]; 
    if (timeDiff > DETACH_HOLD) { 
      myservo[i].detach();
      servoActivationTimer[i] = 0;
    } else if (timeDiff > TONE_HOLD && activeServos[i]) {
      offTone(i);
      activeServos[i] = false;
    }
  }
}

void onTone(int index) {
  int angle = ON_TONE_ANGLE;
  // If the pin is over half way reverse the direction
  if (SERVOS/2 <= index) {
    angle = angle * -1;
  }
  
  attachAndWrite(index, getDefault(index) + angle + offset[index]);
  servoActivationTimer[index] = millis();
  activeServos[index] = true;
}

void offTone(int index) {
  attachAndWrite(index, getDefault(index) + offset[index]);
}
