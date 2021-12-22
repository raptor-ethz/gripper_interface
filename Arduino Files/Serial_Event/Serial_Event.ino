#include <Servo.h>

// servos for right and left grippers
Servo r_servo;
Servo l_servo;

void setup() {
  // initialize serial:
  Serial.begin(115200);

  r_servo.attach(9);
  l_servo.attach(6);

  // close fully
  l_servo.write(5);
  r_servo.write(175);
}

void loop() {}
void serialEvent() {
  while (Serial.available()) {
    int left_cmd = (int)Serial.read();
    int right_cmd = (int)Serial.read();
    l_servo.write(left_cmd);
    r_servo.write(180 - right_cmd);
  }
}