#include <MemoryFree.h>
#include <mcp2515_defs.h>
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>
#include <spi_lib.h>


unsigned char var = 0;

#define NL Serial.println();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(A0,OUTPUT);
  digitalWrite(A0,HIGH);
  
  spi_init();
  var = mcp2515_init();
  if (var != 0)
  {
    Serial.println("mcp2515 config error");
    Serial.print("error:");
    Serial.println(var,BIN);
    NL
    while(1)
    {
      //hang up program
    }
  }
  else
  {

  }

}

void loop() {
  // put your main code here, to run repeatedly:

delay(2000);
CanMessage message; //create new message
message.id = 0xfe;
message.length = 8;
message.RTransR = 0;
message.data[0] = 3;
message.data[1] = 1;
message.data[2] = 4;
message.data[3] = 1;
message.data[4] = 5;
message.data[5] = 9;
message.data[6] = 2;
message.data[7] = 6;

var = can_send_message(&message);
if (var != 0)
{
  Serial.print("transmition error: ");
  Serial.println(var,BIN); 
}
else
{
  Serial.println("Message sent");
}

Serial.print("FreeRAM=");
Serial.println(freeMemory());
Serial.println();


}
