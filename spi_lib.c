#include "spi_lib.h"
#include <avr/io.h>
#include <avr/delay.h>

#define DDR_CS DDRB
#define PORT_CS PORTB
#define P_CS 2

#define CS_HIGH PORT_CS |= (1 << P_CS);
#define CS_LOW PORT_CS &= (1 << P_CS);
void SPI_init(void) 
{ 
    // Enable pins for the SPI interface
    DDR_SPI |= ( 1 << P_SCK ) | ( 1 << P_MOSI ) ; 
    PORT_SPI &= ~( ( 1 << P_SCK ) | ( 1 << P_MOSI ) | ( 1 << P_MISO ) ) ;
    DDR_CS |= ( 1 << P_CS ) ;
    CS_HIGH 
    // Enable the SPI master interfaces, fosc = fclk /  2 == fast :)
    SPCR = ( 1 << SPE ) | ( 1 << MSTR );
    SPSR = ( 1 << SPI2X ) ; 
}

unsigned char spi_putc ( unsigned char data ) 
{ 
    // Sends a byte
    SPDR = data; 
    // Waits for byte to send 
    while ( ! ( SPSR & ( 1 << SPIF ) ) ); 
    return SPDR; 
}
