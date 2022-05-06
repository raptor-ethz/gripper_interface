
#include <Servo.h>

// Define Limits (depended on gripper structure)
#define ROT_SRV1_UP 180 // TODO -> insert right vals
#define ROT_SRV1_DW 80  // TODO -> insert right vals
#define ROT_SRV2_UP 180 // TODO -> insert right vals
#define ROT_SRV2_DW 90  // TODO -> insert right vals

#define LIN_SRV1_UP 180 // TODO -> insert right vals
#define LIN_SRV1_DW 80  // TODO -> insert right vals
#define LIN_SRV2_UP 180 // TODO -> insert right vals
#define LIN_SRV2_DW 90  // TODO -> insert right vals

#define ROTLIN_SRV1_UP 180
#define ROTLIN_SRV1_DW 80
#define ROTLIN_SRV2_UP 180
#define ROTLIN_SRV2_DW 90
#define ROTLIN_SRV3_UP 155
#define ROTLIN_SRV3_DW 0

// define Servo Port numbers
#define SRV_PORT_1 9
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

// Global Variables
int gripper = 0;
int servo_1_deg = 0;
int servo_2_deg = 0;
int servo_3_deg = 0;
bool grasp = false;
bool request = false;

void setup()
{
    // initialize Servos
    Servo_1.attach(SRV_PORT_1);
    Servo_2.attach(SRV_PORT_2);
    Servo_3.attach(SRV_PORT_3);

    // initialize Serial
    Serial.begin(115200);

    // Initialize Analog Inputs
    pinMode(SENSOR_PORT_1, INPUT);
    pinMode(SENSOR_PORT_2, INPUT);
    pinMode(SENSOR_PORT_3, INPUT);
    pinMode(SENSOR_PORT_4, INPUT);
}

void loop()
{
    // for (int i = START_3; i < END_3; i++)
    // {
    //     Servo_3.write(i);
    //     delay(4);
    // }
    // for (int i = END_3; i > START_3; i--)
    // {
    //     Servo_3.write(i);
    //     delay(4);
    // }
    // for (int i = 90; i < 180; i++)
    // {
    //     Servo_1.write(i);
    //     Servo_2.write(i);
    //     if ((i - 90) * 2 < END_3)
    //     {
    //         Servo_3.write((i - 90) * 2);
    //     }
    //     delay(10);
    // }
    // delay(1000);
    // for (int i = 180; i > 90; i--)
    // {
    //     Servo_1.write(i);
    //     Servo_2.write(i);
    //     if ((2 * i - 210) > START_3)
    //     {
    //         Servo_3.write((2 * i - 210));
    //     }
    //     delay(10);
    // }
    // delay(1000);
}

void SerialEvent()
{
    // read whole package
    int Data[] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 8; i++)
    {
        Data[i] = (int)Serial.read();
    }
}