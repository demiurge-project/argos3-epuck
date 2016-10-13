
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

#ifndef IRCOM_RECEIVE_H
#define IRCOM_RECEIVE_H

// public
void ircomListen();
int ircomReceiveDone();
inline int ircomIsReceiving ();
inline int ircomIsListening ();
void ircomStopListening ();

// private
void ircomReceiveMain();
void ircomReceiveListen();
void ircomReceiveWord();
int ircomReceiveGetMaxSensor();
int ircomReceiveDemodulate();
int ircomReceiveCheckCRC();


// IRCOM_RECEIVE_H
#endif
