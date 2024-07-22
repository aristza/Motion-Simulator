#include <AFMotor.h>
#include <SoftwareSerial.h>

#define THRESHOLD 30
#define MINSPEED 140
#define MAXSPEED 255

#define LEFT 1
#define RIGHT 0

// Motors position on AFMotor Shield
AF_DCMotor motorR(1);
AF_DCMotor motorL(4);
AF_DCMotor motors[2] = {motorR, motorL};

// Potentiometer pins
int POT[2] = {A0, A1};

// Desired position (Destination)
int DEST[2] = {128, 128};
// Actual position of motor
int POS[2] = {128, 128};

SoftwareSerial bluetooth(A4, A5);

void setup() {
  Serial.begin(115200);
  bluetooth.begin(9600);
  motorR.run(RELEASE);
  motorL.run(RELEASE);
  delay(100);
}

void loop() {
  if (Serial.available() > 0) setDest();
  getPos();
  setMotor(LEFT);
  setMotor(RIGHT);
  delay(20);
}

// Read desired position from Serial
void setDest(){
  DEST[LEFT] = (Serial.readStringUntil('A')).toInt();
  DEST[RIGHT] = (Serial.readStringUntil('B')).toInt();
  Serial.println("RECEIVED");
}

// Read the position of the potentiometers/motors
int getPos() {
  POS[RIGHT] = map(analogRead(POT[RIGHT]), 0, 1023, 0, 255);
  POS[LEFT] = map(analogRead(POT[LEFT]), 0, 1023, 0, 255);
  Serial.println((String)POS[0] + " " + POS[1]);
}

// Calculate the speed of motor based on its distance from the desired position
int getSpd(int motor){
  int dist = abs(DEST[motor] - POS[motor]);

  if (dist <= 80) return MINSPEED;
  else if (dist >= 200) return MAXSPEED;
  else return dist;
}

// Calculate the direction of rotation
int getDir(int motor){
  int dir = POS[motor] - DEST[motor];

  // RELEASE if close to DEST
  if(abs(dir) <= THRESHOLD) return RELEASE;
  
  // Otherwise return direction
  if (dir >= 0) return FORWARD;
  else return BACKWARD;
}

void setMotor(int motor){
  motors[motor].run(getDir(motor));
  motors[motor].setSpeed(getSpd(motor));
}
