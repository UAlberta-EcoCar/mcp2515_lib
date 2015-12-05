//this sketch simulates a node in the CAN network
#include <mcp2515_filter_settings.h>
#include <can_message_defs.h>
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>

/*
 * - This node will be listening for:
 *      - MESSAGE_FC_LOGGING_FCTEMP_ID
 *      - MESSAGE_FC_LOGGING_FCVOLT_FCCURR_ID
 *      - not MESSAGE_FC_LOGGING_CAPVOLT_CAPCURR_ID (they should be filtered out)
 *      
 * - This node will be sending only:
 *      - Brake IDs (once a second)
 *      - Gas Pedal IDs (5 times a second)
 */

unsigned char var = 0;
#define NL Serial.println();
#define SPACE Serial.print(" ");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 
  if (can_init())
  {
    Serial.println("mcp2515 config error: ");
    while(1); // hang up program
  }
  
  //else everything is good (hopefully)
  delay(2000);
}




unsigned long time1 = millis();
unsigned long time2 = millis();
unsigned long fc_temp1 = 0;
unsigned long fc_temp2 = 0;
unsigned long fc_volt = 0;
unsigned long fc_curr = 0;
unsigned long fc_cap_volt = 0;
unsigned long fc_cap_curr = 0;
unsigned char rx_status;

// Allocate the send and receive memory  just once to save space
CanMessage message_receive;
CanMessage message_send;


void loop() 
{
  //Send Gas Pedal Message 1 time a second
  if ((millis() - time1) > 1000) {
    // Fill message fields
    message_send.id = MESSAGE_GAS_PEDAL_ID;
    message_send.length = MESSAGE_GAS_PEDAL_LENGTH;
    message_send.RTransR = 0;
    message_send.MESSAGE_GAS_PEDAL_DATA = GAS_PEDAL_ON;

    // Write message to tx buffers
    if (can_send_message(&message_send)) {
      Serial.println("Gas Pedal Transmission Error");
    }
    else {
      Serial.println("Sent Gas Pedal Message");
    }
    time1 = millis();
  }


  //Send Brake Pedal Message once per 3 seconds
  if ((millis() - time2) > 3000) {
    // Fill message fields
    message_send.id = MESSAGE_BRAKE_ID;
    message_send.length = MESSAGE_BRAKE_LENGTH;
    message_send.RTransR = 0;
    message_send.MESSAGE_BRAKE_DATA = BRAKE_ON;

    // Write to tx buffers
    if (can_send_message(&message_send)) {
      Serial.println("Brake Transmission Error");
    }
    else {
      Serial.println("Sent Brake Message");
    }
    time2 = millis();
  }
  
  // A Message is Received
  if (digitalRead(9) == 0) {
    // Uncomment to read rx_status register for debugging
    rx_status = mcp2515_read_rx_status();
    Serial.print("rx_status: ");
    Serial.println(rx_status, HEX);

    message_receive = can_get_message();  // Get message from rx buffers

    unsigned int ID = message_receive.id; // copy message ID
    
    if (ID == MESSAGE_FC_LOGGING_FCTEMP_ID){
      //message is fuel cell logging its temperature
      fc_temp1 = message_receive.MESSAGE_FC_LOGGING_FCTEMP_DATA1;
      fc_temp2 = message_receive.MESSAGE_FC_LOGGING_FCTEMP_DATA2;
      Serial.print("FC Temperature(1): ");
      Serial.println(fc_temp1);
      Serial.print("FC Temperature(2): ");
      Serial.println(fc_temp2);
    }
    else if (ID == MESSAGE_FC_LOGGING_FCVOLT_FCCURR_ID){
      //message is fuel cell logging its voltage and current
      fc_volt = message_receive.MESSAGE_FC_LOGGING_FCVOLT_DATA;
      fc_curr = message_receive.MESSAGE_FC_LOGGING_FCCURR_DATA;
      Serial.print("FC Voltage: ");
      Serial.println(fc_volt);
      Serial.print("FC Current: ");
      Serial.println(fc_curr);
    }
    else if (ID == MESSAGE_FC_LOGGING_CAPVOLT_CAPCURR_ID){
      //message is fuel cell logging its capacitor voltage and current
      fc_cap_volt = message_receive.MESSAGE_FC_LOGGING_CAPVOLT_DATA;
      fc_cap_curr = message_receive.MESSAGE_FC_LOGGING_CAPCURR_DATA;
      Serial.print("FC Capacitor Voltage: ");
      Serial.println(fc_cap_volt);
      Serial.print("FC Capacitor Current: ");
      Serial.println(fc_cap_curr);
    }
    else {
      Serial.print("Invalid Message");
      Serial.println(ID,BIN);
    }
  }
  delay(1);
}
