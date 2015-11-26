//this sketch receives data over CAN
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>



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
unsigned long time1 = 0;
unsigned long message_count = 0; 
void loop() 
{
  //make new empty message
  CanMessage message;
  message.id = 0;
  if(millis()-time1 > 1000)
  {
    Serial.print("Messages recieved: ");
    Serial.println(message_count);
    message_count = 0;
    time1 = millis();
  }
  
  if(digitalRead(9) == 0)
  {
    //wait for an message received interrupt
    //receive message
    message = can_get_message();
    if(message.id == 0x7)
    {
      message_count ++;
    }
  }
}
