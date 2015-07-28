#ifndef spi_lib_H
#define spi_lib_H


#define DDR_SPI DDRB 
#define PORT_SPI PORTB
#define P_MISO 4 
#define P_MOSI 3 
#define P_SCK 5


void SPI_init(void);
unsigned char spi_putc ( unsigned char data );



#endif
