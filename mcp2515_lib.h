#ifndef mcp2515_can_lib_H
#define mcp2515_can_lib_H

#define DDR_CAN_CS DDRB
#define PORT_CAN_CS PORTB
#define P_CAN_CS 2

#define CAN_CS_HIGH PORT_CAN_CS |= (1 << P_CAN_CS);
#define CAN_CS_LOW PORT_CAN_CS &= ~(1 << P_CAN_CS);

void mcp2515_bit_modify ( unsigned char address, unsigned char mask, unsigned char data );
unsigned char mcp2515_read_register(unsigned char adress);
unsigned char mcp2515_init(void);

#define read_TXnRTS_pins (7 & (mcp2515_read_register(TXRTSCTRL) >> 3))

typedef struct 
{ 
    uint16_t   id;
    uint8_t    RTR;
    uint8_t    length;
    uint8_t    data [ 8 ] ;
    } CanMessage; 


#endif
