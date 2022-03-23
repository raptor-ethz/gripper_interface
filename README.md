# RAPTOR Gripper Interface

This application is for controlling the gripper mounted to the drone. It will run onboard the Raspberry Pi companion computer. which need to be connected to a microcontroller running `Serial_Event.ino`. The application consists of a FastDDs subscriber listening to new gripper commands and once it detects a command, it will send the gripper angle from the command to the microcontroller over serial. 

## Installation

You will need the following dependencies installed on your system:
- [eProsima FastDDS](https://fast-dds.docs.eprosima.com/en/latest/installation/binaries/binaries_linux.html)
  
After the dependencies have been installed, clone this repository using: 

```bash
git clone --recursive https://github.com/raptor-ethz/gripper_interface.git
```
Then, you can compile and build it with cmake. 

## Usage

You should pass the serial connection to the microcontroller as argument when running the application. This will look something like this:

```bash
./build/app/gripper_interface /dev/ttyACM0
```