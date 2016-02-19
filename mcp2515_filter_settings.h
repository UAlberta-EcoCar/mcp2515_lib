#ifndef MCP2515_FILTER_SETTINGS_H
#define MCP2515_FILTER_SETTINGS_H



//#include "can_message_defs.h"

// List of Node IDs
#define NODE_1 1
#define NODE_2 2
#define FILTER_FUEL_CELL_CONTROLLER 3
#define FILTER_MOTOR_CONTROLLER 4
#define LISTEN_ALL 5

// Choose the Node Being Used
#ifndef NODE_ID
#define NODE_ID LISTEN_ALL
#endif

// MASKS:
//		- 0x7f0 considers all bits except the least significant byte
//		- 0x7ff considers all bits

#if NODE_ID==NODE_1

// do not consider data type bits
#define MASK0 0x7f0
#define MASK1 0x7f0

#define FILTER0 MESSAGE_BRAKE_ID
#define FILTER1 0
#define FILTER2 0
#define FILTER3 0
#define FILTER4 0
#define FILTER5 0

#endif


#if NODE_ID==NODE_2

// do not consider data type bits
#define MASK0 0x7f0
#define MASK1 0x7f0

#define FILTER0 MESSAGE_FC_LOGGING_FCTEMP_ID
#define FILTER1 MESSAGE_FC_LOGGING_FCVOLT_FCCURR_ID
#define FILTER2 0
#define FILTER3 0
#define FILTER4 0
#define FILTER5 0

#endif


#if NODE_ID==LISTEN_ALL

#define MASK0 0x000
#define MASK1 0x000

#define FILTER0 0
#define FILTER1 0
#define FILTER2 0
#define FILTER3 0
#define FILTER4 0
#define FILTER5 0

#endif

#endif