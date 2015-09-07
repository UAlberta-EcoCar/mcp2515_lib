#ifndef mcp2515_settings_H
#define mcp2515_settings_H

//baudrate defaults to 250000
//or you can set it to one of the values below
#ifndef CAN_BAUDRATE
#define CAN_BAUDRATE 250000
#endif

//see http://www.kvaser.com/support/calculators/bit-timing-calculator/ for calculating bitrate.
//I'm not sure how they work exactly

//the following assume 16MHz clock. 
#if CAN_BAUDRATE==125000
#define CNF1_Setting  0x03
#define CNF2_Setting  0xF0
#define CNF3_Setting  0x86
#endif

#if CAN_BAUDRATE==250000
#define CNF1_Setting  0x41
#define CNF2_Setting  0xF1
#define CNF3_Setting  0x85
#endif

#if CAN_BAUDRATE==500000
#define CNF1_Setting  0
#define CNF2_Setting  0xF0
#define CNF3_Setting  0x86
#endif


#if CAN_BAUDRATE==1000000
#define CNF1_Setting  0
#define CNF2_Setting  0xD0
#define CNF3_Setting  0x82
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

//CANCTRL sets different modes:
//normal mode
#define NORMAL_MODE 0
//loop back mode
#define LOOPBACK_MODE 2
//listen only mode
#define LISTENONLY_MODE 3

//default to normal mode
#ifndef CANMODE
#define CANCTRL_Setting NORMAL_MODE
//Or you can define a different mode
#else CANCTRL_Setting CANMODE
#endif

#endif
