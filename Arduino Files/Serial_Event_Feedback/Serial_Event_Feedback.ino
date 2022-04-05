

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

  //Serial.begin(9600);

  // use analog pin 1 for voltage measurement

  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);


  // attaches the servos [r_servo-> D9, l_servo->D6]

  r_servo.attach(9);

  l_servo.attach(6);

  r_servo.write(160);

  l_servo.write(20);
}

int crop(int data, int lower_boundary, int upper_boundary) {
  if (data > upper_boundary) {
    return upper_boundary;
  }
  if (data < lower_boundary) {
    return lower_boundary;
  }
  return data;
}

void loop() {

  //analog read
  /*float sens_fr = analogRead(A2);
    float sens_fl = analogRead(A3);
    float sens_br = analogRead(A4);
    float sens_bl = analogRead(A5);

    //normalize to 0-100
    int msg_fr = (int)(sens_fr * 100.0 / 1024.0);
    int msg_fl = (int)(sens_fl * 100.0 / 1024.0);
    int msg_br = (int)(sens_br * 100.0 / 1024.0);
    int msg_bl = (int)(sens_bl * 100.0 / 1024.0);*/

  //check if in range
  /*msg_fr = crop(msg_fr,0,100);
    msg_fl = crop(msg_fl,0,100);
    msg_br = crop(msg_br,0,100);
    msg_bl = crop(msg_bl,0,100);*/

  //Serial.write(message);
  /*Serial.write(110); //code for front_right
    Serial.write(msg_fr);
    Serial.write(111); //code for front_left
    Serial.write(msg_fl);
    Serial.write(112); //code for back_right
    Serial.write(msg_br);
    Serial.write(113); //code for back_left
    Serial.write(msg_bl);*/
  /*  Serial.println("=========");
    Serial.println(msg0);
    Serial.println(msg1);
    Serial.println(msg2);
    Serial.println(msg3);*/
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

    if (cmd == 180) {
      float sens_fr = analogRead(A2);
      float sens_fl = analogRead(A3);
      float sens_br = analogRead(A4);
      float sens_bl = analogRead(A5);

      //normalize to 0-100
      int msg_fr = (int)(sens_fr * 100.0 / 1024.0);
      int msg_fl = (int)(sens_fl * 100.0 / 1024.0);
      int msg_br = (int)(sens_br * 100.0 / 1024.0);
      int msg_bl = (int)(sens_bl * 100.0 / 1024.0);
      
      //Serial.write(message);
      Serial.write(110); //code for front_right
      Serial.write(msg_fr);
      Serial.write(111); //code for front_left
      Serial.write(msg_fl);
      Serial.write(112); //code for back_right
      Serial.write(msg_br);
      Serial.write(113); //code for back_left
      Serial.write(msg_bl);
    }

    if (cmd == 181) { // grab with force sensor (currently deactivated)

      /*int angle_r = start_r;

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
        }*/
    }
  }
}
