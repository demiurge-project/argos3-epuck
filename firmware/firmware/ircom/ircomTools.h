
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

#ifndef IRCOM_TOOLS_H
#define IRCOM_TOOLS_H


float ircomEstimateDistance (int irPerceived);
float ircomEstimateDirection (int middleSensor);
void ircomInt2Bin (long int n, ircomWord w);
long int ircomBin2Int(volatile ircomWord w);
long int ircomGetTime();
void ircomResetTime (); 

// IRCOM_TOOLS_H
#endif
