

/* Sweep

  by BARRAGAN <http://barraganstudio.com>

  This example code is in the public domain.

  modified 8 Nov 2013

  by Scott Fitzgerald

  http://www.arduino.cc/en/Tutorial/Sweep

*/

#include <Servo.h>

// servos for right and left grippers

Servo r_servo;

Servo l_servo;

// above this, consodered as object_detected

const int threshold = 150;

int really_dect = 0;

const int max_angle = 80;

// check if object has been object_detected

bool check_object_detected();

bool object_detected = false;

// int start_r = 160;  // somewhere down or middle

int start_r = 160; // up

int start_l = 20; // somewhere down or middle

int angle_speed = 1;

int end_r = 110;

int end_l = 75;

void setup() {

  // initialize serial communication at 9600 bits per second:

  // initialize serial:

  Serial.begin(115200);

  // Serial.begin(9600);

  // use analog pin 1 for voltage measurement

  pinMode(A2, INPUT);

  // attaches the servos [r_servo-> D9, l_servo->D6]

  r_servo.attach(9);

  l_servo.attach(6);

  r_servo.write(160);

  l_servo.write(20);
}

void loop() {

  float sensorValue = analogRead(A2);

  int message = (int)(sensorValue * 255.0 / 1024.0);

  Serial.write(message);

  // Serial.println(message);

  delay(100);
}

bool check_object_detected() {

  //  delay(200);

  // reading voltag

  float sensorValue = analogRead(A2);

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 -
  // 5V):

  float voltage = sensorValue * (5.0 / 1023.0) * 1000;

  Serial.print("Servo volt: ");

  Serial.println(voltage);

  if (voltage <= threshold) {

    // reset variable, only want high values direct in Folge to be consired as
    // object detected

    really_dect = 0;
  }

  // print sensor value to serial monitor

  if (voltage > threshold) {

    really_dect = really_dect + 1;
  }

  if (really_dect == 3) {

    really_dect = 0;

    return true;
  }

  return false;
}

void serialEvent() {

  while (Serial.available()) {

    // Depending on the angle, there are multiple options for actuation.

    // If the command is in between [0, max_angle]: actuate left arm

    // If the command is in between [max_angle, 2*max_angle]: actuate right arm

    // If the command is in between [2*max_angle, 3*max_angle]: actuate both

    // arms symmetrically

    int cmd = (int)Serial.read();

    if ((cmd >= 0) && (cmd < max_angle)) { // left arm

      l_servo.write(end_l - cmd);

      start_l = end_l - cmd;
    }

    if ((cmd >= max_angle) && (cmd < 2 * max_angle)) { // right arm

      r_servo.write(end_r + cmd - max_angle);

      start_r = end_r + cmd;
    }

    if ((cmd >= 2 * max_angle) &&
        (cmd < 3 * max_angle)) { // grab with force sensor

      int angle_r = start_r;

      int angle_l = start_l;

      while (true) {

        if (angle_l < end_l) {

          angle_l += angle_speed;
        }

        if (angle_r > end_r) {

          angle_r -= angle_speed;
        }

        r_servo.write(angle_r);

        l_servo.write(angle_l);

        if (object_detected) {

          r_servo.write(
              angle_r -
              3); // gsomewhere down or middle (right now right upright)

          l_servo.write(angle_l + 3);

          break;
        }
      }
    }
  }
}
