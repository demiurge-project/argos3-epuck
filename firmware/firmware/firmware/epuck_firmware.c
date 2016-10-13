/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file argos2/real_robot/epuck/firmware/epuck_firmware.c
 *
 * This file provides the implementation, pic30 side, for the communication
 * via serial port with arm linux board. Sensors and actuators data are stored
 * in two different structures (BaseActuatorState and BaseActuatorState)
 * defined in argos2/real_robot/epuck/real_epuck_base.h
 *
 * @author Carlo Pinciroli   - <cpinciro@ulb.ac.be>
 * @author Gianpiero Francesca   - <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni - <lorenzo.garattoni3@studio.unibo.it>
 * @author Matteo Amaducci   - <matteo.amaducci2@studio.unibo.it>
 */


#include <p30f6014A.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>
//#include <motor_led/advance_one_timer/e_led.h>
//#include <motor_led/e_epuck_ports.h>
//#include <motor_led/e_init_port.h>

#include <a_d/advance_ad_scan/e_ad_conv.h>

#include <I2C/e_I2C_protocol.h>

#include <e_randb.h>
//#include <randb/e_randb.h>

#include <stdlib.h>
#include <time.h>
#include <ircom.h>
#include "../real_epuck_base.h"

#define uart2_send_buffer(buf,size) { e_send_uart2_char(buf,size); while(e_uart2_sending()); }

BaseSensorConfiguration sSensorConfiguration;
BaseActuatorState sActuatorState;
BaseActuatorState sOldActuatorState;
BaseSensorState sSensorState;


/***********************************/

/* to avoid that the math trap causes a reset: */
void _trapISR_MathError(void) {
    INTCON1bits.MATHERR = 0;
}

/***********************************/

/* receive actuator data from the serial link. The '\r' means end of all data */
int receive_data(void) {
    int i;
    char c;
    for (i = 0; i < sizeof (BaseActuatorState); ++i) {
        while (!e_getchar_uart2(&c));
        *((char*) (&sActuatorState) + i) = c;
    }

    set_BaseActuatorState();

    UInt32 delay;
    /* handle the pic30 reset sent by ARM board */
    if (sActuatorState.Reset == 'R') {
        return 0;
    }
    return 1;
}

/* receive the sensors' configuration from the serial link.  */
void receive_sensors_configuration(void) {

    int i;
    char c;
    for (i = 0; i < sizeof (BaseSensorConfiguration); ++i) {
        while (!e_getchar_uart2(&c));
        *((char*) (&sSensorConfiguration) + i) = c;
    }
    setRBconfiguration();


}

/* configure the RB with the data from the board*/
void setRBconfiguration(void) {
    if (sSensorConfiguration.RBEnabled) {
        /*RB INIT*/

        e_init_randb(I2C);
        e_randb_set_range((unsigned char) sSensorConfiguration.RBRange);

        e_randb_set_calculation(ON_BOARD);
        e_randb_store_light_conditions();
        /*END RB INIT*/

    }
    sActuatorState.RBMessageData = 0; //by default the RB send 0 as message data
    sSensorState.RBhasReceived = 0; //no message received by default

    //END INIT RB
}

/* send on the serial link sensors values (proximity, ground and RB for now)*/
void send_data(void) {
    int i;

    /*****read the 8 proximity sensor (8 int -> 16 bytes)*****/
    for (i = 0; i < 8; ++i) {
        sSensorState.Proximity[i] = e_get_prox(i);
    }
    /*****read LOW BATTERY *****/
    sSensorState.Battery_LOW=BATT_LOW;
    /*****read the accelerometer sensor (3-> 6 byte)*****/
    /*
        e_get_acc(&sSensorState.Accel[0],
                &sSensorState.Accel[1],
                &sSensorState.Accel[2]);
     */
    /*****read the micro-volume sensor (3-> 6 byte)*****/
    /*
        e_get_micro(&sSensorState.Micro[0],
                &sSensorState.Micro[1],
                &sSensorState.Micro[2]);
     */
    /*****read the 8 light sensor (8 int -> 16 bytes)*****/
    for (i = 0; i < 8; ++i) {
        sSensorState.Light[i] = e_get_ambient_light(i);
    }
    /****read the RB sensor if is enabled (8 bytes)****/
    if (sSensorConfiguration.RBEnabled) {
        unsigned char received = 0;
        received = e_randb_get_if_received();
        sSensorState.RBhasReceived = received;
        if (received != 0) {
            /*get last distance */
            sSensorState.RBMessage.Range = e_randb_get_range();

            /*get last angle*/
            sSensorState.RBMessage.Bearing = e_randb_get_bearing();
            /* Get data*/
            sSensorState.RBMessage.Data = e_randb_get_data();
        }
    }
    //SEND
    uart2_send_buffer((char*) (&sSensorState), sizeof (BaseSensorState));



}

/***********************************/

/*set the values of the actuator*/
void set_BaseActuatorState(void) {
    int i;

    /* Motors speed */
    if (sOldActuatorState.LWheel != sActuatorState.LWheel) {
        e_set_speed_left(sActuatorState.LWheel);
    }
    if (sOldActuatorState.RWheel != sActuatorState.RWheel) {
        e_set_speed_right(sActuatorState.RWheel);
    }

    /* leds */
    for (i = 0; i < 8; ++i) {
        /* check old leds state with the new one, if they are equals do nothing */
        if (((sOldActuatorState.RingLeds & (0x01 << i)) ^ (sActuatorState.RingLeds & (0x01 << i)))) {
            /* led's state has to be changed, value greater than 1 inverse the previous led's state */
            e_set_led(i, 0x10);
        }
    }

    /* front led */
    if (((sOldActuatorState.OtherLeds & (0x01)) ^ (sActuatorState.OtherLeds & (0x01)))) {
        e_set_front_led(0x10);
    }
    /* body_led */
    if (((sOldActuatorState.OtherLeds & (0x02)) ^ (sActuatorState.OtherLeds & (0x02)))) {
        e_set_body_led(0x10);
    }

    /*RB sends the packet*/
    if (sSensorConfiguration.RBEnabled) {
        e_randb_send_all_data(sActuatorState.RBMessageData);
    }



    sOldActuatorState = sActuatorState;
}

/***********************************/
void init() {

    /*waiting for linux board boot*/
    long long int delay2 = 0;
    for (delay2 = 0; delay2 < 10000000; delay2++) {
        __asm__("nop");
    }


    e_init_port();
    e_init_ad_scan();
    e_init_motors();
    e_start_agendas_processing();
    //
    // e_init_uart1();
    //
    e_init_uart2();
    /*
        e_init_acc();
        e_init_micro();
        e_init_prox();
     */
    e_calibrate_ir();

    e_i2cp_init();


    int i = 0;

    long int delay = 0;
    e_set_led(8, 0);
    for (i = 0; i < 8; i++) {
        e_set_led(i, 1);
        for (delay = 0; delay < 100000; delay++) {
            __asm__("nop");
        }
        e_set_led(i, 0);
    }
    e_set_led(8, 0);


    if (RCONbits.POR) { // reset if power on (some problem for few robots)
        RCONbits.POR = 0;
        RESET();
    }
    /* the following 3 lines are needed to release completely the camera on the robot and let use the camera on the gumstix ext.
     * Now are not needed anymore because the front camera cannot be used.
       SIO_C_DIR = INPUT_PIN;
       SIO_D_DIR = INPUT_PIN;
       CAM_RESET_DIR = INPUT_PIN;
     */



}

/***********************************/
int main(void) {
    char c;
    init();


    while (1) {

        int i = 0;
        long int delay = 0;
        int change = 0;
        e_set_led(8, 0);
        /** Wait for the ARM board to connect **/
        while (!e_getchar_uart2(&c)) { /* The code inside this loop is just to make the robot blink.Remove it if you don't like it*/
            change++;
            if (change % 10 == 0) {
                i = (i + 1) % 8;
                e_set_led(i, 1);
            }
            for (delay = 0;
                 delay < 100000; delay++) {
                __asm__("nop");
            }
            if (change % 10 == 0) {
                e_set_led(i, 0);
            }

        }
        //e_set_led(8, 1);


        /*Receive the configuration of the sensors from the board*/

        receive_sensors_configuration();
        //e_set_led(8, 0);

        /*Start IR Communication*/
        ircomStart();
        ircomEnableContinuousListening();
        ircomListen();


        /** Main cycle **/
        while (1) {
            /*Stop ircom timer interrupts while sending and receiving data on serial link (uart2)*/
            ircomStop();
            send_data();
            /* if receive_data returns 0 --> reset*/
            if (!receive_data()) {
                break;
            }

            /* Control if the IR Communication has been activated in the controller*/
            if (sActuatorState.IRComEnabled) {
                /*Restart IR Communication by re-enabling timer interrupts*/
                ircomRestart();
                ircomListen();

                ircomSend(sActuatorState.IRComMessageData);
                while (ircomSendDone() == 0);

                /*Delay between send and receive*/
                for (delay = 0; delay < 50000; delay++) {
                    __asm__("nop");
                }


                IrcomMessage imsg;
                ircomPopMessage(&imsg);
                if (imsg.error == 0) {
                    sSensorState.IRComHasReceived = 1;
                    sSensorState.IRComMessage.Data = imsg.value;
                    sSensorState.IRComMessage.Direction = imsg.direction;
                    sSensorState.IRComMessage.Distance = imsg.distance;
                    sSensorState.IRComMessage.Sensor = imsg.receivingSensor;
                    /*
                                            e_set_led(8, 0);
                                            for (i = 0; i < 8; i++) {
                                                e_set_led(i, 1);
                                                for (delay = 0; delay < 10000; delay++) {
                                                    __asm__("nop");
                                                }
                                                e_set_led(i, 0);
                                            }
                     */

                } else {
                    sSensorState.IRComHasReceived = 0;
                }
            }

        }
    }
    ircomStop();
    return 0;
}
