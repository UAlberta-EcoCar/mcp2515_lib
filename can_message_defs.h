#ifndef CAN_MESSAGE_DEFS_H
#define CAN_MESSAGE_DEFS_H
//Defines for all messages that will go over our CAN-BUS
//All messages will be defined here to avoid conflicts
//11 bit identifier = valid id's are 0 to 2047
//messages are sent according to priority, 0 is highest priority
//keep message priorities spreadout for future additions


/* ID Assignment:
 * bits 0 - 3 are for data type 
 *		0b1111 = two 4 byte integers
 *		0b1110 = single 4 byte integer
 *		0b0001 = single boolean byte
 * 10th bit is for real time (0) vs data logging (1)
 * 9th bit is for high (0) or low (1) relative priority
 * IDEA: 8th and 7th bits for general type (i.e. fuel cell, motor, others...)
 * bits 4 - 8 are for identifying the message source
 */

//when brake pedal is hit it is a high priority message (0b000 0001 0001)
//Absolutely critical messages have 0b000 0xxx xxxx format
#define MESSAGE_BRAKE_ID 0x11
#define MESSAGE_BRAKE_LENGTH 1
#define BRAKE_BYTE 0
#define BRAKE_BIT 0


//FC ERROR message is real time high priority (0b000 0010 1110)
//All Fuel cell messages have 0bxx0 1xxx xxxx format
#define MESSAGE_FC_ERROR_ID 0x2e
#define MESSAGE_FC_ERROR_LENGTH 4
#define ERROR_INT 0 //FC uses 32 bit integers
#define ERROR_BYTE0 0 //mcp2515 uses bytes
#define ERROR_BYTE1 1
#define ERROR_BYTE2 2
#define ERROR_BYTE3 3



//Reading the gas pedal is real time but not high priority (0b011 1000 0001)
// Non critical, motor, fuel cell messages have 0bxx1 1xxx xxxx
#define MESSAGE_GAS_PEDAL_ID 0x381
#define MESSAGE_GAS_PEDAL_LENGTH 1
#define GAS_PEDAL_MSB 0
#define GAS_PEDAL_LSB 1


//FC Data Logging (8 types so far): low priority (0b110 1xxx 111x)

//Fuelcell temperature mK
#define MESSAGE_FC_LOGGING_FCTEMP_ID 0x68f
#define MESSAGE_FC_LOGGING_FCTEMP_LENGTH 8
#define FCTEMP1_INT 0 //fc uses 4 byte integers
#define FCTEMP1_BYTE0 0 //mcp2515 uses single bytes
#define FCTEMP1_BYTE1 1
#define FCTEMP1_BYTE2 2
#define FCTEMP1_BYTE3 3
#define FCTEMP2_INT 1
#define FCTEMP2_BYTE0 4
#define FCTEMP2_BYTE1 5
#define FCTEMP2_BYTE2 6
#define FCTEMP2_BYTE3 7

//Fuelcell current and voltage mA and mV
#define MESSAGE_FC_LOGGING_FCVOLT_FCCURR_ID 0x69f
#define MESSAGE_FC_LOGGING_FCVOLT_FCCURR_LENGTH 8
#define FCVOLT_INT 0 //fc uses 4 byte integers
#define FCVOLT_BYTE0 0 //mcp2515 uses single bytes
#define FCVOLT_BYTE1 1
#define FCVOLT_BYTE2 2
#define FCVOLT_BYTE3 3
#define FCCURR_INT 0 //fc uses 4 byte integers
#define FCCURR_BYTE0 0 //mcp2515 uses single bytes
#define FCCURR_BYTE1 1
#define FCCURR_BYTE2 2
#define FCCURR_BYTE3 3

//Super capacitor volt and current mV mA
#define MESSAGE_FC_LOGGING_CAPVOLT_CAPCURR_ID 0x6af
#define MESSAGE_FC_LOGGING_CAPVOLT_CAPCURR_LENGTH 8
#define CAPVOLT_INT 0 //fc uses 4 byte integers
#define CAPVOLT_BYTE0 0 //mcp2515 uses single bytes
#define CAPVOLT_BYTE1 1
#define CAPVOLT_BYTE2 2
#define CAPVOLT_BYTE3 3
#define CAPCURR_INT 1 //fc uses 4 byte integers
#define CAPCURR_BYTE0 4 //mcp2515 uses single bytes
#define CAPCURR_BYTE1 5
#define CAPCURR_BYTE2 6
#define CAPCURR_BYTE3 7

#define MESSAGE_FC_LOGGING_MFLOW_ID 0x6be
#define MESSAGE_FC_LOGGING_MFLOW_LENGTH 4
#define MFLOW_INT 0 //fc uses 4 byte integers
#define MFLOW_BYTE0 0 //mcp2515 uses bytes
#define MFLOW_BYTE1 1
#define MFLOW_BYTE2 2
#define MFLOW_BYTE3 3

// Fuel Cell Pressure Readings
#define MESSAGE_FC_LOGGING_FCPRES_TANKPRES_ID 0x6cf
#define MESSAGE_FC_LOGGING_FCPRES_TANKPRES_LENGTH 8
#define FCPRES_INT 0 //fc uses 4 byte integers
#define FCPRES_BYTE0 0 //mcp2515 uses bytes
#define FCPRES_BYTE1 1
#define FCPRES_BYTE2 2 
#define FCPRES_BYTE3 3 
#define TANKPRES_INT 1
#define TANKPRES_BYTE0 4
#define TANKPRES_BYTE1 5
#define TANKPRES_BYTE2 6
#define TANKPRES_BYTE3 7

// Fuel Cell Ambient Temperature Readings 1 and 2
#define MESSAGE_FC_LOGGING_AMBTEMP12_ID 0x6df
#define MESSAGE_FC_LOGGING_AMBTEMP12_LENGTH 8
#define AMBTEMP1_INT 0 //fc uses 4 byte integers
#define AMBTEMP1_BYTE0 0 //mcp2515 uses bytes
#define AMBTEMP1_BYTE1 1
#define AMBTEMP1_BYTE2 2
#define AMBTEMP1_BYTE3 3
#define AMBTEMP2_INT 1 //fc uses 4 byte integers
#define AMBTEMP2_BYTE0 4 //mcp2515 uses bytes
#define AMBTEMP2_BYTE1 5
#define AMBTEMP2_BYTE2 6
#define AMBTEMP2_BYTE3 7

// Fuel Cell Ambient Temperature Readings 3 and 4
#define MESSAGE_FC_LOGGING_AMBTEMP34_ID 0x6ef
#define MESSAGE_FC_LOGGING_AMBTEMP34_LENGTH 8
#define AMBTEMP3_INT 0 //fc uses 4 byte integers
#define AMBTEMP3_BYTE0 0 //mcp2515 uses bytes
#define AMBTEMP3_BYTE1 1
#define AMBTEMP3_BYTE2 2
#define AMBTEMP3_BYTE3 3
#define AMBTEMP4_INT 1 //fc uses 4 byte integers
#define AMBTEMP4_BYTE0 4 //mcp2515 uses bytes
#define AMBTEMP4_BYTE1 5
#define AMBTEMP4_BYTE2 6
#define AMBTEMP4_BYTE3 7

//Fuelcell state
#define MESSAGE_FC_LOGGING_STATE_ID 0x6fe
#define MESSAGE_FC_LOGGING_STATE_LENGTH 4
#define STATE_BYTE 3
#define STATE_INT 0

#endif