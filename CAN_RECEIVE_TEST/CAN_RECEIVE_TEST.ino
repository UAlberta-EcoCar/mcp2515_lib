#include <MemoryFree.h>
#include <mcp2515_defs.h>
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
  var = mcp2515_init();
  if (var != 0)
  {
    Serial.println("mcp2515 config error");
    Serial.print("error:");
    Serial.println(var);
    NL
  }
  else
  {

  }
  pinMode(2,INPUT); //mcp2515 interrupt pin
}

CanMessage message;
void loop() {
  // put your main code here, to run repeatedly:
while(digitalRead(2) == 1)
{
  //wait for an message received interrupt
}
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
  Serial.print(message.data[0]);
  SPACE
  Serial.print(message.data[1]);
  SPACE
  Serial.print(message.data[2]);
  SPACE
  Serial.print(message.data[3]);
  SPACE
  Serial.print(message.data[4]);
  SPACE
  Serial.print(message.data[5]);
  SPACE
  Serial.print(message.data[6]);
  SPACE
  Serial.println(message.data[7]);
}

Serial.print("FreeRAM=");
Serial.println(freeMemory());
Serial.println();
}
