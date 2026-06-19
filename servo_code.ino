#include <Servo.h>


const uint8_t redPin     = 2;   
const uint8_t greenPin   = 3;   
const uint8_t bluePin    = 4;   

const uint8_t ir1Pin     = 5;   
const uint8_t ir2Pin     = 6;   
const uint8_t ir3Pin     = 7;   


const uint8_t servo1Pin  = 11;  
const uint8_t servo2Pin  = 10;   
const uint8_t servo3Pin  = 9;    

Servo servo1, servo2, servo3;

const uint8_t FAST_DELAY = 1;   
const uint8_t SLOW_DELAY = 18;  


const int CENTER_ANGLE = 90;
const int FULL_ANGLE   = 180;   
const int MIN_ANGLE    = 0;

int servo1Angle = CENTER_ANGLE;
int servo2Angle = CENTER_ANGLE;
int servo3Angle = CENTER_ANGLE;

bool servo1Moved = false; 
bool servo2Moved = false; 
bool servo3Moved = false;

uint8_t lastColor = 0;

bool irWasActive = false;

void smoothMove(Servo &s, int &currentAngle, int toAngle, uint8_t stepDelay) {

  toAngle = constrain(toAngle, MIN_ANGLE, 180);
  currentAngle = constrain(currentAngle, MIN_ANGLE, 180);

  if (currentAngle == toAngle) return;

  if (currentAngle < toAngle) {
    for (int a = currentAngle + 1; a <= toAngle; ++a) {
      s.write(a);
      delay(stepDelay);
      yield();
    }
  } else {
    for (int a = currentAngle - 1; a >= toAngle; --a) {
      s.write(a);
      delay(stepDelay);
      yield();
    }
  }
  currentAngle = toAngle; 
}

void resetAllServos(uint8_t returnDelay) {
  smoothMove(servo1, servo1Angle, CENTER_ANGLE, returnDelay);
  smoothMove(servo2, servo2Angle, CENTER_ANGLE, returnDelay);
  smoothMove(servo3, servo3Angle, CENTER_ANGLE, returnDelay);

  servo1Moved = servo2Moved = servo3Moved = false;
}

void tiltServoForColor(uint8_t color) {
  switch (color) {
    case 1: 
      smoothMove(servo1, servo1Angle, FULL_ANGLE, FAST_DELAY);
      servo1Moved = true;
      break;
    case 2:
      smoothMove(servo2, servo2Angle, FULL_ANGLE, FAST_DELAY);
      servo2Moved = true;
      break;
    case 3:
      smoothMove(servo3, servo3Angle, FULL_ANGLE, FAST_DELAY);
      servo3Moved = true;
      break;
    default:

      break;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(redPin, INPUT);
  pinMode(greenPin, INPUT);
  pinMode(bluePin, INPUT);

  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);
  pinMode(ir3Pin, INPUT);

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);

  // initial positions
  servo1.write(CENTER_ANGLE);
  servo2.write(CENTER_ANGLE);
  servo3.write(CENTER_ANGLE);

  servo1Angle = CENTER_ANGLE;
  servo2Angle = CENTER_ANGLE;
  servo3Angle = CENTER_ANGLE;

  delay(300);
}

void loop() {
  bool redDetected   = (digitalRead(redPin) == HIGH);
  bool greenDetected = (digitalRead(greenPin) == HIGH);
  bool blueDetected  = (digitalRead(bluePin) == HIGH);

  bool ir1 = (digitalRead(ir1Pin) == HIGH);
  bool ir2 = (digitalRead(ir2Pin) == HIGH);
  bool ir3 = (digitalRead(ir3Pin) == HIGH);

  bool anyIr = (ir1 || ir2 || ir3);
  if (anyIr && !irWasActive) {
    Serial.println("Object detected (IR) — resetting all servos to center");
    resetAllServos(FAST_DELAY);
  }
  irWasActive = anyIr; 

  uint8_t currentColor = 0;
  if (redDetected) currentColor = 1;
  else if (greenDetected) currentColor = 2;
  else if (blueDetected) currentColor = 3;

  if (currentColor != lastColor) {
    Serial.print("Color changed from ");
    Serial.print(lastColor);
    Serial.print(" to ");
    Serial.println(currentColor);

    resetAllServos(FAST_DELAY);

    if (currentColor != 0) {
      tiltServoForColor(currentColor);
    }

    lastColor = currentColor;
  }


  if (redDetected) {
    if (ir1) {
      if (!servo1Moved) {
        smoothMove(servo1, servo1Angle, FULL_ANGLE, FAST_DELAY);
        servo1Moved = true;
      }
    } else {
      if (servo1Moved) {
        smoothMove(servo1, servo1Angle, CENTER_ANGLE, SLOW_DELAY);
        servo1Moved = false;
      }
    }

    if (servo2Moved) { smoothMove(servo2, servo2Angle, CENTER_ANGLE, SLOW_DELAY); servo2Moved = false; }
    if (servo3Moved) { smoothMove(servo3, servo3Angle, CENTER_ANGLE, SLOW_DELAY); servo3Moved = false; }
  }
  else if (greenDetected) {
      smoothMove(servo1, servo1Angle, CENTER_ANGLE, FAST_DELAY);
    if (ir2) {
      if (!servo2Moved) {
        smoothMove(servo2, servo2Angle, FULL_ANGLE, FAST_DELAY);
        servo2Moved = true;
      }
    } else {
      if (servo2Moved) {
        smoothMove(servo2, servo2Angle, CENTER_ANGLE, SLOW_DELAY);
        servo2Moved = false;
      }
    }

    if (servo1Moved) { smoothMove(servo1, servo1Angle, CENTER_ANGLE, SLOW_DELAY); servo1Moved = false; }
    if (servo3Moved) { smoothMove(servo3, servo3Angle, CENTER_ANGLE, SLOW_DELAY); servo3Moved = false; }
  }
  else if (blueDetected) {
    if (ir3) {
      if (!servo3Moved) {
        smoothMove(servo3, servo3Angle, FULL_ANGLE, FAST_DELAY);
        servo3Moved = true;
      }
    } else {
      if (servo3Moved) {
        smoothMove(servo3, servo3Angle, CENTER_ANGLE, SLOW_DELAY);
        servo3Moved = false;
      }
    }

    if (servo1Moved) { smoothMove(servo1, servo1Angle, CENTER_ANGLE, SLOW_DELAY); servo1Moved = false; }
    if (servo2Moved) { smoothMove(servo2, servo2Angle, CENTER_ANGLE, SLOW_DELAY); servo2Moved = false; }
  }
  else {
    if (servo1Angle != CENTER_ANGLE) { smoothMove(servo1, servo1Angle, CENTER_ANGLE, FAST_DELAY); }
    if (servo2Moved) { smoothMove(servo2, servo2Angle, CENTER_ANGLE, FAST_DELAY); servo2Moved = false; }
    if (servo3Moved) { smoothMove(servo3, servo3Angle, CENTER_ANGLE, FAST_DELAY); servo3Moved = false; }
    servo1Moved = false; 
  }

  delay(30);
}