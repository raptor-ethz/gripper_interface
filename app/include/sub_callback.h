#pragma once
#include "RotGripCmd_msg.h"
#include "RotGripCmd_msgPubSubTypes.h"
#include "GripperSensor_msg.h"
#include "GripperSensor_msgPubSubTypes.h"
// Subscriber data that needs to be accessed in main
namespace sub
{
    cpp_msg::RotGripCmd_msg grip_cmd;
}

namespace pub
{
    cpp_msg::GripperSensor_msg sensor_msg;
}