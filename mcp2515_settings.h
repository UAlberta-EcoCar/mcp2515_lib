#ifndef mcp2515_settings_H
#define mcp2515_settings_H

//to do: comments explaining each setting

//see http://www.kvaser.com/support/calculators/bit-timing-calculator/ for calculating these values
//im not sure how they work exactly
#define CNF1_Setting  0xc0
#define CNF2_Setting  0x91
#define CNF3_Setting  0x03
//this should be 500kHz with a 10 MHz crystal


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
