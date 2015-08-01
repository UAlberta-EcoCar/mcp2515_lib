#include "mcp2515_lib.h"
#include "spi_lib.h"
#include "mcp2515_defs.h"
#include "mcp2515_settings.h"
#include <arduino.h>
#include <avr/io.h>
#include <avr/delay.h>



void mcp2515_write_register( unsigned char address, unsigned char data ) 
{ 
    //  CS of the MCP2515 on low drag
    CAN_CS_LOW
    //tell chip you're going to write
    spi_putc ( SPI_WRITE ) ;
    //send address to write to
    spi_putc ( address ) ;    
    //send value
    spi_putc ( data ) ; 
    // CS line unlock     
    CAN_CS_HIGH 
}

void mcp2515_bit_modify ( unsigned char address, unsigned char mask, unsigned char data ) 
{ 
    // / CS of the MCP2515 on low drag
    CAN_CS_LOW
    spi_putc ( SPI_BIT_MODIFY ) ;
    spi_putc ( address ) ;
    spi_putc ( mask ) ;
    spi_putc ( data ) ; 
	// re-enable CS line
    CAN_CS_HIGH 
}

unsigned char mcp2515_read_register(unsigned char adress)
{
    unsigned char data;
   
    // /CS des MCP2515 auf Low ziehen
    CAN_CS_LOW
   
    spi_putc(SPI_READ);
    spi_putc(adress);
   
    data = spi_putc(0xff); 
   
    // /CS Leitung wieder freigeben
    CAN_CS_HIGH
   
    return data;
}



unsigned int mcp2515_init(char mode_select)
{
	DDR_CAN_CS |= (1 << P_CAN_CS); //set CS to output
	CAN_CS_LOW
	spi_putc(SPI_RESET);
	_delay_ms(10); //wait for chip to reset
	CAN_CS_HIGH
	
	unsigned int error = 0;
	
	//write to all registers using settings defined in other file
	//also verify the settings
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
	if (mcp2515_read_register(CANINTE) != CANINTE_Setting)
	{
		error |= (1 << 3);
	}
	
	mcp2515_write_register ( RXB0CTRL, RXB0CTRL_Setting );
	if(mcp2515_read_register(RXB0CTRL) != RXB0CTRL_Setting)
	{
		error |= (1 << 4);
	}
	
	mcp2515_write_register ( RXB1CTRL, RXB1CTRL_Setting );
	if(mcp2515_read_register(RXB1CTRL) != RXB1CTRL_Setting )
	{
		error |= (1 << 5);
	}
	
	mcp2515_write_register ( BFPCTRL, BFPCTRL_Setting ) ;
	if ((mcp2515_read_register(BFPCTRL) & 0x0F) != BFPCTRL_Setting)
	{
		error |= (1 << 6);
	}
	
	mcp2515_write_register ( TXRTSCTRL, TXRTSCTRL_Setting ) ;
	if ((mcp2515_read_register(TXRTSCTRL) & 0x07) != TXRTSCTRL_Setting)
	{
		error |= (1 << 7);
	}
	
	/*
	if (mode_select == loopback)
	{
		//enter loopback mode
		mcp2515_write_register(CANCTRL,CANCTRL_Setting); 
	}
	*/
	else
	{
		//enter normal mode
		mcp2515_write_register(CANCTRL,CANCTRL_Setting);
	}
	return(error);
}

unsigned char can_send_message ( CanMessage *p_message ) //sends message using tx buffer 0
{ 	
	//to do: utilise buffers 1 and 2 if buffer 0 is full
	
	//wait for TXB0CTRL.TXREQ bit to be clear
	while(mcp2515_read_register(TXB0CTRL) & (1 << TXREQ))
	{
		//do nothing
	}
	
	//set buffer to high priority (its the only buffer used)
	mcp2515_write_register(TXB0CTRL, (1 << TXP0) | (1 << TXP1));
	
	
	//load tx0 buffers
	CAN_CS_LOW //select mcp2515
	
	spi_putc(SPI_WRITE_TX | 0x00); //start at TXB0SIDH
	
	spi_putc(p_message -> id >> 3); //write to TXBnSIDH
	spi_putc(p_message -> id << 5); //write to TXBnSIDL
	spi_putc(0x00); //write to  TXBnEID8
	spi_putc(0x00); //write to TXBnEID0
	
	
	
	if (p_message -> RTransR) //if message is a remote transmit request
	{
		//write data length and RTR bit
		spi_putc((1 << RTR) | p_message -> length); //write to TXBnDLC
	}
	else
	{
		//just write data length
		spi_putc(p_message -> length);
	}
	//write data bytes
	for (char i = 0; i < p_message -> length; i ++)
	{
		spi_putc(p_message -> data[i]); //write to TXBnDm
	}
	CAN_CS_HIGH //done loading tx 0 register
		
    // send CAN Message  
    CAN_CS_LOW
    spi_putc ( SPI_RTS | 0x01 ) ;  //request to send tx buffer 0 (1st buffer therefore write 0x01)
    CAN_CS_HIGH

	return (0); 
}


unsigned char mcp2515_read_rx_status ( void ) 
{ 
    unsigned char data; 
    // CS of the MCP2515 on low drag     
    CAN_CS_LOW     
    spi_putc ( SPI_RX_STATUS ) ;     
    data = spi_putc ( 0xFF ) ; 
    // The data is sent again repeated , . 
    // So you need only one of the two bytes evaluated     
    spi_putc ( 0xff ) ; 
    CAN_CS_HIGH
    return data;
}


CanMessage can_get_message ( void ) 
{ 
    CanMessage p_message; //create a message
    // read status 
    unsigned char status = mcp2515_read_rx_status ( ) ;
    
    //start reading message
 
    
    if ( bit_is_set ( status, 6 ) ) 
    { // message in buffer 0  
    	CAN_CS_LOW  //select mcp2515
		spi_putc(SPI_READ_RX | 0); //Start reading at RXB0SIDH
	} 
    else if ( bit_is_set ( status, 7 ) ) 
    { // message in buffer 1      
    	CAN_CS_LOW  //select mcp2515
		spi_putc(SPI_READ_RX | 2); //Start reading at RXB1SIDH
    } 
    else 
    { // Error: No new message available
		p_message.id = 0;
		return p_message;
    } 
	
	
	p_message.data[6] = 0;
	
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
    CAN_CS_HIGH
	
	
	
    return(p_message);
}
