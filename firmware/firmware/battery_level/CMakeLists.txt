add_executable(battery battery.c)

target_link_libraries(battery ircom I2C advance_a_d bluetooth init_port motor_led motor_led_advance randb uart)
