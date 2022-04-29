
#include <Servo.h>

Servo Servo_1;
int PORT_1 = 9;
int START_1 = 90;
int END_1 = 180;

Servo Servo_2;
int PORT_2 = 10;
int START_2 = 90;
int END_2 = 180;

Servo Servo_3;
int PORT_3 = 11;
int START_3 = 0;
int END_3 = 155;

void setup()
{
    Servo_1.attach(PORT_1);
    Servo_2.attach(PORT_2);
    Servo_3.attach(PORT_3);
    Serial.begin(9600);
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
    for (int i = 90; i < 180; i++)
    {
        Servo_1.write(i);
        Servo_2.write(i);
        if ((i - 90) * 2 < END_3)
        {
            Servo_3.write((i - 90) * 2);
        }
        delay(10);
    }
    delay(1000);
    for (int i = 180; i > 90; i--)
    {
        Servo_1.write(i);
        Servo_2.write(i);
        if ((2 * i - 210) > START_3)
        {
            Servo_3.write((2 * i - 210));
        }
        delay(10);
    }
    delay(1000);
}