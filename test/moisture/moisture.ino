/*
  Code for using ESP8266 to read moisture
  
  Made by: NIHAL T P
  GitHub: https://github.com/nihaltp
  LinkedIn: https://www.linkedin.com/in/nihal-tp/
*/

// MARK: setup
void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
}

// MARK: loop
void loop() {
  int moisture = analogRead(A0);
  Serial.print("Moisture: ");
  Serial.println(moisture);
  // TODO: add mapping function
  delay(1000);
}
