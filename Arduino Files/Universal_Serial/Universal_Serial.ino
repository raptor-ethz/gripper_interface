
#include <Servo.h>

// Define Limits (depended on gripper structure)

// rotational gripper
#define ROT_SRV1_OP 110
#define ROT_SRV1_CL 0
#define ROT_SRV2_OP 110
#define ROT_SRV2_CL 0

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
#define SRV_PORT_2 9
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

// create buffer for Serial receiving (RX)
const int RX_BUFFER_SIZE = 6;
uint8_t rx_buffer[RX_BUFFER_SIZE];

// create buffer for Serial sending (TX)
const int TX_BUFFER_SIZE = 4;
uint8_t tx_buffer[TX_BUFFER_SIZE];

void setup()
{
    // initialize Servos
    Servo_1.attach(SRV_PORT_1, 750, 2250);
    Servo_2.attach(SRV_PORT_2, 750, 2250);
    // Servo_3.attach(SRV_PORT_3);

    // open gripper (not finished)
    Servo_1.write(ROT_SRV1_OP);
    Servo_2.write(ROT_SRV2_OP);

    // pinMode(SRV_PORT_1, OUTPUT);
    //  initialize Serial
    Serial.begin(115200);

    // Initialize Analog Inputs
    pinMode(SENSOR_PORT_1, INPUT);
    pinMode(SENSOR_PORT_2, INPUT);
    pinMode(SENSOR_PORT_3, INPUT);
    pinMode(SENSOR_PORT_4, INPUT);
}

int cap(int val, int lower_boundary, int upper_boundary)
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

    /* SERVO TEST */
    // Servo_1.write(0);
    // Servo_2.write(0);
    // delay(1000);
    // Servo_1.write(110);
    // Servo_2.write(110);
    // delay(3000);

    /* SERIAL CONNECTION */
    if (Serial.available())
    {
        Serial.readBytes(rx_buffer, RX_BUFFER_SIZE);
    }
    else
    {
        // no data available
        rx_buffer[0] = 3;
    }

    switch (rx_buffer[0])
    {
    case 0: // rotational gripper
        // servo 1
        Servo_1.write(cap(rx_buffer[1], ROT_SRV1_CL, ROT_SRV1_OP));
        // servo 2
        Servo_2.write(cap(rx_buffer[2], ROT_SRV2_CL, ROT_SRV2_OP));
        // no servo 3
        // trigger grasp
        // TODO
        // send back sensor values
        if (rx_buffer[5] == 1)
        {
            tx_buffer[0] = (uint8_t)map(analogRead(SENSOR_PORT_1), 0, 1023, 0, 100);
            tx_buffer[1] = (uint8_t)map(analogRead(SENSOR_PORT_2), 0, 1023, 0, 100);
            tx_buffer[2] = (uint8_t)map(analogRead(SENSOR_PORT_3), 0, 1023, 0, 100);
            tx_buffer[3] = (uint8_t)map(analogRead(SENSOR_PORT_4), 0, 1023, 0, 100);
            Serial.write(tx_buffer, TX_BUFFER_SIZE);
        }
        break;
    case 1: // linear gripper
        // servo 1
        Servo_1.write(cap(rx_buffer[1], LIN_SRV1_UP, LIN_SRV1_DW));
        // servo 2
        Servo_2.write(cap(rx_buffer[2], LIN_SRV2_UP, LIN_SRV2_DW));
        // no servo 3
        // no grasp algo because no force sensors on linear gripper
        // no force sensors on linear gripper
        break;
    case 2: // rot-lin gripper
        // servo 1
        Servo_1.write(cap(rx_buffer[1], ROTLIN_SRV1_UP, ROTLIN_SRV1_DW));
        // servo 2
        Servo_2.write(cap(rx_buffer[2], ROTLIN_SRV2_UP, ROTLIN_SRV2_DW));
        // no servo 3
        Servo_2.write(cap(rx_buffer[3], ROTLIN_SRV3_UP, ROTLIN_SRV3_DW));
        // no grasp algo because no force sensors on rot-lin gripper
        // no force sensors on rot-lin gripper
        break;
    }
}