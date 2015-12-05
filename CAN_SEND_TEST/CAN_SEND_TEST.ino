//This example sends the first 8 digits of pi over can.
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>
#include <can_message_defs.h>


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
    while(1)
    {
      //hang up program
    }
  }
  //else everything is good (hopefully)
}

void loop() 
{

  delay(2000);

  //creat new message
  CanMessage message_send;
  //set message id
  //for example use id for Fuel cell temperature
  message_send.id = MESSAGE_FC_ID_FCTEMP;
  //set message length
  message_send.length = MESSAGE_FC_LENGTH_FCTEMP;
  //set whether message is a remote transmit request (it's not)
  message_send.RTransR = 0;
  //set 8 data bytes
  //fuel cell controller has two different temperature readings
  message_send.MESSAGE_FC_DATA_FCTEMP1 = 4000; //32bit unsigned int
  message_send.MESSAGE_FC_DATA_FCTEMP2 = 3999;
  
  if (can_send_message(&message_send))
  {
    Serial.print("Transmition Error: ");
    //something is wrong and/or all transmition buffers are full.
    NL
  }
  else
  {
    Serial.println("Message sent");
  }

  NL
}
