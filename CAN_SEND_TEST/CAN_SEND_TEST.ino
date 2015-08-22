//This examples sends the first 8 digits of pi over can.
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>
#include <spi_lib.h>


unsigned char var = 0;

#define NL Serial.println();
#define SPACE Serial.print(" ");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  spi_init();
  var = mcp2515_init(normal);
  if (var != 0)
  {
    Serial.print("mcp2515 config error: ");
    Serial.println(var,BIN);
    NL
    while(1)
    {
      //hang up program
    }
  }
}

void loop() 
{

  delay(2000);

  //creat new message
  CanMessage message_send;
  //set message id
  message_send.id = 0x7;
  //set message length
  message_send.length = 8;
  //set whether message is a remote transmit request
  message_send.RTransR = 0;
  //set 8 data bytes
  message_send.data[0] = 3;
  message_send.data[1] = 1;
  message_send.data[2] = 4;
  message_send.data[3] = 1;
  message_send.data[4] = 5;
  message_send.data[5] = 9;
  message_send.data[6] = 2;
  message_send.data[7] = 6;
  
  var = can_send_message(&message_send);
  if (var != 0)
  {
    Serial.print("transmition error: ");
    Serial.println(var,BIN); 
  }
  else
  {
    Serial.println("Message sent");
  }

  NL

}
