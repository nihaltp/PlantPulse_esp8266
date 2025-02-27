/*
  Code for using ESP8266 to control a BTS motor driver 
  to move the sensor up and down
  
  Made by: NIHAL T P
  GitHub: https://github.com/nihaltp
  LinkedIn: https://www.linkedin.com/in/nihal-tp/
*/

#define SERIAL_PORT true   // TODO: Change to: false

// TODO: Change Pins
#warning "Verify pin assignments for motors before running the code."
// These pins should be able to handle 0-255 PWM
const int IN1  = D0; // IN1
const int IN2  = D1; // IN2

const int LS1  = D2; // Limit Switch On Top

// TODO: Adjust speed as needed
const int SMSpeed = 100;       // Speed for the sensor motor, max - 255, min - 0
const int SMTime = 5 * 1000;  // Time to move the sensor up and down

unsigned long previousMillis = 0;
int state = 0;

#define debugPrintln(x) if (SERIAL_PORT) Serial.println(x)

void BTS_movements(int x);

// MARK: setup
void setup() {
  Serial.begin(115200);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(LS1, INPUT);
  
  // Set the initial state of the motor
  while (digitalRead(LS1) == LOW)
  {
    BTS_movements(SMSpeed);
  }
}

void loop() {
  if (previousMillis == 0) {
    previousMillis = millis();
  }
  
  switch(state) {
    case 0:
      debugPrintln("Moving the sensor down.");
      BTS_movements(SMSpeed);
      if (millis() - previousMillis >= SMTime) {
        previousMillis = millis();
        state += 1;
      }
      break;
    
    case 1:
      BTS_movements(-SMSpeed);
      debugPrintln("Moving the sensor up.");
      if (millis() - previousMillis >= SMTime) {
        previousMillis = millis();
        state += 1;
      }
      break;
    
    default:  // Stop or transition to another behavior
      BTS_movements(0);
      debugPrintln("Stopped the movement.");
      break;
  }
}

/**
 * Sets the motor rotation speed based on the given values.
 * @param x speed for the left motor, positive for forward and negative for backward
 */

// MARK: BTS movements
void BTS_movements(int x) {
  analogWrite(IN1, max(0, x));
  analogWrite(IN2, max(0, -x));
}
