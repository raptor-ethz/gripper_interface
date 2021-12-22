#include <Servo.h>

// servos for right and left grippers
Servo r_servo;
Servo l_servo;

const int max_angle = 80;

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
    int cmd = (int)Serial.read();
    if ((cmd >= 0) && (cmd < max_angle)) {  // left arm
      l_servo.write(cmd);
    }
    if ((cmd >= max_angle) && (cmd < 2 * max_angle)) {  // right arm
      r_servo.write(180 - (cmd - max_angle));
    }
    if ((cmd >= 2 * max_angle) && (cmd < 3 * max_angle)) {  // both arms
      l_servo.write(cmd - 2 * max_angle);
      r_servo.write(180 - (cmd - 2 * max_angle));
    }
  }
}