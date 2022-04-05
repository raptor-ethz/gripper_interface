// Fastdds
#include <stdio.h>
#include <string.h>
#include <sub_callback.h>

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "domain_participant.h"
#include "sub_callback.h"
#include "subscriber.h"
#include "publisher.h"

// serialib
#include <stdio.h>
#include <unistd.h>

#include "../include/serialib/serialib.h"

#define SERIAL_PORT "/dev/ttyACM2"

const int max_angle = 80; // defined also in arduino code

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
  // Fastdds

  // Create participant. Arguments-> Domain id, QOS name
  DefaultParticipant dp(0, "gripper_interface");
  // Create subscriber with msg type
  DDSSubscriber grip_cmd_sub(idl_msg::RotGripCmd_msgPubSubType(),
                             &sub::grip_cmd, "grip_cmd", dp.participant());
  // Create publisher with msg type
  DDSPublisher sensor_msg_pub(idl_msg::GripperSensor_msgPubSubType(),
                              "gripper_sensor_msg", dp.participant());

  // Serial object
  serialib serial;

  // If connection fails, return the error code otherwise, display a success
  // message
  char errorOpening = serial.openDevice(serial_port.c_str(), 115200);
  if (errorOpening != 1)
    return errorOpening;
  std::cout << "Successful connection to " << serial_port << std::endl;

  while (true)
  {
    grip_cmd_sub.listener->wait_for_data();
    serial.flushReceiver();

    int front_cmd = sub::grip_cmd.front_arm_deg;
    serial.writeChar((char)front_cmd);

    int back_cmd = sub::grip_cmd.back_arm_deg + max_angle;
    serial.writeChar((char)back_cmd);

    if (sub::grip_cmd.trigger_gripper == true)
    {
      std::cout << "triggering gripper" << std::endl;
      serial.writeChar((char)181);
    }

    // print out command
    std::cout << "arm(front/back): \t" << sub::grip_cmd.front_arm_deg << "\t "
              << sub::grip_cmd.back_arm_deg << std::endl;

    // reading sensor values
    // std::cout << "reading sensor values" << std::endl;
    serial.writeChar((char)180);
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // wait for arduino to respond
    for (int i = 0; i < 20; i++)
    {
      char force_val = (char)126;
      // std::cout << "FORCE" << (int)force_val << std::endl;
      serial.readChar(&force_val, 20);
      if ((int)force_val != 126) // check if nothing was read
      {
        if ((int)force_val == 110) // prefix for front left
        {
          serial.readChar(&force_val, 20);
          pub::sensor_msg.force_front_left = (int)force_val;
          // std::cout << "front left: " << (int)force_val << std::endl;
        }
        if ((int)force_val == 111) // prefix for front right
        {
          serial.readChar(&force_val, 20);
          pub::sensor_msg.force_front_right = (int)force_val;
          // std::cout << "front right: " << (int)force_val << std::endl;
        }
        if ((int)force_val == 112) // prefix for back left
        {
          serial.readChar(&force_val, 20);
          pub::sensor_msg.force_back_left = (int)force_val;
          // std::cout << "back left: " << (int)force_val << std::endl;
        }
        if ((int)force_val == 113) // prefix for back right
        {
          serial.readChar(&force_val, 20);
          pub::sensor_msg.force_back_right = (int)force_val;
          // std::cout << "back right: " << (int)force_val << std::endl;
        }
      }
    }
    std::cout << "sensor(br,bl,fr,fl): \t" << pub::sensor_msg.force_back_right << "\t" << pub::sensor_msg.force_back_left << "\t" << pub::sensor_msg.force_front_right << "\t" << pub::sensor_msg.force_front_left << "\t" << std::endl;
    sensor_msg_pub.publish(pub::sensor_msg);
  }
  serial.closeDevice();
  return 0;
}
