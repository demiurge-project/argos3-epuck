add_library(init_port STATIC e_init_port.c)
add_library(motor_led STATIC e_led.c e_motors.c e_motors_timer3.c)
add_subdirectory(advance_one_timer)
