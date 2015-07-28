#include "can_lib.h"
#include "spi_lib.h"
#include "mcp2515_defs.h"
#include "mcp2515_settings.h"

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
    spi_putc ( data ) ; // re-enable / CS line
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

unsigned char CAN_init(void)
{
	DDR_CAN_CS |= (1 << P_CAN_CS); //set CS to output
	CAN_CS_LOW
	spi_putc(SPI_RESET);
	_delay_ms(10); //wait for chip to reset
	CAN_CS_HIGH
	
	unsigned int error = 0;
	
	//write to all registers using settings defined above
	///also verify the settings
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
	if (mcp2515_read_register(BFPCTRL) != BFPCTRL_Setting)
	{
		error |= (1 << 6);
	}
	
	mcp2515_write_register ( TXRTSCTRL, TXRTSCTRL_Setting ) ;
	if (mcp2515_read_register(TXRTSCTRL) != TXRTSCTRL_Setting)
	{
		error |= (1 << 7);
	}
	
	
	mcp2515_bit_modify ( CANCTRL, 0xE0, 0 ) ; //exit configure mode
	
	return(error);
}

void can_send_message ( CanMessage *p_message ) //sends message using tx buffer 0
{ 
    uint8_t length = p_message-> length; 
    // ID If the message is a "Remote Transmit Request" 
    if ( p_message-> RTransR ) 
    { 
    	/* An RTR message does have a length but no data */ 
    	// message length + RTR set         
    	mcp2515_write_register ( TXB0DLC, ( 1 << RTR ) | length ) ; 
    } 
    else 
    { 
    	// Set the message length         
    	mcp2515_write_register ( TXB0DLC, length ) ; 
    	// data 
    	for ( uint8_t i = 0 ; i <length; i ++ ) 
    	{             
    		mcp2515_write_register ( TXB0D0 + i, p_message-> data [ i ] ) ; 
    	} 
    } 
    // CAN Message     
    CAN_CS_LOW     
    spi_putc ( SPI_RTS | 0x01 ) ;     
    CAN_CS_HIGH
}


uint8_t mcp2515_read_rx_status ( void ) 
{ 
    uint8_t data; 
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
    uint8_t status = mcp2515_read_rx_status ( ) ;
    if ( bit_is_set ( status, 6 ) ) 
    { // message in buffer 0        
    PORT_CS & = ~ ( 1 << P_CS ) ;    // CS Low         
    spi_putc ( SPI_READ_RX ) ; 
    	
    } 
    else if ( bit_is_set ( status, 7 ) ) 
    { // message in buffer 1         
    PORT_CS & = ~ ( 1 << P_CS ) ;    // CS Low         
    spi_putc ( SPI_READ_RX | 0x04 ) ; 
    	
    } 
    else 
    { /* Error: No new message available */ 
    return 0xff; 
    } 
    // read standard ID     
    p_message.id =  (uint16_t) spi_putc( 0xff ) << 3 ;     
    p_message.id |= (uint16_t) spi_putc( 0xff ) >> 5 ;     
    spi_putc ( 0xff ) ;     
    spi_putc ( 0xFF ) ; 
    // read length 
    p_message.length = spi_putc ( 0xff ) & 0x0f;     
    // data read 
    for ( uint8_t i = 0 ; i <length; i ++ ) 
    {         
    	p_message.data[i] = spi_putc ( 0xff ) ;
    }     
    CAN_CS_HIGH 
    if ( bit_is_set ( status, 3 ) ) 
    {         
    	p_message.RTR = 1 ; 
    	
    } else 
    {         
    	p_message.RTR = 0 ;
    	
    } 
    return(p_message);
}
