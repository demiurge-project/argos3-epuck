
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

#ifndef IRCOM_MESSAGES_C
#define IRCOM_MESSAGES_C

#include "ircom.h"
#include "ircomMessages.h"

void ircomFlushMessages()
{
    ircomData.messagesCount = 0;
    ircomData.messagesBegin = 0;
    ircomData.messagesEnd = 0;
}

void ircomPushMessage(long int value, float distance, float direction, int receivingSensor, int error)
{
    ircomData.messages[ircomData.messagesEnd].value = value;
    ircomData.messages[ircomData.messagesEnd].distance = distance;
    ircomData.messages[ircomData.messagesEnd].direction = direction;
    ircomData.messages[ircomData.messagesEnd].receivingSensor = receivingSensor;
    ircomData.messages[ircomData.messagesEnd].error = error;

    ircomData.messagesEnd = (ircomData.messagesEnd + 1) % IRCOM_MESSAGES_MAX;

    // if messages is full, new message deletes oldest one
    if (ircomData.messagesCount == IRCOM_MESSAGES_MAX)
    {
	ircomData.messagesBegin = (ircomData.messagesBegin + 1) % IRCOM_MESSAGES_MAX;
    }
    else
    {
	ircomData.messagesCount++;
    }
}

void ircomPopMessage(IrcomMessage* msg)
{
    if (ircomData.messagesCount == 0)
    {
	msg->error = -1;
	return;
    }

    // copy message
    msg->value = ircomData.messages[ircomData.messagesBegin].value;
    msg->distance = ircomData.messages[ircomData.messagesBegin].distance;
    msg->direction = ircomData.messages[ircomData.messagesBegin].direction;
    msg->receivingSensor = ircomData.messages[ircomData.messagesBegin].receivingSensor;
    msg->error = ircomData.messages[ircomData.messagesBegin].error;

    ircomData.messagesBegin = (ircomData.messagesBegin + 1) % IRCOM_MESSAGES_MAX;
    ircomData.messagesCount--;
}

#endif // IRCOM_MESSAGES_C
