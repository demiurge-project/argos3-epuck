
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
*/


#ifndef IRCOM_TOOLS_C
#define IRCOM_TOOLS_C

#include <stdio.h>
#include <e_epuck_ports.h>


#include "ircom.h"
#include "ircomTools.h"

float ircomEstimateDistance (int irPerceived) 
{

  static float lutDistance[] = {  7.5,  8.0,  8.5,  9.0,  9.5, 10.0, 10.5, 11.0, 11.5, 12.0, 
                                 12.5, 13.0, 13.5, 14.0, 14.5, 15.0, 15.5, 16.0, 16.5, 17.0, 
								 17.5, 18.0, 18.5, 19.0, 19.5, 20.0, 20.5, 21.0, 21.5, 22.0, 22.5};
  static float lutCoeff[]    = { 3533.795, 2103.529, 1135.429, 701.817, 508.598, 375.808, 305.284, 264.408, 206.877, 167.442, 
                                  137.039,  113.265,   91.575,  79.199,  69.404,  66.090,  51.444,  44.032,  38.548,  34.990, 
								   31.674,   27.999,   25.922,  23.381,  21.986,  20.040,  18.303,  17.134,  16.112,  14.560, 10};

  int i = 0;
  while (irPerceived < lutCoeff[i])
      i++;

  float distance;
  if(i==0) distance = lutDistance[0];
  else  distance = lutDistance[i-1] + (lutDistance[i] - lutDistance[i-1]) * (irPerceived - lutCoeff[i]) / (lutCoeff[i - 1] - lutCoeff[i]);

  return distance;
}

float ircomEstimateDirection (int middleSensor)
{
    static float lutAngle[] = {5.986479, 5.410521, 4.712389, 3.665191, 2.617994, 1.570796, 0.8726646, 0.296706};
    return (lutAngle[middleSensor]); 
}

void ircomInt2Bin (long int n, ircomWord w)
{
    int i;

    for (i = IRCOM_WORDSIZE - 1; i >= 0 ; i--)
    {
		w[i] = n % 2;
		n = n >> 1;
    } 
}

long int ircomBin2Int(volatile ircomWord w)
{
    long int n = 0;
    
    int i; 
    for (i = 0; i < IRCOM_WORDSIZE; i++)
    {
		n = n << 1;
		n += w[i];
    }

    return n;
}

// sampling time = 81.3 usec
long int ircomGetTime( void )
{
    return ircomData.time;
}

void ircomResetTime ( void ) 
{
    ircomData.time = 0;
}


// IRCOM_TOOLS_C
#endif
