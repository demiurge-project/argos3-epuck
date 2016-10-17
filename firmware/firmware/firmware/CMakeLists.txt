add_executable(epuck_firmware epuck_firmware.c)

target_link_libraries(epuck_firmware ircom I2C advance_a_d bluetooth init_port motor_led motor_led_advance randb uart)
