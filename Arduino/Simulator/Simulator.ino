int MR[] = {8, 9};    //Right motor pins
int ML[] = {6, 7};    //Left motor pins
int EN[] = {2, 1};    //enable pins for right and left
int POT[] = {54, 55}; //potentiometer pins for right and left
int POS[] = {10, 10};   //position requested

void setup() {

  pinMode(POT[0], INPUT);
  pinMode(POT[1], INPUT);

  pinMode(MR[0], OUTPUT);
  pinMode(MR[1], OUTPUT);
  pinMode(EN[0], OUTPUT);
  
  pinMode(ML[0], OUTPUT);
  pinMode(ML[1], OUTPUT);
  pinMode(EN[1], OUTPUT);

  Serial.begin(115200);
}

void loop() {

  if (Serial.available() > 0) setPos();
  setMotor(0);
  setMotor(1);

  delay(10);
}

void setDir(int motor[], int dir){
  
  digitalWrite(motor[0], LOW);
  digitalWrite(motor[1], LOW);

  if (dir != -1) digitalWrite(motor[dir], HIGH);
  
}

void setSpd(int motor, int spd){

  analogWrite(EN[motor], spd);
  //analogWrite(EN[motor], 100);
  
}

int getDir(int motor){
  
  int potval = map(analogRead(POT[motor]), 0, 1023, 0, 255);
  int sub = potval - POS[motor];

  if(abs(sub) <= 30) return -1;
  
  if (sub >= 0) return 1;
  else return 0;
  
}

int getSpd(int motor){

  int sub = 0;
    
  int potval = map(analogRead(POT[motor]), 0, 1023, 0, 255);

  sub = abs(POS[motor] - potval);

  if (sub <= 30) return 80;
  else if (sub <= 80) return 80;
  else if (sub <= 140) return 140;
  else if (sub <= 200) return 200;
  else return 200;
  
}

void setMotor(int motor){

  if (motor == 0) setDir(MR, getDir(motor));
  else setDir(ML, getDir(motor));
  
  setSpd(motor, getSpd(motor));
}

void setPos(){
  //String A = Serial.readStringUntil('A');
  //String B = Serial.readStringUntil('B');
  POS[1] = (Serial.readStringUntil('A')).toInt();
  POS[0] = (Serial.readStringUntil('B')).toInt();
}

