#ifndef mcp2515_settings_H
#define mcp2515_settings_H

#ifndef CAN_BAUDRATE
#define CAN_BAUDRATE 250K //default baudrate 250k
#endif

//see http://www.kvaser.com/support/calculators/bit-timing-calculator/ for calculating baudrate.
//im not sure how they work exactly

#if CAN_BAUDRATE==125K
#define CNF1_Setting  
#define CNF2_Setting  
#define CNF3_Setting  
#endif

#if CAN_BAUDRATE==250K
#define CNF1_Setting  
#define CNF2_Setting  
#define CNF3_Setting  
#endif

#if CAN_BAUDRATE==500K
#define CNF1_Setting  
#define CNF2_Setting  
#define CNF3_Setting  
#endif


#define CANINTE_Setting (( 1 << RX1IE ) | ( 1 << RX0IE ))
//interrupt when either receive buffer is full

#define RXB0CTRL_Setting (( 1 << RXM1 ) | ( 1 << RXM0 )|(1 << BUKT))
//all receive masks/filters disabled buffer 0
//message will rollover to buffer 1 if buffer 0 is full

#define RXB1CTRL_Setting (( 1 << RXM1 ) | ( 1 << RXM0 ))
//all receive mask/filters disabled for buffer 1
 
#define BFPCTRL_Setting ((1 << B0BFE)|(1 << B1BFE))
//RXnBF pins set as standard ouputs

#define TXRTSCTRL_Setting 0
//TXnRTS pins set as general inputs

#define CANCTRL_Setting 0
//normal mode

#endif
