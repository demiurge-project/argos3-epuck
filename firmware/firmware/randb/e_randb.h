/********************************************************************************
 *
 *       Range and Bearing board of e-puck
 *       Version 1.0 January 2008
 *
 ********************************************************************************/


#ifndef _ERANDB
#define _ERANDB

#define RAB_ID 0x40

#define ON_BOARD 	0
#define ON_ROBOT 	1

#define FALSE 0
#define TRUE 1

#define WAITING 0
#define RECEIVING 1

#define I2C 0
#define UART 1

typedef struct{
	unsigned char received;
	unsigned char alpha;
	unsigned int  data;
	unsigned int distance;
}randbReceptionStruct;

typedef struct {
	unsigned int data;
	float bearing;
	unsigned int range;
	unsigned int max_peak;
	unsigned char max_sensor;
} finalDataRegister;


extern int calcOnBoard;

/************************* COMMON FUNCTIONS*********************************/

/****************************************************************************
 * Function: void  e_init_randb ( unsigned char mode );
 * Description: Init the e-RandB board 
 * Inputs
 * 	- unsigned char mode: 
 * 		Defines if the communication is done with the I2C ( mode = I2C )
 * 		or the UART ( mode = UART )
 * Return: void 
 ***************************************************************************/

void e_init_randb ( unsigned char mode );

/****************************************************************************/
/***************************** I2C Functions ********************************/
/****************************************************************************/

/****************************************************************************
 * Function: 		void  e_randb_get_data( void );
 * Description:	If a frame has been received returns data 
 * Inputs:			void
 * Return: 			unsigned int data	
 ***************************************************************************/

unsigned int  e_randb_get_data( void );

/****************************************************************************
 * Function: 		void  e_randb_get_range( void );
 * Description:	If a frame has been received returns range
 * Inputs:			void
 * Return: 			unsigned int range 
 ***************************************************************************/

unsigned int e_randb_get_range( void );

/****************************************************************************
 * Function: 		void  e_randb_get_bearing( void );
 * Description:	If a frame has been received returns bearing
 * Inputs:			void
 * Return: 			double bearing (in radians)
 ***************************************************************************/

double e_randb_get_bearing( void );


/****************************************************************************
 * Function: 		void  e_randb_get_sensor( void );
 * Description:	If a frame has been received returns the sensor 
 * 							with maximum power received
 * Inputs:			void
 * Return: 			unsigned char max sensor
 ***************************************************************************/

unsigned char e_randb_get_sensor ( void );

/****************************************************************************
 * Function: 		void  e_randb_get_if_received ( void );
 * Description:	Checks if data has been received on the board
 * Inputs:			void
 * Return: 			0 if nothing received, 1 if a frame has been received 
 ***************************************************************************/

unsigned char e_randb_get_if_received( void );

/****************************************************************************
 * Function: 		void  e_randb_set_range ( unsigned char distance );
 * Description: Sets the range of transmission
 * Inputs:
 * 							- unsigned char distance: 
 * 								0 --> Full Range (1m. aprox., depending on light conditions)
 * 								255 --> Shortest Range (0cm aprox., depending on light conditions )
 * Return: 			void 
 ***************************************************************************/

void e_randb_set_range ( unsigned char distance);

/****************************************************************************
 * Function: 		void  e_randb_set_calculation ( unsigned char value );
 * Description: At some point we tought that the board could just take
 *     					data en leave the calculations for the robot.
 *       				At the moment, it is better to allow the board 
 *       				to do the calculations 
 * Inputs:
 * 							- unsigned char value: 
 * 									ON_BOARD -> The board makes the calculations of the r&b
 * 									ON_ROBOT -> The robot makes the calculations of the r&b
 * Return: 			void 
 ***************************************************************************/

void e_randb_set_calculation ( unsigned char value );

/****************************************************************************
 * Function: 		void  e_randb_store_light_conditions ( void );
 * Description: Store light conditions to use them as offset
 * 							for the calculations of the range and bearing
 * Inputs: 			void
 * Return: 			void 
 ***************************************************************************/
void e_randb_store_light_conditions ( void );

/****************************************************************************
 * Function: 		void  e_randb_store_data ( unsigned char channel, unsigned int data );
 * Description: Tells the board to store some data "data" to be sent later
 * 							by sensor "channel"
 * Inputs: 
 * 							- unsigned char channel: The sensor which will send the data 
 * 							- unsigned int data: The data to be sent by the sensor
 * Return: 			void 
 ***************************************************************************/

void e_randb_store_data ( unsigned char channel , unsigned int data );

/****************************************************************************
 * Function: 		void  e_randb_send_data ( void );
 * Description: Tells the board to send the data stored previouslly with 
 * 							e_randb_store_data
 * Inputs:			void 
 * Return: 			void 
 ***************************************************************************/

void e_randb_send_data ( void );

/****************************************************************************
 * Function: 		void  e_randb_send_all_data ( unsigned int data );
 * Description: Tells the board to send the data "data" through all the sensors
 * Inputs:			
 * 							- unsigned int data: The data to be sent by all the sensors
 * Return: 			void 
 ***************************************************************************/

void e_randb_send_all_data( unsigned int data );


/****************************************************************************
 * Function: 		void  e_randb_reception( finalDataRegister* data );
 * Description:	If data received returns struct with the data, range, bearing
 *							and max_sensor of the frame received.
 * Inputs:			
 * 							- finalDataRegister* data: Pointer to a struct where the data 
 * 							received will be loaded 
 * Return: 			0 if nothing received
 * 							1 if a frame was received
 ***************************************************************************/
unsigned char e_randb_reception( finalDataRegister* data );




/****************************************************************************/
/***************************** UART Functions *******************************/
/****************************************************************************/

/****************************************************************************
 * Function:		void  e_randb_set_uart_communication ( unsigned char mode );
 * Description: Tells the Board Receptios are send through the UART 
 * Inputs:
 * 							- unsigned char mode: 
 * 									Asks the board to send data throrugh the I2C ( mode = I2C )
 * 									or the UART ( mode = UART )
 * Return: 			void 
 ***************************************************************************/

void e_randb_set_uart_communication ( unsigned char mode);

/****************************************************************************
 * Function: 		void  e_randb_uart_set_range ( unsigned char distance );
 * Description: Sets the range of transmission
 * Inputs:
 * 							- unsigned char distance: 
 * 								0 --> Full Range (1m. aprox., depending on light conditions)
 * 								255 --> Shortest Range (0cm aprox., depending on light conditions )
 * Return: 			void 
 ***************************************************************************/

void e_randb_uart_set_range ( unsigned char distance);

/****************************************************************************
 * Function: 		void  e_randb_uart_set_calculation ( unsigned char value );
 * Description: At some point we tought that the board could just take
 *     					data en leave the calculations for the robot.
 *       				At the moment, it is better to allow the board 
 *       				to do the calculations 
 * Inputs:
 * 							- unsigned char value: 
 * 									ON_BOARD -> The board makes the calculations of the r&b
 * 									ON_ROBOT -> The robot makes the calculations of the r&b
 * Return: 			void 
 ***************************************************************************/

void e_randb_uart_set_calculation ( unsigned char value );

/****************************************************************************
 * Function: 		void  e_randb_uart_store_light_conditions ( void );
 * Description: Store light conditions to use them as offset
 * 							for the calculations of the range and bearing
 * Inputs: 			void
 * Return: 			void 
 ***************************************************************************/

void e_randb_uart_store_light_conditions ( void );

/****************************************************************************
 * Function: 		void  e_randb_uart_store_data ( unsigned char channel, unsigned int data );
 * Description: Tells the board to store some data "data" to be sent later
 * 							by sensor "channel"
 * Inputs: 
 * 							- unsigned char channel: The sensor which will send the data 
 * 							- unsigned int data: The data to be sent by the sensor
 * Return: 			void 
 ***************************************************************************/

void e_randb_uart_store_data ( unsigned char channel , unsigned int data );

/****************************************************************************
 * Function: 		void  e_randb_uart_send_data ( void );
 * Description: Tells the board to send the data stored previouslly with 
 * 							e_randb_uart_store_data
 * Inputs:			void 
 * Return: 			void 
 ***************************************************************************/
void e_randb_uart_send_data ( void );

/****************************************************************************
 * Function: 		void  e_randb_uart_send_all_data ( unsigned int data );
 * Description: Tells the board to send the data "data" through all the sensors
 * Inputs:			
 * 							- unsigned int data: The data to be sent by the sensor
 * Return: 			void 
 ***************************************************************************/

void e_randb_uart_send_all_data( unsigned int data );

/****************************************************************************
 * Function: 		void  e_randb_get_data_uart( finalDataRegister* data );
 * Description:	If data received returns struct with the data, range, bearing, 
 *							max_peak and max_sensor of the frame received.
 * Inputs:			
 * 							- finalDataRegister* data: Pointer to a struct where the data 
 * 							received will be loaded 
 * Return: 			0 if nothing received
 * 							1 if a frame was received
 ***************************************************************************/

unsigned char e_randb_get_data_uart ( finalDataRegister* data);

/****************************************************************************
 * Function: 		void  e_randb_get_uart2 ( void )
 * Description:	State machine incharged of getting the information 
 * 							from the baord through UART interruptions.
 * 							It is started and managed by the agenda
 * Inputs:			void 	
 * Return: 			void 
 ***************************************************************************/
void e_randb_get_uart2 ( void );



/**********************************************************************************/
/***************** CALC ON ROBOT FUNCTIONS********************************************/
/**********************************************************************************/


/* Get All the peaks and data values received by board */
unsigned int e_randb_get_all_data ( unsigned int *peaks, unsigned int *data);

/* Get All the peaks and data values received by board and calcs reange and bearing*/
int e_randb_all_reception( unsigned int *range, float *bearing, unsigned int *data );
#endif
