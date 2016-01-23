//this sketch simulates a node in the CAN network
#include <mcp2515_filter_settings.h>
#include <can_message_defs.h>
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>

#include <SPI.h>
#include <SD.h>

const int CS = 10;


void setup() {
  // Start Serial Communication
  Serial.begin(115200);
  while (!Serial);

  // Initialize CAN Controller
  Serial.print("Initializing mcp2515 CAN controller... ");
  if (can_init())
  {
    Serial.println("mcp2515 config error");
    while(1); //hang up program
  }
  Serial.println("mcp2515 initialization successful");

  //Initialize SD Communication
  Serial.print("Initializing SD card... ");

  // Retry initializing until successful
  while (!SD.begin(CS)){
    Serial.println("Card failed, or not present");
    delay(100);
    Serial.print("Initializing SD card... ");
  }
  Serial.println("SD card initialized.");

  // Clear SD file being written to
  SD.remove("datalog.txt");


  // Short delay and then begin listening
  delay(2000);
}

CanMessage message_send;      // Create empty message for sending
CanMessage message_receive_0;   // Create 2 empty messages for receiving
CanMessage message_receive_1;

bool MESSAGE0_FULL = false;
bool MESSAGE1_FULL = false;
unsigned int counter = 0;


unsigned int ID_tmp;
unsigned int length_tmp;
unsigned int data_tmp;


void loop() 
{
  // If there is an indication that data has been recently read from buffers:
  if (MESSAGE0_FULL || MESSAGE1_FULL){ // If any message has been taken off CANBUS, construct a string to write to SD card
    
    String datastring = ""; // Create empty string
    
    if (MESSAGE0_FULL){     // Write message0 to a single line separated by spaces
      Serial.print("Mess 0 ");
      ID_tmp = message_receive_0.id;
      length_tmp = message_receive_0.length;
      
      datastring += String(ID_tmp);  // add ID to string
      datastring += " ";
      datastring += String(length_tmp); // add length to string
      for (int i = 0; i < length_tmp / 2; i++){ // Add data to string
        datastring += " ";
        datastring += String(message_receive_0.data.u32[i]);
      }
      datastring += "\n"; // Creates a newline at the end of the line
      MESSAGE0_FULL = false;
    }
    if (MESSAGE1_FULL){     // Write message1 to a single line separated by spaces
      Serial.print("Mess 1 ");
      
      ID_tmp = message_receive_1.id;
      length_tmp = message_receive_1.length;
      
      datastring += String(ID_tmp);  // add ID to string
      datastring += " ";
      datastring += String(length_tmp); // add length to string
      for (int i = 0; i < length_tmp / 2; i++){ // Add data to string
        datastring += " ";
        datastring += String(message_receive_1.data.u32[i]);
      }
      datastring += "\n"; // Creates a newline at the end of the line
      MESSAGE1_FULL = false;
    }
    // Open a File for writing to
    File dataFile = SD.open("datalog.txt", FILE_WRITE);   // Open file in SD card for writing
    delayMicroseconds(10); // Let SD finish creating the file
    if (!SD.exists("datalog.txt")){   // Check if file was successfully opened
      Serial.println("After: datalog.txt does not exist");
      while(1);
    }
    // Write strings to SD card  
    if (dataFile) { // if the file is open, perform the write commands
      dataFile.print(datastring);       // Write to opened file then close it
      dataFile.close();
      Serial.print(datastring);         // print to the serial port too:
    }
    else {      // if the file isn't open, pop up an error:
      Serial.println("error writing to datalog.txt");
    }
  }

    
  
  

  // If the CAN_INT is low, there are pending messages in the rx buffers
  // Retrieve 1 or both messages in the buffers

  while(digitalRead(9) == 0) {
    if (!MESSAGE0_FULL){
      message_receive_0 = can_get_message();  // Get the message from the rx buffers
      MESSAGE0_FULL = true;
    }
    else if (!MESSAGE1_FULL){
      message_receive_1 = can_get_message();  // Get the message from the rx buffers
      MESSAGE1_FULL = true;
    }
    
    else {
      break;
    }
  }
  delay(1);
}
