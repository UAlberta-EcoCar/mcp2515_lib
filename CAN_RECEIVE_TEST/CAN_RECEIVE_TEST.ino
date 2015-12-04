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

  Serial.print("ID: ");
  Serial.println(message.id,HEX);

  Serial.print("Length: ");
  Serial.println(message.length);

  if (message.RTransR)
  {
    Serial.println("Remote transmit request");
  }
  else
  {
    Serial.println("Data:");
    for(char i = 0; i < message.length; i++)
    {
      Serial.print(message.data.u8[i],HEX);
      SPACE
    }
  }
NL
NL
}
