/*
  Code for using ESP8266 to control
  a rover using L298N motor driver
  
  Made by: NIHAL T P
  GitHub: https://github.com/nihaltp
  LinkedIn: https://www.linkedin.com/in/nihal-tp/
*/

#define SERIAL_PORT true  // TODO: Change to: false

// TODO: Change Pins
#warning "Verify pin assignments for motors before running the code."
const int L1 = D0;  // Left  1
const int L2 = D1;  // Left  2
const int R1 = D3;  // Right 1
const int R2 = D4;  // Right 2

// These pins should be able to handle 0-255 PWM
const int ENL = D2;  // Left Enable
const int ENR = D5;  // Right Enable

// PWM range of ESP8266 is 0 - 1023
const int MAX_SPEED = 1023;
const int HALF_SPEED = 512;
const int OFF_SPEED = 0;

unsigned long previousMillis = 0;
int state = 0;

#define debugPrint(x)    if (SERIAL_PORT) Serial.print(x)
#define debugPrintln(x)  if (SERIAL_PORT) Serial.println(x)

void L298Nmovements(int x, int y);

// MARK: setup
void setup() {
  Serial.begin(115200);
  
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(ENL, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(ENR, OUTPUT);
}

// MARK: loop
void loop() {
  if (previousMillis == 0) {
    previousMillis = millis();
  }
  
  switch (state) {
    case 0:  // Move forward for 10 seconds
      L298Nmovements(HALF_SPEED, HALF_SPEED);
      if (millis() - previousMillis >= 10000) {
        previousMillis = millis();
        state += 1;
      }
      break;
    
    case 1:
      L298Nmovements(HALF_SPEED, -HALF_SPEED);
      if (millis() - previousMillis >= 5000) {
        previousMillis = millis();
        state += 1;
      }
      break;
    
    case 2:
      L298Nmovements(-HALF_SPEED, HALF_SPEED);
      if (millis() - previousMillis >= 4000) {
        previousMillis = millis();
        state += 1;
      }
      break;
    
    default:
      L298Nmovements(OFF_SPEED, OFF_SPEED);
      break;
  }
}

/**
 * Sets the motor rotation speed based on the given values.
 * @param x speed for the left motor, positive for forward and negative for backward
 * @param y speed for the right motor, positive for forward and negative for backward
 */

// MARK: L298Nmovements
void L298Nmovements(int x, int y) {
  digitalWrite(L1, (x > 0) ? HIGH : LOW);
  digitalWrite(L2, (x < 0) ? HIGH : LOW);
  digitalWrite(R1, (y > 0) ? HIGH : LOW);
  digitalWrite(R2, (y < 0) ? HIGH : LOW);
  
  analogWrite(ENL, abs(x));
  analogWrite(ENR, abs(y));
}
