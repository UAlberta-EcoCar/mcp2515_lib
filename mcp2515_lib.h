#ifndef MCP2515_LIB_H
#define MCP2515_LIB_H



#define DDR_CAN_CS DDRB
#define PORT_CAN_CS PORTB
#define P_CAN_CS 2

#define CAN_CS_HIGH PORT_CAN_CS |= (1 << P_CAN_CS);
#define CAN_CS_LOW PORT_CAN_CS &= ~(1 << P_CAN_CS);

void mcp2515_bit_modify ( unsigned char address, unsigned char mask, unsigned char data );
unsigned char mcp2515_read_register(unsigned char adress);
unsigned int mcp2515_init(void);
void mcp2515_write_register( unsigned char address, unsigned char data ) ;


#define read_TXnRTS_pins (7 & (mcp2515_read_register(TXRTSCTRL) >> 3))
#define RXnBF_pins_on mcp2515_write_register(BFPCTRL,BFPCTRL_Setting|(1<<B1BFS)|(1<<B0BFS));
#define RXnBF_pins_off mcp2515_write_register(BFPCTRL,BFPCTRL_Setting);

typedef struct 
{ 
    unsigned int   id = 0;
    unsigned char    RTransR = 0; //defaults to not a remote transmit request
    unsigned char    length;
    unsigned char    data [ 7 ] ;
} CanMessage; 

unsigned char can_send_message ( CanMessage *p_message );
CanMessage can_get_message ( void ) ;


#endif
