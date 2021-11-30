// Fastdds
#include <stdio.h>
#include <sub_callback.h>

#include <iostream>

#include "default_participant.h"
#include "default_subscriber.h"
#include "sub_callback.h"

int main() {
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

  FILE *file;
  // Opening device file

  while (true) {
    file = fopen("/dev/ttyACM0", "w");
    cmd_sub.listener->wait_for_data();
    // std::cout << ">>" << std::endl;
    // std::cin >> getnum;
    fprintf(file, "%d",
            (int)sub::grip_cmd.position.x);  // Writing to the file
    fclose(file);
  }
}

// #include <stdio.h>

// #include <iostream>
// int main() {
//   FILE *file;
//   // Opening device file

//   int getnum;

//   while (true) {
//     file = fopen("/dev/ttyACM0", "w");
//     std::cout << ">>" << std::endl;
//     std::cin >> getnum;
//     fprintf(file, "%d", getnum);  // Writing to the file
//     fclose(file);
//   }
// }