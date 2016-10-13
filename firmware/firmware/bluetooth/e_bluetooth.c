/********************************************************************************

			Bluetooth for e-puck
			Version 1.0 July 2006 Michael Bonani


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2006-2007 Michael Bonani

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup bluetooth
 * \brief Manage Bluetooth.
 *
 * This module manage the Bluetooth device.
 * \author Code: Michael Bonani \n Doc: Jonathan Besuchet
 */

/* Bluetooth.c */
#include "./../uart/e_uart_char.h"
#include "./../motor_led/e_epuck_ports.h"
#include "e_bluetooth.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

unsigned char e_bt_local_paired_device[6*8];
struct BtDevice e_bt_present_device[10];
struct BtEPuck e_bt_present_epuck[10];

char local_bt_PIN[4];


/*------ special e-puck fuction ------*/

/*! \brief Try to find other e-puck
*
* This function make global inguiry and check which device are e-puck, 
* and list them in globales tables.
* \return number of e-puck found
* \sa e_bt_present_device, e_bt_present_epuck
*/
int e_bt_find_epuck(void)
{
	int device_find, e_puck_find;
	int i,j;
	
	e_puck_find=0;
	device_find=e_bt_inquiry(e_bt_present_device);
	
	for (i=0;i<device_find;i++)
	{
		if((e_bt_present_device[i].class[0]==0)&(e_bt_present_device[i].class[1]==0)&(e_bt_present_device[i].class[2]==0)&(e_bt_present_device[i].address[5]==0x08))//marque of e-puck
		{
			e_bt_get_friendly_name(&e_bt_present_device[i]);
			if((e_bt_present_device[i].friendly_name[0]=='e')&(e_bt_present_device[i].friendly_name[1]=='-')&(e_bt_present_device[i].friendly_name[2]=='p'))
			{				
				for(j=0;j<4;j++)
				{
					e_bt_present_epuck[e_puck_find].address[j]=e_bt_present_device[i].address[j];//copy address
					e_bt_present_epuck[e_puck_find].number[j]=e_bt_present_device[i].friendly_name[j+7];//extract number(=pin)
				}
					e_bt_present_epuck[e_puck_find].number[4]='\0';//end with null
				for(j=4;j<6;j++)
					e_bt_present_epuck[e_puck_find].address[j]=e_bt_present_device[i].address[j];
				e_puck_find++;	
			}
		}
	}
	return e_puck_find;
}

/* \brief This function will connect to first e-puck found.
 * \return 0 if connect otherwise return error code
 */
char e_bt_connect_epuck(void)
{
	int e_puck_find;
	char error;
	
	e_bt_read_local_pin_number(local_bt_PIN);
	e_puck_find=e_bt_find_epuck();
	if(e_puck_find)
	{
		e_bt_write_local_pin_number(&e_bt_present_epuck[0].number[0]);
		error=e_bt_etablish_SPP_link(&e_bt_present_epuck[0].address[0]);
		e_bt_write_local_pin_number(local_bt_PIN);
		return error;
	}	
	else
		return 99; 
}


/*------ low level bluettoth fuction ------*/

/*! \brief Reset the bluetooth module
 * \return The version number
 */
int e_bt_reset(void)
{
	char send[7];
	char read[30];
	int i;
	char c;
	char version[5];
	
	send[0]=0x02; //send reset request
	send[1]=0x52;
	send[2]=0x26;
	send[3]=0x00;
	send[4]=0x00;
	send[5]=0x78;
	send[6]=0x03;
	e_send_uart1_char(send,7);
	
	i=0;
	c=0;
	
	 do
	    {
	      if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	     		i++;
			}	
	    }
	    while (((char)c != 0x03)||(i<(read[3]+6)));
	    read[i]='\0';
	
		for(i=0;i<read[6];i++)			//extract version
			version[i]=read[i+7];
		version[i]=	'\0';
	return atoi(version);
}

/*! \brief Factory reset of the bluetooth module
 * \warning use this function only if you are sure, your e-puck must
 * be restarted, and renamed!!!
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_factory_reset(void)
{
	char send[8];
	char read[10];
	int i;
	char c;
	
	send[0]=0x02;
	send[1]=0x52; 
	send[2]=0x1A;
	send[3]=0x00;
	send[4]=0x00;
	send[5]=0x6c;
	send[6]=0x03;//link number 1-30
	e_send_uart1_char(send,7);
	
	 i=0;
	 c=0;
		do
	    {
	      if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	     		i++;
			}	
	    }
	    while (((char)c != 0x03)||(i<(read[3]+6)));
	    read[i]='\0';
	return read[6];	//return error 0=no error	
}

/*! \brief Change to transparent mode
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_tranparent_mode(void)
{
	char send[8];
	char read[10];
	int i;
	char c;
	
	send[0]=0x02;
	send[1]=0x52; 
	send[2]=0x11;
	send[3]=0x01;
	send[4]=0x00;
	send[5]=0x64;
	send[6]=0x01;//link number 1-30
	send[7]=0x03;
	e_send_uart1_char(send,8);
	
	 i=0;
	 c=0;
		do
	    {
	      if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	     		i++;
			}	
	    }
	    while (((char)c != 0x03)||(i<(read[3]+6)));
	    read[i]='\0';
	return read[6];	//return error 0=no error	
}


/*! \brief Exit from the transparent mode */
void e_bt_exit_tranparent_mode(void)
{
	long i;
	U1MODEbits.UARTEN=0;//disable uart1
	_TRISF3=0;
	_LATF3=0;//uart 1 TX
	for(i=0;i<MILLISEC;i++);//wait at least 1ms
	_LATF3=1;//uart 1 TX
	_TRISF3=1;
	e_init_uart1();	
}

/*! \brief Read the PIN number of this e-puck's bluetooth module
 * \param PIN A pointer to store the PIN number
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_read_local_pin_number(char *PIN)
{
	char send[7];
	unsigned char read[30];
	int i;
	char c;
	
	send[0]=0x02; //send PIN request
	send[1]=0x52;
	send[2]=0x16;
	send[3]=0x00;
	send[4]=0x00;
	send[5]=0x68;
	send[6]=0x03;
	e_send_uart1_char(send,7);
	
	 i=0;
	 c=0;
	
	 do
	    {
	      if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	     		i++;
			}	
	    }
	    while (((char)c != 0x03)||(i<(read[3]+6)));
	    read[i]='\0';
	
		for(i=0;i<read[7];i++)			//extract PIN
			PIN[i]=read[i+8];
		PIN[i]=	'\0';
	return read[6];//return error 0=no error
}

/*! \brief Read the name of this e-puck's bluetooth module
 * \param name A pointer to store the name
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_read_local_name(char *name)
{
	char send[7];
	char read[40];
	int i;
	char c;
	
	send[0]=0x02; 
	send[1]=0x52; 
	send[2]=0x03;//send Name request
	send[3]=0x00;
	send[4]=0x00;
	send[5]=0x55;
	send[6]=0x03;
	e_send_uart1_char(send,7);
	
	 i=0;
	 c=0;
	
	 do
	    {
	      if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	     		i++;
			}	
	    }
	    while (((char)c != 0x03)||(i<(read[3]+6)));
	    read[i]='\0';
	
		for(i=0;i<read[7];i++)			//extract Name
			name[i]=read[i+8];
	
	return read[6];//return error 0=no error
}

/*! \brief Write the PIN number on this e-puck's bluetooth module
 * \param PIN A pointer to store the PIN number
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_write_local_pin_number(char *PIN)
{
	char send[30];
	char read[10];
	int i;
	char c;
	int numberlenght;
	
	numberlenght=strlen(PIN);
	//send_uart2(PIN,numberlenght);
	send[0]=0x02; 
	send[1]=0x52;
	send[2]=0x17;
	send[3]=numberlenght+1;
	send[4]=0x00;
	send[5]=(send[1]+send[2]+send[3]);
	send[6]=numberlenght;
	for (i=0;i<numberlenght;i++)
		send[i+7]=PIN[i];
	send[7+numberlenght]=0x03;
	e_send_uart1_char(send,numberlenght+8);
	
	 i=0;
	 c=0;
	
	 do
	    {
	      if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	     		i++;
			}	
	    }
	    while (((char)c != 0x03)||(i<(read[3]+6)));
	    read[i]='\0';
	return read[6];//return error 0=no error
}

/*! \brief Write the name on this e-puck's bluetooth module
 * \param name A pointer to store the name
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_write_local_name(char *name)
{
	char send[40];
	char read[10];
	int i;
	char c;
	int namelenght;
	
	namelenght=strlen(name);
	namelenght++;//add null caracter
	//send_uart2(PIN,numberlenght);
	send[0]=0x02; //send PIN request
	send[1]=0x52;
	send[2]=0x04;
	send[3]=namelenght+1;
	send[4]=0x00;
	send[5]=(send[1]+send[2]+send[3]);
	send[6]=namelenght;
	for (i=0;i<namelenght;i++)
		send[i+7]=name[i];
	send[7+namelenght]=0x03;
	e_send_uart1_char(send,namelenght+8);
	
	 i=0;
	 c=0;
	
	 do
	    {
	      if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	     		i++;
			}	
	    }
	    while (((char)c != 0x03)||(i<(read[3]+6)));
	    read[i]='\0';
	return read[6];//return error 0=no error
}

/*! \brief Research all the accessible bluetooth devices
 * \param device A pointer to BtDevice to store all the caracteristics
 * of each devices found
 * \return the number of device found
 * \sa BtDevice, e_bt_present_device
 */
int e_bt_inquiry(struct BtDevice *device)
{
	char send[10];
	char read[50];
	int devicefound;
	int i,j;
	char c;
	
	send[0]=0x02; //send Name request
	send[1]=0x52; 
	send[2]=0x00;
	send[3]=0x03;
	send[4]=0x00;
	send[5]=0x55;
	send[6]=0x05;//0x01-0x30 time of inquiri in sec
	send[7]=0x0A;//number of maximu response 0=infinite
	send[8]=0x00;//mode
	send[9]=0x03;
	e_send_uart1_char(send,10);
	devicefound=0;
	 i=j=0;
	 c=0;
		do{
	 		i=0;
	 		do
	    	{
	      		if (e_getchar_uart1(&read[i]))		//read response
				{	
					c=read[i];
	     			i++;
				}	
	    	}
	   		while (((char)c != 0x03)||(i<(read[3]+6)));
	    	if(read[1]==0x69)
	    	{	for(i=0;i<6;i++)			//extract BTaddress
					device[devicefound].address[i]=read[i+6];
				for(i=0;i<3;i++)			//extract BTaddress
					device[devicefound].class[i]=read[i+12];
				devicefound++;	
			}
		}		
	    while((read[1]!=0x43)&(read[2]!=0x00));
	return devicefound;//return number of device found
}

/*! \brief To get the friendly name of a bluetooth device
 * \param device A pointer on the device that you want the name
 * \return bluetooth error if one occur, 0 otherwise
 * \sa BtDevice
 */
char e_bt_get_friendly_name(struct BtDevice *device)
{
	char send[13];
	char read[50];
	
	int i;
	char c;
	
	send[0]=0x02; //send Name request
	send[1]=0x52; 
	send[2]=0x02;
	send[3]=0x06;
	send[4]=0x00;
	send[5]=0x5a;
	send[6]=device[0].address[0];//address of device
	send[7]=device[0].address[1];
	send[8]=device[0].address[2];
	send[9]=device[0].address[3];
	send[10]=device[0].address[4];
	send[11]=device[0].address[5];
	send[12]=0x03;
	e_send_uart1_char(send,13);
	
	i=0;
	c=0;
	do
    {
    	if (e_getchar_uart1(&read[i]))		//read response
		{	
			c=read[i];
     		i++;
		}	
    }
    while (((char)c != 0x03)||(i<(read[3]+6)));
    read[i]='\0';

	if (read[6]==0)
		for(i=0;i<read[13];i++)			//extract Name
			device[0].friendly_name[i]=read[i+14];
	return read[6];	//return error 0=no error
}

/*! \brief Try to connect to another bluetooth device
 * \param address A pointer on the device address which you want
 * to connect
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_etablish_SPP_link(char *address)
{
	char send[15];
	char read[50];
	
	int i;
	char c;
	
	send[0]=0x02; //send Name request
	send[1]=0x52; 
	send[2]=0x0a;
	send[3]=0x08;
	send[4]=0x00;
	send[5]=0x64;
	send[6]=0x01;
	send[7]=address[0];//address of device
	send[8]=address[1];
	send[9]=address[2];
	send[10]=address[3];
	send[11]=address[4];
	send[12]=address[5];
	send[13]=0x01;
	send[14]=0x03;
	e_send_uart1_char(send,15);
	
	i=0;
	c=0;
	do{
 		i=0;
 		do
    	{
      		if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
     			i++;
			}	
    	}
   		while (((char)c != 0x03)||(i<(read[3]+6)));
	}		
	while((read[2]!=0x69)&(read[2]!=0x0B));//spp link etablished
	    
	return read[6];	//return rfcomm error 0=no error	
}

/*! \brief Unconnect from the current bluetooth device
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_release_SPP_link(void)
{
	char send[8];
	char read[10];
	int i;
	char c;
	
	send[0]=0x02;
	send[1]=0x52; 
	send[2]=0x0d;
	send[3]=0x01;
	send[4]=0x00;
	send[5]=0x60;
	send[6]=0x01;//link number 1-30
	send[7]=0x03;
	e_send_uart1_char(send,8);
	
	 i=0;
	 c=0;
	do
    {
      if (e_getchar_uart1(&read[i]))		//read response
		{	
			c=read[i];
     		i++;
		}	
    }
    while (((char)c != 0x03)||(i<(read[3]+6)));
    read[i]='\0';
	if((read[2]==0x0d)&(read[6]!=0x00))//control confirm request (in case no link etablished)
		return read[6];	//return error 0=no error
		
		
	do{
 		i=0;
 		do
    	{
      		if (e_getchar_uart1(&read[i]))//read response
			{	
				c=read[i];
     			i++;
			}	
    	}
   		while (((char)c != 0x03)||(i<(read[3]+6)));
	}		
	while((read[1]!=0x69)&(read[2]!=0x051));//spp link released
	    
	return read[6];	//return rfcomm error 0=no error	
}

/*! \brief Send data to the current bluetooth device
 * \warning send maximum 127 bytes if you are in non transparent mode
 * \param data The datas to send
 * \param datalength The length of the datas to send
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_send_SPP_data(char *data, char datalength)
{
	char send[120];
	char read[10];
	int i;
	char c;
	
	//send_uart2(PIN,numberlenght);
	send[0]=0x02; //send PIN request
	send[1]=0x52;
	send[2]=0x0F;
	send[3]=datalength+3;
	send[4]=0x00;
	send[5]=(send[1]+send[2]+send[3]);
	send[6]=0x01;//local port
	send[7]=datalength;
	send[8]=0x00;
	for (i=0;i<datalength;i++)
		send[i+9]=data[i];
	send[9+datalength]=0x03;
	e_send_uart1_char(send,datalength+10);
	
	 i=0;
	 c=0;
	
	 do
	    {
	      if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	     		i++;
			}	
	    }
	    while (((char)c != 0x03)||(i<(read[3]+6)));
	    read[i]='\0';
	return read[6];//return error 0=no error
}

/*! \brief Make a list of the bluetooth address of paired device
 * \return The number of device found
 * \sa e_bt_local_paired_device
 */
char e_bt_list_local_paired_device(void)
{
	char send[7];
	char read[(8*6)+10];
	int devicefound;
	int i;
	char c;
	
	send[0]=0x02; 
	send[1]=0x52; 
	send[2]=0x1c;
	send[3]=0x00;
	send[4]=0x00;
	send[5]=0x6e;
	send[6]=0x03;
	e_send_uart1_char(send,7);
	devicefound=0;
	 i=0;
	 c=0;
		do
	   	{
	     	if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	    			i++;
			}	
	   	}
	  	while (((char)c != 0x03)||(i<(read[3]+6)));
	   	devicefound=read[7];
	   	if(devicefound!=0)
	   	{
		   	for(i=0;i<6*devicefound;i++)			//extract BTaddress
				e_bt_local_paired_device[i]=read[i+8];
		}
	
	return devicefound;//return number of device found
}

/*! \brief Remove a paired bluetooth device
 * \param j The number of the paired device to remove from the
 * e_bt_local_paired_device array.
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_remove_local_paired_device(int j)
{
	char send[13];
	char read[10];
	int i;
	char c;
	
	send[0]=0x02; //send Name request
	send[1]=0x52; 
	send[2]=0x1b;
	send[3]=0x06;
	send[4]=0x00;
	send[5]=0x73;
	send[6]=e_bt_local_paired_device[0+j*6];//address of device
	send[7]=e_bt_local_paired_device[1+j*6];
	send[8]=e_bt_local_paired_device[2+j*6];
	send[9]=e_bt_local_paired_device[3+j*6];
	send[10]=e_bt_local_paired_device[4+j*6];
	send[11]=e_bt_local_paired_device[5+j*6];
	send[12]=0x03;
	e_send_uart1_char(send,13);
	
	 i=0;
	 c=0;
		do
	    {
	      if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	     		i++;
			}	
	    }
	    while (((char)c != 0x03)||(i<(read[3]+6)));
	    read[i]='\0';
	return read[6];	//return error 0=no error
}


/*! \brief Set event filter of module
 * \warning this function can change the Bluetooth behaviours and than this library could not work anymore. 
 * \param event 0x00: All events reported
 * 0x01: No ACL Link Indicators (default)
 * 0x02: No events reported
 * 0x03: No events generated and no UART break generated when device leaves transparent mode
 * \return bluetooth error if one occur, 0 otherwise
 */
char e_bt_set_event_filter(char event)
{
	char send[8];
	char read[10];
	int i;
	char c;
	
	send[0]=0x02;
	send[1]=0x52; //request
	send[2]=0x4E; //opcode set event
	send[3]=0x01; //datalenth
	send[4]=0x00;
	send[5]=0xA1; //checksum
	send[6]=event;//link number 1-30
 	send[7]=0x03; //end
	e_send_uart1_char(send,8);
	
	i=0;
	c=0;
	if(event>1)
		return 0;

	do
    {
      if (e_getchar_uart1(&read[i]))		//read response
		{	
			c=read[i];
     		i++;
		}	
    }
    while (((char)c != 0x03)||(i<(read[3]+6)));
    read[i]='\0';
	return read[6];	//return error 0=no error	
}

/*! \brief Get event filter of module if it is different than 0x02
 * 
 * \return event filter 
 *	0x00: All events reported
 * 	0x01: No ACL Link Indicators (default)
 * 	0x02: No events reported
 */
char e_bt_get_event_filter(void)
{
	char send[8];
	char read[10];
	int i;
	char c;
	
	send[0]=0x02;
	send[1]=0x52; //request
	send[2]=0x4F; //opcode get event
	send[3]=0x00; //datalenth
	send[4]=0x00;
	send[5]=0xA1; //checksum
	send[6]=0x03; //end
	e_send_uart1_char(send,7);
	
	 i=0;
	 c=0;
		do
	    {
	      if (e_getchar_uart1(&read[i]))		//read response
			{	
				c=read[i];
	     		i++;
			}	
	    }
	    while (((char)c != 0x03)||(i<(read[3]+6)));
	    read[i]='\0';
	return read[6];
}
