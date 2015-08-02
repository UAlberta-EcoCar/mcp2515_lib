#ifndef SPI_LIB_H
#define SPI_LIB_H


//atmega 328p spi port is on port b
#define DDR_SPI DDRB 
#define PORT_SPI PORTB
//miso is pin 4
#define P_MISO 4 
//mosi is pin 3
#define P_MOSI 3 
//sck is pin 5
#define P_SCK 5

//sets up spi bus
void spi_init(void);

//sends/receives data over spi
unsigned char spi_putc ( unsigned char data );


#endif
