// Fastdds
#include <stdio.h>
#include <string.h>
#include <sub_callback.h>

#include <iostream>
#include <string>

#include "default_participant.h"
#include "default_subscriber.h"
#include "sub_callback.h"

// serialib
#include <stdio.h>
#include <unistd.h>

#include "../include/serialib/serialib.h"

#define SERIAL_PORT "/dev/ttyACM2"

int main(int argc, char* argv[]) {
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
  DDSSubscriber cmd_sub(idl_msg::QuadPositionCmdPubSubType(), &sub::grip_cmd,
                        "grip_cmd", dp.participant());
  // Intiailize fastdds subscriber
  cmd_sub.init();
  // Serial object
  serialib serial;

  // If connection fails, return the error code otherwise, display a success
  // message
  char errorOpening = serial.openDevice(serial_port.c_str(), 115200);
  if (errorOpening != 1) return errorOpening;
  std::cout << "Successful connection to " << serial_port << std::endl;

  // cout loop fro testing
  while (true) {
    cmd_sub.listener->wait_for_data();
    int cmd = (float)sub::grip_cmd.position.x;
    serial.writeChar((char)cmd);
    std::cout << (int)sub::grip_cmd.position.x << std::endl;
  }
  serial.closeDevice();
  return 0;
}