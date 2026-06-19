#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define IR3 13
#define IR2 12
#define IR1 11
#define S0 10
#define S1 9
#define S2 8
#define S3 7
#define sensorOut 6
#define I3 5
#define I2 4
#define I1 3
#define belt 2

int irr1 = 0;
int irr2 = 0;
int irr3 = 0;

int order[100];
int front = -1;
int back = 0;

int redMin = 19, redMax = 141;
int greenMin = 17, greenMax = 210;
int blueMin = 18, blueMax = 171;

int redPW = 0, greenPW = 0, bluePW = 0;
int redValue, greenValue, blueValue;

void setup() {
  // initialize I2C LCD
  lcd.init();        // initialize the lcd
  lcd.backlight();   // turn on backlight (if supported)
  lcd.print("Color Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
  Serial.begin(9600);

  pinMode(IR3, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR1, INPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(I3, OUTPUT);
  pinMode(I2, OUTPUT);
  pinMode(I1, OUTPUT);
  pinMode(belt, OUTPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  digitalWrite(I3, LOW);
  digitalWrite(I2, LOW);
  digitalWrite(I1, LOW);
  digitalWrite(belt, LOW);

  for (int i = 0; i < 15; i++) {
    Serial.println();
  }

  order[back] = -1;
}

void loop() {
  // read color sensor
  redPW = getRedPW();
  greenPW = getGreenPW();
  bluePW = getBluePW();

  redValue = map(redPW, redMin, redMax, 255, 0);
  greenValue = map(greenPW, greenMin, greenMax, 255, 0);
  blueValue = map(bluePW, blueMin, blueMax, 255, 0);

  redValue = constrain(redValue, 0, 255);
  greenValue = constrain(greenValue, 0, 255);
  blueValue = constrain(blueValue, 0, 255);

  if (Serial.available()) {
    char cmd = Serial.read();

    if(cmd == 'r') {
      Serial.println(digitalRead(3));
      Serial.println(digitalRead(4));
      Serial.println(digitalRead(5));
      Serial.println(digitalRead(11));
      Serial.println(digitalRead(12));
      Serial.println(digitalRead(13));
      Serial.println(redValue);
      Serial.println(greenValue);
      Serial.println(blueValue);
    }
  }

  if(back - front == 1) {
    lcd.setCursor(0, 0);
    lcd.print("NO OBJECT       ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
  else {
    switch (order[front+1]) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("COLOR: RED      ");
        lcd.setCursor(0, 1);
        lcd.print("COUNT: ");
        lcd.print(irr1);
        if(irr2 < 10) lcd.print("        ");
        else if(irr2 > 9 && irr2 < 99) lcd.print("        ");
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.print("COLOR: GREEN    ");
        lcd.setCursor(0, 1);
        lcd.print("COUNT: ");
        lcd.print(irr2);
        if(irr2 < 10) lcd.print("        ");
        else if(irr2 > 9 && irr2 < 99) lcd.print("        ");
        break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print("COLOR: BLUE      ");
        lcd.setCursor(0, 1);
        lcd.print("COUNT: ");
        lcd.print(irr3);
        if(irr3 < 10) lcd.print("        ");
        else if(irr3 > 9 && irr3 < 99) lcd.print("        ");
        break;
      default:
        lcd.print("UNKNOWN ");
        break;
    }
  } 

  getColorName(redValue, greenValue, blueValue);

  if (!irr1 && !irr2 && !irr3) {
    digitalWrite(belt, LOW);
  }
  else if (irr1 && order[front + 1] == 0) {
    if (digitalRead(IR1)) digitalWrite(belt, LOW);
    else {
      delay(20);
      irr1--;
      front++;
      lcd.setCursor(0, 0);
      lcd.print("COLOR: ");
      lcd.print("RED         ");
      lcd.setCursor(0, 1);
      lcd.print("PUSHED          ");
      Serial.println("RED PUSHED");
      Serial.println("BELT QUEUE = ");
      Serial.print("[");
      for (int i = front + 1; i < back; i++) {
        Serial.print(order[i]);
        if (i != back - 1) Serial.print(",");
      }
      Serial.println("]");
      digitalWrite(belt, HIGH);
      digitalWrite(I1, LOW);
      delay(500);
      digitalWrite(I1, irr3);
    }
  }
  else if (irr2 && order[front + 1] == 1) {
    if (digitalRead(IR2)) digitalWrite(belt, LOW);
    else {
      delay(20);
      irr2--;
      front++;
      lcd.print("COLOR: ");
      lcd.print("GREEN       ");
      lcd.setCursor(0, 1);
      lcd.print("PUSHED          ");
      Serial.println("GREEN PUSHED");
      Serial.println("BELT QUEUE = ");
      Serial.print("[");
      for (int i = front + 1; i < back; i++) {
        Serial.print(order[i]);
        if (i != back - 1) Serial.print(",");
      }
      Serial.println("]");
      digitalWrite(belt, HIGH);
      digitalWrite(I2, LOW);
      delay(500);
      digitalWrite(I2, irr2);
    }
  }
  else if (irr3 && order[front + 1] == 2) {
    if (digitalRead(IR3)) digitalWrite(belt, LOW);
    else {
      delay(20);
      irr3--;
      front++;
      lcd.print("COLOR: ");
      lcd.print("BLUE        ");
      lcd.setCursor(0, 1);
      lcd.print("PUSHED          ");
      Serial.println("BLUE PUSHED");
      Serial.println("BELT QUEUE = ");
      Serial.print("[");
      for (int i = front + 1; i < back; i++) {
        Serial.print(order[i]);
        if (i != back - 1) Serial.print(",");
      }
      Serial.println("]");
      digitalWrite(belt, HIGH);
      digitalWrite(I3, LOW);
      delay(500);
      digitalWrite(I3, irr3);
    }
  }
}


int getRedPW() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  return pulseIn(sensorOut, LOW);
}

int getGreenPW() {
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  return pulseIn(sensorOut, LOW);
}

int getBluePW() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  return pulseIn(sensorOut, LOW);
}

String getColorName(int r, int g, int b) {
  if (r < 50 && g < 50 && b < 50) {
    return "BLACK";
  }
  else if (r > 200 && g > 200 && b > 200) {
    return "WHITE";
  }
  else if (r > g && r > b) {
    irr1++;
    order[back++] = 0;
    digitalWrite(I1, irr1);
    Serial.println("RED DETECTED");
    Serial.println("BELT QUEUE = ");
    Serial.print("[");
    for (int i = front + 1; i < back; i++) {
      Serial.print(order[i]);
      if (i != back - 1) Serial.print(",");
    }
    Serial.println("]");
    digitalWrite(belt, HIGH);
    delay(500);
    digitalWrite(belt, LOW);
    delay(500);
    return "RED";
  }
  else if (g > r && g > b) {
    irr2++;
    order[back++] = 1;
    digitalWrite(I2, irr2);
    Serial.println("GREEN DETECTED");
    Serial.println("BELT QUEUE = ");
    Serial.print("[");
    for (int i = front + 1; i < back; i++) {
      Serial.print(order[i]);
      if (i != back - 1) Serial.print(",");
    }
    Serial.println("]");
    delay(500);
    return "GREEN";
  }
  else if (b > r && b > g) {
    irr3++;
    order[back++] = 2;
    digitalWrite(I3, irr3);
    Serial.println("BLUE DETECTED");
    Serial.println("BELT QUEUE = ");
    Serial.print("[");
    for (int i = front + 1; i < back; i++) {
      Serial.print(order[i]);
      if (i != back - 1) Serial.print(",");
    }
    Serial.println("]");
    delay(500);
    return "BLUE";
  }
  else return "UNKNOWN";
}
