#include <Servo.h>

boolean newCommand = false;
boolean gripperStatus = false;
int LEDPIN = 6;

// servos for right and left grippers
Servo r_servo;
Servo l_servo;

int angle_cmd = 0;
// set gripper opening limit
const int max_angle = 80;

// go to gripper home position
void close_fully();

// set angle for both gripper arms at once
void set_angle(int angle);

void setup() {
  // initialize serial:
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  r_servo.attach(9);
  l_servo.attach(6);
  close_fully();
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    left_cmd = (int)Serial.read();
    right_cmd = (int)Serial.read();
    l_servo.write(left_cmd);
    r_servo.write(right_cmd);
    // angle_cmd = (int)Serial.read();
    // if (angle_cmd < max_angle) { //left servos
    //   l_servo.write(angle_cmd);
    // }
    // if ((angle_cmd >= max_angle) && (angle_cmd < 2 * max_angle)) { //right
    // servos
    //   r_servo.write(180 - (angle_cmd - max_angle));
    // }
    // if (angle_cmd >= 2 * max_angle) { //both servos
    //   set_angle(angle_cmd - 2 * max_angle)
    // }
  }
}

void set_angle(int angle) {
  if (angle <= max_angle) {
    l_servo.write(angle);
    r_servo.write(180 - angle);
  }
}

void close_fully() { set_angle(0); }

void open_fully() { set_angle(max_angle); }
