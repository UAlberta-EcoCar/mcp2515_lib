#ifndef mcp2515_settings_H
#define mcp2515_settings_H

//to do: comments explaining each setting


#define CNF1_Setting  (( 1 << BRP0 ) | ( 1 << BRP1 ) | ( 1 << BRP2 )) 
//Should be 125 kbps (will try faster later)

#define CNF2_Setting (( 1 << BTLMODE ) | ( 1 << PHSEG11 ))
#define CNF3_Setting ( 1 << PHSEG21 )
//have to do with how mcp2515 samples it's receive lines

#define CANINTE_Setting (( 1 << RX1IE ) | ( 1 << RX0IE ))
//interrupt when either receive buffer is full

#define RXB0CTRL_Setting (( 1 << RXM1 ) | ( 1 << RXM0 ))
//all recieve masks/filters disabled buffer 0

#define RXB1CTRL_Setting (( 1 << RXM1 ) | ( 1 << RXM0 ))
//same for buffer 1
 
#define BFPCTRL_Setting ((1 << B0BFE)|(1 << B1BFE))
//RXnBF pins set as standard ouputs

#define TXRTSCTRL_Setting 0

#define CANCTRL_Setting (1 << OSM)
//Normal mode and One shot transmit mode

#endif