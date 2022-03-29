#include <Servo.h>

// servos for right and left grippers
Servo r_servo;
Servo l_servo;

const int max_angle = 80;

// Before running, make sure to adjust the servo such that the closed position
// reflects an angle that is desirable (e.g. no constant load on the servos)

void setup() {
  // initialize serial:
  Serial.begin(115200);

  // Attach to pins 6 and 9. Pins must support PWM.
  r_servo.attach(9);
  l_servo.attach(6);

  // close fully
  l_servo.write(5);
  r_servo.write(175);
}
// The servos are controlled completely over the serial interrupt. This is much
// faster than a polling based setup in the loop.
void loop() {}

void serialEvent() {
  while (Serial.available()) {
    // Depending on the angle, there are multiple options for actuation.
    // If the command is in between [0, max_angle]: actuate left arm
    // If the command is in between [max_angle, 2*max_angle]: actuate right arm
    // If the command is in between [2*max_angle, 3*max_angle]: actuate both
    // arms symmetrically

    int cmd = (int)Serial.read();
    if ((cmd >= 0) && (cmd < max_angle)) { // left arm
      l_servo.write(cmd);
    }

    if ((cmd >= max_angle) && (cmd < 2 * max_angle)) { // right arm
      r_servo.write(180 - (cmd - max_angle));
    }

    if ((cmd >= 2 * max_angle) && (cmd < 3 * max_angle)) { // both arms
      l_servo.write(cmd - 2 * max_angle);
      r_servo.write(180 - (cmd - 2 * max_angle));
    }
  }
}