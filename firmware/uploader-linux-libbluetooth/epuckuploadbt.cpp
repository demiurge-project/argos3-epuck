/*
	epuckuploadbt.cpp
	
	libbluetooth based uploader for e-puck
	(c) 2007 Stephane Magnenat (stephane at magnenat dot net) - Mobots group - LSRO1 - EPFL
	
	Based on:
	-	epuckupload.c
		Created by Olivier Michel on 12/15/06.
		(inpired from epuckupload perl script by Xavier Raemy and Thomas Lochmatter)
		Copyright 2006 Cyberbotics Ltd. All rights reserved.
		
	-	libbluetooth samples from Internet
*/

#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

const unsigned programVersion = 2;
const char *programName = "epuck bluetooth uploader (using libbluetooth)";
const char *copyrightInfo = "(c) 2006 - 2007 Olivier Michel, Stephane Magnenat";

typedef struct MemoryChunkStruct
{ /* size is 64KB = 0x10000 bytes */
  char order;
  unsigned char *data;
  int address;
  int len;
  struct MemoryChunkStruct *next;
} MemoryChunk;

typedef struct PacketStruct
{
  unsigned char data[101]; // 4+32*3+1
  struct PacketStruct *next;
} Packet;

MemoryChunk *memory_chunk=NULL;
Packet *packet=NULL;

MemoryChunk *get_memory_chunk(int address,char order)
{
	MemoryChunk *m;
	
	if (memory_chunk)
	{
		for(m=memory_chunk;m->next;m=m->next)
			if (address==m->address)
				return m;
		if (address==m->address)
			return m;
		m->next=(MemoryChunk *)malloc(sizeof(MemoryChunk));
		m=m->next;
	}
	else
	{
		memory_chunk=(MemoryChunk *)malloc(sizeof(MemoryChunk));
		m=memory_chunk;
	}
	m->address=address;
	m->data=(unsigned char *)malloc(0x10010); // the hex file sometimes goes over 0xffff addresses with a maximum of 0x10
	memset(m->data,0xff,0x10010);
	m->len=0;
	m->order=0;
	m->next=NULL;
	return m;
}

/* Sets the bootloader return address */
void entrypoint_startsequence(unsigned char *v)
{
	char old_value[3];
	MemoryChunk *m=get_memory_chunk(0,2);
	old_value[0]=m->data[0];
	old_value[1]=m->data[1];
	old_value[2]=m->data[4];
		
	m->data[0]=v[0];
	m->data[1]=v[1];
	m->data[2]=0x04;
	m->data[3]=0x00;
	m->data[4]=v[2];
	m->data[5]=0x00;
	m->data[6]=0x00;
	m->data[7]=0x00;
	
	v[0]=old_value[0];
	v[1]=old_value[1];
	v[2]=old_value[2];
}

/* Sets the bootloader return address */
void entrypoint_returnaddress(int bootloader,unsigned char *address,int robot_id)
{
	MemoryChunk *m=get_memory_chunk(bootloader,1);
	unsigned char *data=m->data;
	data[108]=robot_id & 0xff;
	data[109]=(robot_id>>8) & 0xff;
	data[110]=0x00;
	data[111]=0x00;
	data[112]=address[0];
	data[113]=address[1];
	data[114]=0x04;
	data[115]=0x00;
	data[116]=address[2];
	data[117]=0x00;
	data[118]=0x00;
	data[119]=0x00;
	if (m->len<120)
		m->len=120;
}

/* Writes the memory chunks in hexadecimal representation to a file */
void writefilehex(const char *filename)
{
	FILE *fd=fopen(filename,"w");
	MemoryChunk *m,**m_array;
	int i,j,n=0;
	if (fd==NULL) return;
	/* sort the memory chunk by order in m_array */
	for(m=memory_chunk;m;m=m->next) n++;
	m_array = (MemoryChunk **)malloc(n*sizeof(MemoryChunk *));
	i=0;
	for(j=0;j<3;j++)
		for(m=memory_chunk;m;m=m->next)
			if (m->order==j)
				m_array[i++]=m;
	
	for(i=0;i<n;i++)
	{
		m = m_array[i];
		fprintf(fd,"Base address: %d (length: %d bytes)",m->address,m->len);
		for(j=0;j<m->len;j++)
		{
			if (j % 16==0)
				fprintf(fd,"\n");
			fprintf(fd,"%02X ",m->data[j]);
		}
		fprintf(fd,"\n");
	}
	fclose(fd);
	free(m_array);
}

int readfileINHX32(const char *filename)
{
	char line[64];
	char hex[5];
	int i,j,k,value,line_counter;
	unsigned char len;
	unsigned short int address;
	unsigned char type;
	unsigned char checksum;
	MemoryChunk *m=NULL;
	FILE *fd=fopen(filename,"r");
	if (fd==NULL)
		return -1;
	line_counter=0;
	while (fscanf(fd,"%64s",line)==1)
	{
		line_counter++;
		i=1;
		hex[0]=line[i++];
		hex[1]=line[i++];
		hex[2]='\0';
		sscanf(hex,"%x",&value);
		len=(unsigned char)value;
		hex[0]=line[i++];
		hex[1]=line[i++];
		hex[2]=line[i++];
		hex[3]=line[i++];
		hex[4]='\0';
		sscanf(hex,"%x",&value);
		address=(unsigned short int)value;
		hex[0]=line[i++];
		hex[1]=line[i++];
		hex[2]='\0';
		sscanf(hex,"%x",&value);
		type=(unsigned char)value;
		j=i; /* start of data chunk */
		k=strlen(&line[j])/2-1;
		if (k!=len)
		{
			fprintf(stderr,"wrong data length on line %d, ignored",line_counter);
			continue;
		}
		i+=2*len;
		hex[0]=line[i++];
		hex[1]=line[i--]; // return to previous position with i
		// 0 already set for hex[2]
		sscanf(hex,"%x",&value);
		checksum=(unsigned char)value;
		// printf("%s (len=%d, address=%x, type=%d checksum=%x)\n",line,len,address,type,checksum);
		if (type==0 && m!=NULL)
		{
			for(k=0;k<len;k++)
			{
				hex[0]=line[j++];
				hex[1]=line[j++];
				// 0 already set for hex[2]
				sscanf(hex,"%x",&value);
				if (address+k >= 0x10010)
				{
					std::cerr << "Error, a single line of the hex file runs past the 64 KB boundary of a chunk, which is not supported, aborting." << std::endl;
					exit(1);
				}
				m->data[address+k]=(unsigned char)value;
			}
			if (m->len<address+len)
				m->len=address+len;
		}
		else if (type==4)
		{
			hex[0]=line[j++];
			hex[1]=line[j++];
			hex[2]=line[j++];
			hex[3]=line[j++];
			hex[4]='\0';
			sscanf(hex,"%x",&value);
			if (value<4)
			{
				value*=0x10000;
				m = get_memory_chunk(value,0);
			}
			else
				m=NULL;
		}
	}
	fclose(fd);
	return 1;
}

/* Prepares the packets to be transmitted */
void create_packets()
{
	MemoryChunk *m,**m_array;
	Packet *p=NULL;
	int addr,i,j,k,n=0,writeaddress,sum;
	/* sort the memory chunk by order in m_array */
	for(m=memory_chunk;m;m=m->next) n++;
	m_array = (MemoryChunk **)malloc(n*sizeof(MemoryChunk *));
	i=0;
	for(j=0;j<3;j++)
		for(m=memory_chunk;m;m=m->next)
			if (m->order==j)
				m_array[i++]=m;
	for(i=0;i<n;i++)
	{
		addr=0;
		while(addr<m_array[i]->len)
		{
			if (p==NULL)
			{
				p = (Packet *)malloc(sizeof(Packet));
				packet=p;
			}
			else
			{
				p->next = (Packet *)malloc(sizeof(Packet));
				p=p->next;
			}
			writeaddress=(addr+m_array[i]->address)/2;
			j=0;
			sum=0;
			p->data[j]= writeaddress      & 0xff; sum+=p->data[j++];
			p->data[j]=(writeaddress>>8)  & 0xff; sum+=p->data[j++];
			p->data[j]=(writeaddress>>16) & 0xff; sum+=p->data[j++];
			p->data[j]=0x60;                      sum+=p->data[j++];
			for(k=0;k<32;k++)
			{
				if (addr < m_array[i]->len)
				{
					p->data[j]=m_array[i]->data[addr++]; sum+=p->data[j++];
					p->data[j]=m_array[i]->data[addr++]; sum+=p->data[j++];
					p->data[j]=m_array[i]->data[addr++]; sum+=p->data[j++];
				}
				else
				{
					p->data[j++]=0xff;
					p->data[j++]=0xff;
					p->data[j++]=0xff;
					sum+=3*0xff;
				}
				addr++;
			}
			sum &= 0xff;
			p->data[j++]=((sum==0)?0:256-sum); // check sum
		}
		p->next=NULL; // last packet
	}
	free(m_array);
}

/* Writes all packets to a file */
void write_packets(const char *file)
{
	int i;
	Packet *p;
	FILE *fd=fopen(file,"w");
	if (fd==NULL)
		return;
	for(p=packet;p;p=p->next)
	{
		for(i=0;i<100;i++)
			fprintf(fd,"%02X ",p->data[i]);
		fprintf(fd,"\n");
	}
	fclose(fd);
}

/* Send packets to file descriptor fd */
bool send_packets(int fd)
{
	// send reset signal: R, ignore answered chars
	// send bootloader start char: 0xC1
	// receive 'q' and 'K' answer
	// send packet
	// receive 'K' answer
	// ...
	// send packet
	// receive 'K' answer
	// send termination: 0x95, 0x00, 0x00, 0xFF
	char buffer[64];
	
	// eat input
	int retval;
	while (1)
	{
		struct timeval timeout;
		timeout.tv_sec=0;
		timeout.tv_usec=0;
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(fd,&readfds);
		retval = select(fd+1,&readfds,NULL,NULL,&timeout);
		if (retval != 0)
			read(fd,buffer,64);
		else
			break;
	}
	
	// wait user input
	std::cout << "Press enter and then reset on the robot" << std::endl;
	getchar();
	
	// wait for 50 times 100 ms
	bool found = false;
	buffer[0]=0xC1;
	for (unsigned i = 0; i < 50; i++)
	{
		struct timeval timeout;
		timeout.tv_sec=0;  // 100 ms timeout
		timeout.tv_usec=100000; // (to leave time to establish the connection)
		
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(fd,&readfds);
		
		write(fd,buffer,1);
		retval = select(fd+1,&readfds,NULL,NULL,&timeout);
		
		if (retval!=0)
		{
			found = true;
			break;
		}
	}
	
	// if no answer, quit
	if (!found)
	{
		std::cerr << "\rError, bootloader did not answered" << std::endl;
		return false;
	}
	
	// checking ack
	int n = read(fd,buffer,64);
	bool ok = false;
	for(int i=0;i<n-1;i++)
		if (buffer[i]=='q' && buffer[i+1]=='K')
			ok = true;
	if (!ok)
	{
		std::cerr << "\rError, no ack received from bootloader" << std::endl;
		return false; // fail, we didn't receive the ack from bootloader
	}
	
	// uploading code
	ok = true;
	printf("\rUploading: ");
	for(Packet *p=packet;p;p=p->next)
	{
		write(fd,p->data,101);
		n=read(fd,buffer,64);
		switch(buffer[0])
		{
			case 'K': printf("#"); break; // all right
			case 'N': printf("!"); ok = false; break; // checksum error
			default:  printf("?"); ok = false; break; // unknown error
		}
		fflush(stdout);
	}
	buffer[0]=0x95;
	buffer[1]=0x00;
	buffer[2]=0x00;
	buffer[3]=0xff;
	write(fd,buffer,4);
	return ok;
}

bool connectToEPuck(const bdaddr_t *ba)
{
	// set the connection parameters (who to connect to)
	struct sockaddr_rc addr;
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 1;
	addr.rc_bdaddr = *ba;
	//memcpy(addr.rc_bdaddr, ba, sizeof(bdaddr_t));
	
	// allocate a socket
	int rfcommSock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	
	// connect to server
	int status = ::connect(rfcommSock, (struct sockaddr *)&addr, sizeof(addr));
	
	if (status == 0)
	{
		unsigned trial = 0;
		while (true)
		{
			if (send_packets(rfcommSock))
				break;
			else
				std::cerr << "Upload failed (" << trial << ") . Trying again. Press CTRL-C to quit" << std::endl;
			trial++;
		}
	}
	else
	{
		std::cerr << "Error, can't connect to rfcomm socket" << std::endl;
	}
	
	close(rfcommSock);
	
	return status == 0;
}

bool scanBluetoothAndUpload(int robotId)
{
	// open device
	int devId = hci_get_route(NULL);
	if (devId < 0)
	{
		std::cerr << "Error, can't get bluetooth adapter ID" << std::endl;
		return false;
	}
	
	// open socket
	int sock = hci_open_dev(devId);
	if (sock < 0)
	{
		std::cerr << "Error,can't open bluetooth adapter" << std::endl;
		return false;
	}

	// query
	std::cout << "Scanning bluetooth:" << std::endl;
	//int length  = 8; /* ~10 seconds */
	int length  = 4; /* ~5 seconds */
	inquiry_info *info = NULL;
	// device id, query length (last 1.28 * length seconds), max devices, lap ??, returned array, flag
	int devicesCount = hci_inquiry(devId, length, 255, NULL, &info, 0);
	if (devicesCount < 0)
	{
		std::cerr << "Error, can't query bluetooth" << std::endl;
		close(sock);
		return false;
	}
	
	// print devices
	bool found = false;
	for (int i = 0; i < devicesCount; i++)
	{
		char addrString[19];
		char addrFriendlyName[256];
		ba2str(&(info+i)->bdaddr, addrString);
		if (hci_read_remote_name(sock, &(info+i)->bdaddr, 256, addrFriendlyName, 0) < 0)
			strcpy(addrFriendlyName, "[unknown]");
		printf("\t%s %s\n", addrString, addrFriendlyName);
		if (strncmp("e-puck_", addrFriendlyName, 7) == 0)
		{
			int id;
			sscanf(addrFriendlyName + 7, "%d", &id);
			if (sscanf(addrFriendlyName + 7, "%d", &id) && (id == robotId))
			{
				std::cout << "Contacting e-puck " << id << std::endl;
				found = connectToEPuck(&(info+i)->bdaddr);
				break;
			}
		}
	}
	
	if (!found)
		std::cerr << "Error: e-puck " << robotId << " not found" << std::endl;
	
	free(info);
	close(sock);
	
	return found;
}

int main(int argc,char *argv[])
{
	unsigned char program_address[]={0x40,0x7f,0x01};
	int robot_id=0;
	
	std::cout << programName << " version " << programVersion << "\n" << copyrightInfo << std::endl;
	if (argc!=3)
	{
		std::cerr << "Error, wrong number of arguments, usage:\n" << argv[0] << " FILE ROBOT_ID" << std::endl;
		return 1;
	}
	if (readfileINHX32(argv[1])==-1)
	{
		std::cerr << "Error, cannot open file: " << argv[1] << std::endl;
		return 2;
	}
	sscanf(argv[2],"%d",&robot_id);
	entrypoint_startsequence(program_address);
	entrypoint_returnaddress(0x17f00*2,program_address,robot_id);
	//writefilehex("epuckupload.memory_new");
	create_packets();
	//write_packets("epuckupload.packets_new");
	if (scanBluetoothAndUpload(robot_id))
	{
		std::cout << "\nUpload success :-)" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "\nUpload failure :-(" << std::endl;
		return 3;
	}
}
