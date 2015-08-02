#ifndef MCP2515_LIB_H
#define MCP2515_LIB_H

//CAN CS pin on port B
#define DDR_CAN_CS DDRB
#define PORT_CAN_CS PORTB
//CAN CS pin is pin b2
#define P_CAN_CS 2

//define a macro to make turning CAN CS on and off a bit less typing
#define CAN_CS_HIGH PORT_CAN_CS |= (1 << P_CAN_CS);
#define CAN_CS_LOW PORT_CAN_CS &= ~(1 << P_CAN_CS);

//this function can modify single bits in a register
//can only be used on certain registers
//ex mcp2515_bit_modify(CANINTF,(1 << RX0IF),0) clears receive buffer 0 full interrupt flag
void mcp2515_bit_modify ( unsigned char address, unsigned char mask, unsigned char data );

//this function returns the value of a register
//ex "int val = mcp2515_read_register(CANINTF);" reads interrupt flags
unsigned char mcp2515_read_register(unsigned char adress);

//sets up the mcp2515
//requires spi to already be started (spi_init())
unsigned int mcp2515_init(char mode_select);
//mcp2515_init() can set the chip to different modes defined here
#define normal 0

//writes to a register
void mcp2515_write_register( unsigned char address, unsigned char data ) ;

// a few macros for using the general io pins
#define read_TXnRTS_pins (7 & (mcp2515_read_register(TXRTSCTRL) >> 3))
#define RXnBF_pins_on mcp2515_write_register(BFPCTRL,BFPCTRL_Setting|(1<<B1BFS)|(1<<B0BFS));
#define RXnBF_pins_off mcp2515_write_register(BFPCTRL,BFPCTRL_Setting);

//structure for building can messages to send to can_send_message()
typedef struct 
{ 
    unsigned int   id = 0;
    unsigned char    RTransR = 0; //defaults to not a remote transmit request
    unsigned char    length = 0;
    unsigned char    data[8] = [0,0,0,0,0,0,0,0];
} CanMessage; 

//sends can message
unsigned char can_send_message ( CanMessage *p_message );
//receive can message
CanMessage can_get_message ( void ) ;

#endif
