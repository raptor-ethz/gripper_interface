
#include <Servo.h>

// Define Limits (depended on gripper structure)

// rotational gripper
#define ROT_SRV1_UP 180 // TODO -> insert right vals
#define ROT_SRV1_DW 0   // TODO -> insert right vals
#define ROT_SRV2_UP 180 // TODO -> insert right vals
#define ROT_SRV2_DW 0   // TODO -> insert right vals

// linear gripper
#define LIN_SRV1_UP 180 // TODO -> insert right vals
#define LIN_SRV1_DW 80  // TODO -> insert right vals
#define LIN_SRV2_UP 180 // TODO -> insert right vals
#define LIN_SRV2_DW 90  // TODO -> insert right vals

// rotational-linear gripper
#define ROTLIN_SRV1_UP 180
#define ROTLIN_SRV1_DW 80
#define ROTLIN_SRV2_UP 180
#define ROTLIN_SRV2_DW 90
#define ROTLIN_SRV3_UP 155
#define ROTLIN_SRV3_DW 0

// define Servo Port numbers
#define SRV_PORT_1 6
#define SRV_PORT_2 10
#define SRV_PORT_3 11

// define Sensor Port numbers
#define SENSOR_PORT_1 A2
#define SENSOR_PORT_2 A3
#define SENSOR_PORT_3 A4
#define SENSOR_PORT_4 A5

// Declare Servo Objects
Servo Servo_1;
Servo Servo_2;
Servo Servo_3;

// create buffer
const int BUFFER_SIZE = 6;
uint8_t buf[BUFFER_SIZE];
int incoming = 0;

void setup()
{
    // initialize Servos
    Servo_1.attach(SRV_PORT_1);
    Servo_2.attach(SRV_PORT_2);
    Servo_3.attach(SRV_PORT_3);
    // pinMode(SRV_PORT_1, OUTPUT);
    //  initialize Serial
    Serial.begin(115200);

    // Initialize Analog Inputs
    pinMode(SENSOR_PORT_1, INPUT);
    pinMode(SENSOR_PORT_2, INPUT);
    pinMode(SENSOR_PORT_3, INPUT);
    pinMode(SENSOR_PORT_4, INPUT);
}

int cap(int val, int upper_boundary, int lower_boundary)
{
    if (val < lower_boundary)
    {
        return lower_boundary;
    }
    if (val > upper_boundary)
    {
        return upper_boundary;
    }
    return val;
}

void loop()
{
    // Servo_1.write(50);
    // delay(1000);
    // Servo_1.write(80);
    // delay(1000);
    // read data from serial port
    if (Serial.available())
    {
        incoming = Serial.readBytes(buf, 6);
    }
    else
    {
        // no data available
        buf[0] = 3;
    }

    switch (buf[0])
    {
    case 0: // rotational gripper
        // servo 1
        Servo_1.write(cap(buf[1], ROT_SRV1_UP, ROT_SRV1_DW));
        // servo 2
        Servo_2.write(cap(buf[2], ROT_SRV2_UP, ROT_SRV2_DW));
        // no servo 3
        // trigger grasp
        // TODO
        // send back sensor values
        // TODO
        break;
    case 1: // linear gripper
        // servo 1
        Servo_1.write(cap(buf[1], LIN_SRV1_UP, LIN_SRV1_DW));
        // servo 2
        Servo_2.write(cap(buf[2], LIN_SRV2_UP, LIN_SRV2_DW));
        // no servo 3
        // no grasp algo because no force sensors on linear gripper
        // no force sensors on linear gripper
        break;
    case 2: // rot-lin gripper
        // servo 1
        Servo_1.write(cap(buf[1], ROTLIN_SRV1_UP, ROTLIN_SRV1_DW));
        // servo 2
        Servo_2.write(cap(buf[2], ROTLIN_SRV2_UP, ROTLIN_SRV2_DW));
        // no servo 3
        Servo_2.write(cap(buf[3], ROTLIN_SRV3_UP, ROTLIN_SRV3_DW));
        // no grasp algo because no force sensors on rot-lin gripper
        // no force sensors on rot-lin gripper
        break;
    }
}