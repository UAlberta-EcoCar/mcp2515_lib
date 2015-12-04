//This example sends the first 8 digits of pi over can.
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>



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
unsigned int message_count = 0;
unsigned long time1 = 0;
void loop() 
{
  //creat new message
  CanMessage message_send;
  //set message id
  message_send.id = 0x7;
  //set message length
  message_send.length = 8;
  //set whether message is a remote transmit request (it's not)
  message_send.RTransR = 0;
  //set 8 data bytes
  message_send.data.u8[0] = 3;
  message_send.data.u8[1] = 1;
  message_send.data.u8[2] = 4;
  message_send.data.u8[3] = 1;
  message_send.data.u8[4] = 5;
  message_send.data.u8[5] = 9;
  message_send.data.u8[6] = 2;
  message_send.data.u8[7] = 6;
  
  if (can_send_message(&message_send))
  {
    //Serial.println("Transmition Error: ");
    //something is wrong and/or all transmition buffers are full.
  }
  else
  {
    //Serial.println("Message sent");
    message_count ++;
  }
  if(millis()-time1 > 1000)
  {
    Serial.print("Messages sent: ");
    Serial.println(message_count);
    message_count = 0;
    
    delay(1000);
    time1 = millis();
  } 
}
