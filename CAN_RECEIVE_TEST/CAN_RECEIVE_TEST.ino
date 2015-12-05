
//this sketch receives data over CAN
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>
#include <can_message_defs.h>


unsigned char var = 0;

#define NL Serial.println();
#define SPACE Serial.print(" ");


void setup() 
{
  Serial.begin(115200);

  
  if (can_init())
  {
    Serial.println("mcp2515 config error");
    NL
    while(1)
    {
      //hang up program
    }
  }
  //else: everything is good
  pinMode(9,INPUT); //mcp2515 interrupt pin
}

void loop() 
{
  //make new empty message
  CanMessage message;
  while(digitalRead(9) == 1)
  {
    //wait for an message received interrupt
  }
  //receive message
  message = can_get_message();
  if(message.id == MESSAGE_FC_FCTEMP_ID)
  {
    Serial.print("FCTEMP ");
    Serial.print(message.MESSAGE_DATA_FCTEMP1);
    Serial.print("  ");
    Serial.println(message.MESSAGE_DATA_FCTEMP2);
  }
NL
NL
}
