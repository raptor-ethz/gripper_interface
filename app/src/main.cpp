// general dependencies
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

// serialib
#include "serialib.h"

// include fastDDS files
#include "domain_participant.h"
#include "subscriber.h"
#include "publisher.h"

// include pubSub and message types
#include "GripCmd_msg.h"
#include "GripCmd_msgPubSubTypes.h"
#include "GripperSensor_msg.h"
#include "GripperSensor_msgPubSubTypes.h"

#define SERIAL_PORT "/dev/ttyACM2"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cout << "ERROR: no serial port given as input argument" << std::endl;
    return 0;
  }
  std::string serial_port = argv[1];
  std::cout << "Initializing serial connection to port " << serial_port
            << " ..." << std::endl;
  /* FASTDDS */

  // Create participant. Arguments-> Domain id, QOS name
  DefaultParticipant dp(0, "gripper_interface");

  // Subscriber and Publisher Messages
  cpp_msg::GripCmd_msg grip_cmd;
  cpp_msg::GripperSensor_msg sensor_msg;

  // Create subscriber and publisher
  DDSSubscriber grip_cmd_sub(idl_msg::GripCmd_msgPubSubType(),
                             &grip_cmd, "grip_cmd", dp.participant());
  DDSPublisher sensor_msg_pub(idl_msg::GripperSensor_msgPubSubType(),
                              "gripper_sensor_msg", dp.participant());

  // initialize Serial object
  serialib serial;

  // If connection fails, return the error code otherwise, display a success
  // message
  char errorOpening = serial.openDevice(serial_port.c_str(), 115200);
  if (errorOpening != 1)
  {
    return errorOpening;
  }
  std::cout << "Successful connection to " << serial_port << std::endl;

  // initialize Data
  unsigned char Command_Data[] = {0, 0, 0, 0, 0, 0};
  // {starting_bit, starting_bit, gripperType, servo1, servo2, servo3, triggerGripper, requestSensorInfo}
  char Sensor_Data[] = {0, 0, 0, 0};
  // {Sensor1,Sensor2,Sensor3,Sensor4}
  for (int i = 0;; i++)
  {
    /* SERIAL SENDING */
    grip_cmd_sub.listener->wait_for_data();
    //  write bytes (8bit = conversion to char type)
    Command_Data[0] = (char)grip_cmd.gripper;
    Command_Data[1] = (char)grip_cmd.servo_1_deg;
    Command_Data[2] = (char)grip_cmd.servo_2_deg;
    Command_Data[3] = (char)grip_cmd.servo_3_deg;
    Command_Data[4] = (char)grip_cmd.trigger_gripper;
    Command_Data[5] = (char)grip_cmd.request_sensor;

    serial.writeBytes(&Command_Data, sizeof(Command_Data));
    // std::cout << "just wrote" << (int)Command_Data[2] << std::endl;
    //  /* SERIAL RECEIVING */
    //  if (grip_cmd.request_sensor)
    //  {
    //  serial.readBytes(&Sensor_Data[0], sizeof(Sensor_Data), 250);
    //  }
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // serial.readBytes(&Test_Read[0], sizeof(Sensor_Data), 250);
    // std::cout << Test_Read[0] << "\t" << Test_Read[1] << "\t" << Test_Read[2] << "\t" << Test_Read[3] << "\t" << Test_Read[4] << "\t" << Test_Read[5] << "\t" << Test_Read[6] << "\t" << Test_Read[7] << std::endl;
  }
  serial.closeDevice();
  return 0;
}

// int main(int argc, char *argv[])
// {
//   if (argc != 2)
//   {
//     std::cout << "ERROR: no serial port given as input argument" << std::endl;
//     return 0;
//   }
//   std::string serial_port = argv[1];
//   std::cout << "Initializing serial connection to port " << serial_port
//             << " ..." << std::endl;
//   // Fastdds

//   // Create participant. Arguments-> Domain id, QOS name
//   DefaultParticipant dp(0, "gripper_interface");
//   // Create subscriber with msg type
//   DDSSubscriber grip_cmd_sub(idl_msg::RotGripCmd_msgPubSubType(),
//                              &sub::grip_cmd, "grip_cmd", dp.participant());
//   // Create publisher with msg type
//   DDSPublisher sensor_msg_pub(idl_msg::GripperSensor_msgPubSubType(),
//                               "gripper_sensor_msg", dp.participant());

//   // Serial object
//   serialib serial;

//   // If connection fails, return the error code otherwise, display a success
//   // message
//   char errorOpening = serial.openDevice(serial_port.c_str(), 115200);
//   if (errorOpening != 1)
//     return errorOpening;
//   std::cout << "Successful connection to " << serial_port << std::endl;

//   while (true)
//   {
//     grip_cmd_sub.listener->wait_for_data();
//     serial.flushReceiver();

//     int front_cmd = sub::grip_cmd.front_arm_deg;
//     serial.writeChar((char)front_cmd);

//     int back_cmd = sub::grip_cmd.back_arm_deg + max_angle;
//     serial.writeChar((char)back_cmd);

//     if (sub::grip_cmd.trigger_gripper == true)
//     {
//       std::cout << "triggering gripper" << std::endl;
//       serial.writeChar((char)181);
//     }

//     // print out command
//     std::cout << "arm(front/back): \t" << sub::grip_cmd.front_arm_deg << "\t "
//               << sub::grip_cmd.back_arm_deg << std::endl;

//     // reading sensor values
//     // std::cout << "reading sensor values" << std::endl;
//     serial.writeChar((char)180);
//     std::this_thread::sleep_for(std::chrono::milliseconds(1)); // wait for arduino to respond
//     for (int i = 0; i < 20; i++)
//     {
//       char force_val = (char)126;
//       // std::cout << "FORCE" << (int)force_val << std::endl;
//       serial.readChar(&force_val, 20);
//       if ((int)force_val != 126) // check if nothing was read
//       {
//         if ((int)force_val == 110) // prefix for front left
//         {
//           serial.readChar(&force_val, 20);
//           pub::sensor_msg.force_front_left = (int)force_val;
//           // std::cout << "front left: " << (int)force_val << std::endl;
//         }
//         if ((int)force_val == 111) // prefix for front right
//         {
//           serial.readChar(&force_val, 20);
//           pub::sensor_msg.force_front_right = (int)force_val;
//           // std::cout << "front right: " << (int)force_val << std::endl;
//         }
//         if ((int)force_val == 112) // prefix for back left
//         {
//           serial.readChar(&force_val, 20);
//           pub::sensor_msg.force_back_left = (int)force_val;
//           // std::cout << "back left: " << (int)force_val << std::endl;
//         }
//         if ((int)force_val == 113) // prefix for back right
//         {
//           serial.readChar(&force_val, 20);
//           pub::sensor_msg.force_back_right = (int)force_val;
//           // std::cout << "back right: " << (int)force_val << std::endl;
//         }
//       }
//     }
//     std::cout << "sensor(br,bl,fr,fl): \t" << pub::sensor_msg.force_back_right << "\t" << pub::sensor_msg.force_back_left << "\t" << pub::sensor_msg.force_front_right << "\t" << pub::sensor_msg.force_front_left << "\t" << std::endl;
//     sensor_msg_pub.publish(pub::sensor_msg);
//   }
//   serial.closeDevice();
//   return 0;
// }
