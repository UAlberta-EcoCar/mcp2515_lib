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
#define MESSAGE_BRAKE_DATA data.u8[0]
#define BRAKE_ON 1
#define BRAKE_OFF 0


//FC ERROR message is real time high priority (0b000 0010 1110)
//All Fuel cell messages have 0bxx0 1xxx xxxx format
#define MESSAGE_FC_ERROR_ID 0x2e
#define MESSAGE_FC_ERROR_LENGTH 4
#define MESSAGE_FC_ERROR_DATA data.u32[0]


//Reading the gas pedal is real time but not high priority (0b011 1000 0001)
// Non critical, motor, fuel cell messages have 0bxx1 1xxx xxxx
#define MESSAGE_GAS_PEDAL_ID 0x381
#define MESSAGE_GAS_PEDAL_LENGTH 1
#define MESSAGE_GAS_PEDAL_DATA data.u8[0]
#define GAS_PEDAL_ON 1
#define GAS_PEDAL_NOT_ON 0
#define GAS_PEDAL_MSB 0
#define GAS_PEDAL_LSB 1


//FC Data Logging: low priority

//Fuelcell temperature mK
#define MESSAGE_FC_LOGGING_FCTEMP_ID 0x68f
#define MESSAGE_FC_LOGGING_FCTEMP_LENGTH 8
#define MESSAGE_FC_LOGGING_FCTEMP_DATA1 data.u32[0]
#define MESSAGE_FC_LOGGING_FCTEMP_DATA2 data.u32[1]

//Fuelcell current and voltage mA and mV
#define MESSAGE_FC_LOGGING_FCVOLT_FCCURR_ID 0x69f
#define MESSAGE_FC_LOGGING_FCVOLT_FCCURR_LENGTH 8
#define MESSAGE_FC_LOGGING_FCVOLT_DATA data.u32[0]
#define MESSAGE_FC_LOGGING_FCCURR_DATA data.u32[1]


//Super capacitor volt and current mV mA
#define MESSAGE_FC_LOGGING_CAPVOLT_CAPCURR_ID 0x6af
#define MESSAGE_FC_LOGGING_CAPVOLT_CAPCURR_LENGTH 8
#define MESSAGE_FC_LOGGING_CAPVOLT_DATA data.u32[0]
#define MESSAGE_FC_LOGGING_CAPCURR_DATA data.u32[1]

#define MESSAGE_FC_LOGGING_MFLOW_ID 0x6be
#define MESSAGE_FC_LOGGING_MFLOW_LENGTH 4
#define MESSAGE_FC_LOGGING_MFLOW_DATA data.u32[0]

// Fuel Cell Pressure Readings
#define MESSAGE_FC_LOGGING_FCPRES_TANKPRES_ID 0x6cf
#define MESSAGE_FC_LOGGING_FCPRES_TANKPRES_LENGTH 8
#define MESSAGE_FC_LOGGING_FCPRES_DATA data.u32[0]
#define MESSAGE_FC_LOGGING_TANKPRES_DATA data.u32[1]

// Fuel Cell Ambient Temperature Readings 1 and 2
#define MESSAGE_FC_LOGGING_AMBTEMP12_ID 0x6df
#define MESSAGE_FC_LOGGING_AMBTEMP12_LENGTH 8
#define MESSAGE_FC_LOGGING_AMBTEMP1_DATA data.u32[0]
#define MESSAGE_FC_LOGGING_AMBTEMP2_DATA data.u32[1]

// Fuel Cell Ambient Temperature Readings 3 and 4
#define MESSAGE_FC_LOGGING_AMBTEMP34_ID 0x6ef
#define MESSAGE_FC_LOGGING_AMBTEMP34_LENGTH 8
#define MESSAGE_FC_LOGGING_AMBTEMP3_DATA data.u32[0]
#define MESSAGE_FC_LOGGING_AMBTEMP4_DATA data.u32[1]

//Fuelcell state
#define MESSAGE_FC_LOGGING_STATE_ID 0x6fe
#define MESSAGE_FC_LOGGING_STATE_LENGTH 4
#define MESSAGE_FC_LOGGING_STATE_DATA data.u32[0]

#endif