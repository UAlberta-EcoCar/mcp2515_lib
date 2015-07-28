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
	
	
	//write to all registers using settings defined above
	///also verify the settings
	mcp2515_write_register ( CNF1 , CNF1_Setting);
	if (mcp2515_read_register(CNF1) != CNF1_Setting)
	{
		return(1);
	}
	
	mcp2515_write_register ( CNF2, CNF2_Setting );
	if (mcp2515_read_register(CNF2) != CNF2_Setting)
	{
		return(3);
	}
	
	mcp2515_write_register ( CNF3, CNF3_Setting );
	if (mcp2515_read_register(CNF3) != CNF3_Setting)
	{
		return(4);
	}
	
	mcp2515_write_register ( CANINTE, CANINTE_Setting);
	if (mcp2515_read_register(CANINTE) != CANINTE_Setting)
	{
		return(5);
	}
	
	mcp2515_write_register ( RXB0CTRL, RXB0CTRL_Setting );
	if(mcp2515_read_register(RXB0CTRL) != RXB0CTRL_Setting)
	{
		return(6);
	}
	
	mcp2515_write_register ( RXB1CTRL, RXB1CTRL_Setting );
	if(mcp2515_read_register(RXB1CTRL) != RXB1CTRL_Setting )
	{
		return(7);
	}
	
	mcp2515_write_register ( BFPCTRL, BFPCTRL_Setting ) ;
	if (mcp2515_read_register(BFPCTRL) != BFPCTRL_Setting)
	{
		return(8);
	}
	
	mcp2515_write_register ( TXRTSCTRL, TXRTSCTRL_Setting ) ;
	if (mcp2515_read_register(TXRTSCTRL) != TXRTSCTRL_Setting)
	{
		return(9);
	}
	
	
	mcp2515_bit_modify ( CANCTRL, 0xE0, 0 ) ; //exit configure mode
	
	return(0);
}


