/*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            	Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Alvaro Gutierrez		09/02/2007	  First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES: 
* Code For the Epuck Range and Bearing Board
**********************************************************************/


//Functions prototype
void init_I2C( void );
void __attribute__((interrupt,no_auto_psv)) _SI2C1Interrupt( void );
char read_register(char reg_add);
void write_register(char reg_add, unsigned char value);
