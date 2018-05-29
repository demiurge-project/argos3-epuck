#include "DataLength.h"
#include "PWMEmission.h"

int dataLength;

void init_data_length(){
	dataLength = 16; //default value
}
void set_data_length(int length){
	if(length != 8 && length != 16 && length != 24 && length != 32){
		// We keep the previous valid value
		return;
	}
	dataLength = length;
	re_init_queue_emission();
}
int get_data_length(){
	return dataLength;
}