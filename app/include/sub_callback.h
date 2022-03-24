#pragma once
#include "RotGripCmd_msg.h"
#include "RotGripCmd_msgPubSubTypes.h"

// Subscriber data that needs to be accessed in main
namespace sub {
cpp_msg::RotGripCmd_msg grip_cmd;
} // namespace sub