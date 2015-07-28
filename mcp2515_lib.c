#include "can_lib.h"
#include "spi_lib.h"
#include "mcp2515_defs.h"

#include <avr/io.h>
#include <avr/delay.h>

#define CNF1_Setting  (( 1 << BRP0 ) | ( 1 << BRP1 ) | ( 1 << BRP2 ) | (1 << BRP3))
#define CNF2_Setting (( 1 << BTLMODE ) | ( 1 << PHSEG11 ))
#define CNF3_Setting ( 1 << PHSEG21 )
#define CANINTE_Setting (( 1 << RX1IE ) | ( 1 << RX0IE ))
#define RXB0CTRL_Setting (( 1 << RXM1 ) | ( 1 << RXM0 ))

void mcp2515_write_register( uint8_t address, uint8_t data ) 
{ 
    //  CS of the MCP2515 on low drag
    CAN_CS_LOW
    spi_putc ( SPI_WRITE ) ;
    spi_putc ( address ) ;     
    spi_putc ( data ) ; // / CS line unlock     
    CAN_CS_HIGH 
}

void mcp2515_bit_modify ( uint8_t address, uint8_t mask, uint8_t data ) 
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
	
	mcp2515_write_register ( RXB1CTRL, ( 1 << RXM1 ) | ( 1 << RXM0 ) );
	mcp2515_write_register ( BFPCTRL, 0 ) ;
	mcp2515_write_register ( TXRTSCTRL, 0 ) ;
	mcp2515_bit_modify ( CANCTRL, 0xE0, 0 ) ;
	
	return(0);
}
