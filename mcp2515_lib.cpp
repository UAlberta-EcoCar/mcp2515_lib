#include "mcp2515_lib.h"
#include "spi_lib.h"
#include "mcp2515_defs.h"
#include "mcp2515_settings.h"
#include <arduino.h>
#include <avr/io.h>
#include <avr/delay.h>



void mcp2515_write_register( unsigned char address, unsigned char data ) 
{ 
    //select the mcp2515
    CAN_CS_LOW
    //tell chip you're going to write
    spi_putc ( SPI_WRITE ) ;
    //send address to write to
    spi_putc ( address ) ;    
    //send value
    spi_putc ( data ) ; 
    //deselect mcp2515  
    CAN_CS_HIGH 
}

void mcp2515_bit_modify ( unsigned char address, unsigned char mask, unsigned char data ) 
{ 
    //select mcp2515
    CAN_CS_LOW
	//send bit modify command
	spi_putc ( SPI_BIT_MODIFY ) ;
	//send register address
    spi_putc ( address ) ;
	//send modify bit mask
	 spi_putc ( mask ) ;
	//send new bit values
    spi_putc ( data ) ; 
	//deselect mcp2515
    CAN_CS_HIGH 
}

unsigned char mcp2515_read_register(unsigned char adress)
{
	unsigned char data;
	//select mcp2515
    CAN_CS_LOW
    //send read command
    spi_putc(SPI_READ);
	//send address you want to read
    spi_putc(adress);
	//send dummy byte to read value
    data = spi_putc(0xff); 
	//deselect mcp2515
    CAN_CS_HIGH
    return data;
}



unsigned int can_init(void)
{
	spi_init();
	DDR_CAN_CS |= (1 << P_CAN_CS); //set CAN_CS pin to output
	CAN_CS_LOW //select mcp2515
	spi_putc(SPI_RESET); //send reset command
	_delay_ms(10); //wait for chip to reset
	CAN_CS_HIGH //deselect chip
	
	unsigned int error = 0;
	
	//write to all registers using settings defined mcp2515_settins.h
	//also verify a few settings so that you know the chip is connected
	mcp2515_write_register ( CNF1 , CNF1_Setting);
	if (mcp2515_read_register(CNF1) != CNF1_Setting)
	{
		error |= 1;
	}
	
	mcp2515_write_register ( CNF2, CNF2_Setting );
	if (mcp2515_read_register(CNF2) != CNF2_Setting)
	{
		error |= (1 << 1);
	}
	
	mcp2515_write_register ( CNF3, CNF3_Setting );
	if (mcp2515_read_register(CNF3) != CNF3_Setting)
	{
		error |= (1 << 2);
	}
	
	mcp2515_write_register ( CANINTE, CANINTE_Setting);

	mcp2515_write_register ( RXB0CTRL, RXB0CTRL_Setting );
	
	mcp2515_write_register ( RXB1CTRL, RXB1CTRL_Setting );
	
	mcp2515_write_register ( BFPCTRL, BFPCTRL_Setting ) ;
	
	mcp2515_write_register ( TXRTSCTRL, TXRTSCTRL_Setting ) ;
	
	//set canctrl to specified mode
	mcp2515_write_register(CANCTRL,CANCTRL_Setting);

	//set tx buffer 0 to high priority
	mcp2515_write_register(TXB0CTRL, (1 << TXP0) | (1 << TXP1));
	
	//set tx buffer 1 to high intermediate priority
	mcp2515_write_register(TXB1CTRL, (1 << TXP1));
	
	//set tx buffer 2 to low intermediate priority
	mcp2515_write_register(TXB2CTRL, (1 << TXP0));
	
	return(error);
}

unsigned char can_send_message ( CanMessage *p_message ) //sends message using tx buffer 0
{ 	
	unsigned char send_command;
	//check if tx buffer 0 is availible for message transmission
	if((mcp2515_read_register(TXB0CTRL) & (1 << TXREQ)) == 0)
	{
		send_command = SPI_RTS | 0x01; //RTS first buffer
		CAN_CS_LOW //select mcp2515
		//write to tx 0 buffers
		spi_putc(SPI_WRITE_TX | 0x00); //starting at TXB0SIDH
	}
	//check tx buffer 1
	else if ((mcp2515_read_register(TXB1CTRL) & (1 << TXREQ))==0)
	{
		send_command = SPI_RTS | 0x02; //RTS second buffer
		CAN_CS_LOW //select mcp2515
		//write to tx 1 buffers
		spi_putc(SPI_WRITE_TX | 0x02); //starting at TXB1SIDH
	}
	//check buffer 2
	else if ((mcp2515_read_register(TXB2CTRL) & (1 << TXREQ))==0)
	{
		send_command = SPI_RTS | 0x04; //RTS third buffer
		CAN_CS_LOW //select mcp2515
		//write to tx 2 buffers
		spi_putc(SPI_WRITE_TX | 0x04); //starting at TXB2SIDH
	}
	else //no buffers open
	{
		//something is wrong or you are sending way to many messages
		return(1);
	}
	
	//SPI_WRITE_TX command automatically moves to next address after write
	//continue writing to other registers
	spi_putc(p_message -> id >> 3); //write to TXBnSIDH
	spi_putc(p_message -> id << 5); //write to TXBnSIDL
	spi_putc(0x00); //write to  TXBnEID8
	spi_putc(0x00); //write to TXBnEID0
	
	if (p_message -> RTransR) //if message is a remote transmit request
	{
		//write data length (should be zero?) and RTR bit
		spi_putc((1 << RTR) | p_message -> length); //write to TXBnDLC
		//RTR does not contain data
	}
	else //message is normal
	{
		//just write data length
		spi_putc(p_message -> length);
		
		//regular message contains data
		
		//write data bytes
		for (char i = 0; i < p_message -> length; i ++)
		{
		spi_putc(p_message -> data[i]); //write to TXBnDm
		}
	}
	
	CAN_CS_HIGH //done loading tx buffer
	
	// tell mcp2515 to send message  
	CAN_CS_LOW
	spi_putc ( send_command );  
	CAN_CS_HIGH

	return (0); 
}


unsigned char mcp2515_read_rx_status ( void ) 
{     //select mcp2515
    CAN_CS_LOW
    //send read rx status command
    spi_putc ( SPI_RX_STATUS ) ; 
    //read status
    unsigned char data = spi_putc ( 0xFF ) ; 
    // The data is sent again repeated
    // So you need only one of the two bytes evaluated     
    //therefore might as well comment out this line
    //spi_putc ( 0xff ) ; 
    CAN_CS_HIGH
    return data;
}


//get message from mcp2515
//issues: 
	//what if there are two messages?
	//does interrupt stay on?
	//solution: read status a second time, return value and let user program decide
				// issue: not as nice
				// extra 3 spi commands?
			//or rebuild function to return two messages
				// issue: function becomes x2 larger. x2 ram usage
	//more testing required
CanMessage can_get_message ( void ) 
{ 
    CanMessage p_message; //create a message
    
	//start reading message
	
	// read status 
    unsigned char status = mcp2515_read_rx_status ( ) ;
     
	if ( bit_is_set ( status, 6 )  )
	{ // message in buffer 0      	CAN_CS_LOW  //select mcp2515
		spi_putc(SPI_READ_RX | 0); //Start reading at RXB0SIDH
	} 
    else if ( bit_is_set ( status, 7 ) ) 
    { // message in buffer 1      
    	CAN_CS_LOW  //select mcp2515
		spi_putc(SPI_READ_RX | 2); //Start reading at RXB1SIDH
    } 
    else 
    { // Error: No new message available
		return p_message;
    } 
	
    //read RXBnSIDH
    p_message.id = spi_putc(0x00) << 3;
    //read RXBnSIDL
    p_message.id |= spi_putc(0x00) >> 5;
    //read RXBnEID8
    spi_putc(0x00);
    //read RXBnEID0
    spi_putc(0x00);
    //read RXBnDLC
    unsigned char RXBnDLC = spi_putc(0x00);
    p_message.length = RXBnDLC & 0xff; //message length is last four bits
    if (RXBnDLC & (1 << RTR)) //if message is a remote transmit request
    {
    	p_message.RTransR = 1; //message is a remote transmit request
    	//it won't have any data
    }
    else //message is not a remote transmit request
    {
    	//read data
    	for(char i = 0; i < p_message.length; i++)
    	{
    		p_message.data[i] = spi_putc(0x00);
    	}
    }
    CAN_CS_HIGH //deselect mcp2515
   	return(p_message);
}