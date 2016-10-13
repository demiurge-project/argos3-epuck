/*
    Copyright 2007 Alexandre Campo, Alvaro Guttierez, Valentin Longchamp.

    This file is part of libIrcom.

    libIrcom is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    libIrcom is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libIrcom.  If not, see <http://www.gnu.org/licenses/>.

    This file was strongly inspired by another e_ad_conv.c from the 
    library released by EPFL. Authors of this "ancestor" are 
    Jean-Joël Borter, Michael Bonani, Valentin Longchamp.
*/

#ifndef _AD_CONV
#define _AD_CONV


/***********************************************************************
 * -------------------- Functions from ad_conv.c -----------------------
 **********************************************************************/
void e_init_ad_scan(void);
void e_ad_scan_on(void);
inline void e_ad_scan_reset(void);
inline void e_ad_skip_samples (int samples_count);
void e_ad_scan_off(void);
void e_ad_ircom_interrupt();
void e_ad_proximity_interrupt();
inline char e_ad_is_array_filled(void);
inline int e_get_prox(int s);
inline int e_get_calibrated_prox(int s);
inline int e_get_ambient_light(int s);
void e_calibrate_ir();

#define SAMPLING_WINDOW 32
#define NB_IR_SENSORS 8
#define ADCS_SETTING 19

extern int* ad_sampling;
extern int* ad_received;
extern volatile unsigned int e_last_ir_scan_id;

extern volatile int ad_activate_proximity;

extern volatile int ad_disable_proximity;
extern volatile int ad_disable_ircom;

extern volatile int e_ambient_ir[NB_IR_SENSORS]; // ambient light measurement
extern volatile int e_ambient_and_reflected_ir[NB_IR_SENSORS]; // light when led is on
extern volatile int e_reflected_ir[NB_IR_SENSORS]; // variation of light
extern volatile int e_init_value_ir[NB_IR_SENSORS];


#endif /*_AD_CONV*/

/* End of File : ad_conv.h */
