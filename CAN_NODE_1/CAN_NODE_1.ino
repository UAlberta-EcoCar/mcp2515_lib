//this sketch simulates a node in the CAN network
#include <mcp2515_filter_settings.h>
#include <can_message_defs.h>
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>

/*
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
  while (!Serial);

 // Initialize CAN
 Serial.print("Initializing mcp2515 CAN controller... ");
  if (can_init()){
    Serial.println("mcp2515 config error");
    while(1); //hang up program
  }
  Serial.println("mcp2515 initialization successful");
  
  // Short delay and then begin communication
  delay(2000);
}




unsigned long time1 = millis();
unsigned long time2 = millis();
unsigned long time3 = millis();
unsigned long fc_temp1 = 0;
unsigned long fc_temp2 = 1;
unsigned long fc_volt = 12;
unsigned long fc_curr = 4;
unsigned long fc_cap_volt = 5;
unsigned long fc_cap_curr = 1;

CanMessage message_send;
CanMessage message_receive;

void loop() 
{ 
  message_receive = can_get_message();
  if (message_receive.id)
  {
    Serial.println("HI");
  }
}






