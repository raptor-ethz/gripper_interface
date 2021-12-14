// Fastdds
#include <stdio.h>
#include <sub_callback.h>

#include <iostream>

#include "default_participant.h"
#include "default_subscriber.h"
#include "sub_callback.h"

// serialib
#include <stdio.h>
#include <unistd.h>

#include "../include/serialib/serialib.h"

#define SERIAL_PORT "/dev/ttyACM2"

int main() {
  std::cout << "hey";
  // Fastdds (We need to create a new message for gripper commands, but for now
  // we just use a position command message and use the x element to send
  // gripper commands)
  // sub::pos_cmd.position.x = 1 -> gripper closes
  // sub::pos_cmd.position.x = 0 -> gripper opens
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
  char errorOpening = serial.openDevice(SERIAL_PORT, 115200);
  if (errorOpening != 1) return errorOpening;
  std::cout << "Successful connection to " << SERIAL_PORT << std::endl;

  // cout loop fro testing
  while (true) {
    cmd_sub.listener->wait_for_data();
    if (sub::grip_cmd.position.x == 1) {
      serial.writeChar('a');
    }
    if (sub::grip_cmd.position.x == 0) {
      serial.writeChar('b');
    }

    std::cout << (int)sub::grip_cmd.position.x << std::endl;
  }
  serial.closeDevice();
}