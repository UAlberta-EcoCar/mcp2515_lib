//this sketch simulates a node in the CAN network
#include <mcp2515_filter_settings.h>
#include <can_message_defs.h>
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>

/*
 * - This node will be listening for:
 *      - Brake IDs
 *      
 * - This node will be sending:
 *      - MESSAGE_FC_LOGGING_FCTEMP_ID twice a second
 *      - MESSAGE_FC_LOGGING_FCVOLT_FCCURR_ID four times a second
 *      - MESSAGE_FC_LOGGING_CAPVOLT_CAPCURR_ID 5 times a second
 */

unsigned char var = 0;
#define NL Serial.println();
#define SPACE Serial.print(" ");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 
  if (can_init())
  {
    Serial.print("mcp2515 config error: ");
    NL
    while(1); //hang up program

  }
  //else everything is good (hopefully)
  delay(2000);
}




unsigned long time1 = millis();
unsigned long time2 = millis();
unsigned long time3 = millis();
unsigned long fc_temp1 = 2750;
unsigned long fc_temp2 = 3250;
unsigned long fc_volt = 12;
unsigned long fc_curr = 4;
unsigned long fc_cap_volt = 5;
unsigned long fc_cap_curr = 1;

CanMessage message_send;
CanMessage message_receive;

void loop() 
{
  /*
  //Send Fuel Cell Temperature 2 times a second
  if ((millis() - time1) > 500) {
    // Fill message fields
    
    message_send.id = MESSAGE_FC_LOGGING_FCTEMP_ID;
    message_send.length = MESSAGE_FC_LOGGING_FCTEMP_LENGTH;
    message_send.RTransR = 0;
    message_send.MESSAGE_FC_LOGGING_FCTEMP_DATA1 = fc_temp1;
    message_send.MESSAGE_FC_LOGGING_FCTEMP_DATA2 = fc_temp2;
    
    if (can_send_message(&message_send)) {
      Serial.println("FC Temp Transmission Error");
    }
    
    else {
      Serial.println("Sent Fuel Cell Temperature");
    }
    time1 = millis();
  }
  //Send Fuel Cell Voltage and Current 4 times a second
  if ((millis() - time2) > 250) {
    // Fill message fields 
    message_send.id = MESSAGE_FC_LOGGING_FCVOLT_FCCURR_ID;
    message_send.length = MESSAGE_FC_LOGGING_FCVOLT_FCCURR_LENGTH;
    message_send.RTransR = 0;
    message_send.MESSAGE_FC_LOGGING_FCVOLT_DATA = fc_volt;
    message_send.MESSAGE_FC_LOGGING_FCVOLT_DATA = fc_curr;
    
    if (can_send_message(&message_send)) {
      Serial.println("FC Voltage/Current Transmission Error");
    }
    else {
      Serial.println("Sent FC Voltage/Current");
    }
    time2 = millis();
  }
  
  //Send Fuel Cell Capacitor Voltage and Current 5 times a second
  if ((millis() - time3) > 200) {

    // Create fill message fields
    message_send.id = MESSAGE_FC_LOGGING_CAPVOLT_CAPCURR_ID;
    message_send.length = MESSAGE_FC_LOGGING_CAPVOLT_CAPCURR_LENGTH;
    message_send.RTransR = 0;
    message_send.MESSAGE_FC_LOGGING_CAPVOLT_DATA = fc_cap_volt;
    message_send.MESSAGE_FC_LOGGING_CAPCURR_DATA = fc_cap_curr;
    
    if (can_send_message(&message_send)) {
      Serial.println("FC Capacitor Voltage/Current Transmission Error");
    }
    else {
      Serial.println("Sent FC Capacitor Voltage/Current");
    }
    time3 = millis();
  }
*/
  
  // A Message is Received
  if (digitalRead(9) == 0) {
 
    message_receive = can_get_message();  // Get the message from the rx buffers
    unsigned int ID = message_receive.id; // copy message ID
    
    if (ID == MESSAGE_BRAKE_ID){
      //message is brake indicator
      if (message_receive.MESSAGE_BRAKE_DATA == BRAKE_ON){
        Serial.println("Brake Pedal Signal Received");
      }
    }
    else if (ID == MESSAGE_GAS_PEDAL_ID){
      //message is gas pedal indicator
      if (message_receive.MESSAGE_GAS_PEDAL_DATA == GAS_PEDAL_ON){
        Serial.println("Gas Pedal Signal Received: Should Not be Receiving");
      }
    }
    else {
      // 
      Serial.println("Invalid Message ID");
    }
  }
  delay(1);
}
