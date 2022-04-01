// Fastdds
#include <stdio.h>
#include <string.h>
#include <sub_callback.h>

#include <iostream>
#include <string>

#include "domain_participant.h"
#include "sub_callback.h"
#include "subscriber.h"

// serialib
#include <stdio.h>
#include <unistd.h>

#include "../include/serialib/serialib.h"

#define SERIAL_PORT "/dev/ttyACM2"

const int max_angle = 80; // defined also in arduino code

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "ERROR: no serial port given as input argument" << std::endl;
    return 0;
  }
  std::string serial_port = argv[1];
  std::cout << "Initializing serial connection to port " << serial_port
            << " ..." << std::endl;
  // Fastdds (We need to create a new message for gripper commands, but for now
  // we just use a position command message and use the x element to send
  // gripper commands)

  // Create participant. Arguments-> Domain id, QOS name
  DefaultParticipant dp(0, "gripper_interface");
  // Create subscriber with msg type
  DDSSubscriber grip_cmd_sub(idl_msg::RotGripCmd_msgPubSubType(),
                             &sub::grip_cmd, "grip_cmd", dp.participant());
  // Intiailize fastdds subscriber
  // cmd_sub.init();
  // Serial object
  serialib serial;

  // If connection fails, return the error code otherwise, display a success
  // message
  char errorOpening = serial.openDevice(serial_port.c_str(), 115200);
  if (errorOpening != 1)
    return errorOpening;
  std::cout << "Successful connection to " << serial_port << std::endl;

  // cout loop fro testing
  while (true) {
    grip_cmd_sub.listener->wait_for_data();

    int front_cmd = sub::grip_cmd.front_arm_deg;
    serial.writeChar((char)front_cmd);

    int back_cmd = sub::grip_cmd.back_arm_deg + max_angle;
    serial.writeChar((char)back_cmd);

    // if (sub::grip_cmd.trigger_gripper == true) {
    //   serial.writeChar((char)180);
    // }
    // std::cout << "front arm: " << sub::grip_cmd.front_arm_deg << "\t back arm
    // "
    //           << sub::grip_cmd.back_arm_deg << std::endl;
    // char force_val = 'a';
    // serial.readChar(&force_val, 20);
    // if ((int)force_val != 97) {
    //   std::cout << "force value: " << (int)force_val << std::endl;
    // }
  }
  serial.closeDevice();
  return 0;
}
