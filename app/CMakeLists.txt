add_executable(gripper_interface src/main.cpp serialib/serialib.cpp )

target_include_directories(gripper_interface PUBLIC serialib)

target_link_libraries(gripper_interface PRIVATE
comm_lib
)

# Third party dependencies
target_link_libraries(gripper_interface PRIVATE
fastcdr 
fastrtps
)
