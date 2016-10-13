
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

#ifndef IRCOM_SEND_H
#define IRCOM_SEND_H

// user interface
void ircomSend (long int value);
int ircomSendDone ();
inline int ircomIsSending ();

// module related
void ircomSendCalcSending(long int value);
void ircomSendMain();
void ircomSendSync();
void ircomSendOut();
void ircomSendNextBit();

// IRCOM_SEND_H
#endif
