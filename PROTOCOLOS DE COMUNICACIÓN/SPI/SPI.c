#include "MKL25Z4.h"

// Para SPI 0
void SPI_Init(void) {
    // clock SPI y puerto
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; 
    SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;  

    // Configurar pines como SCK MOSI MISO
    PORTD->PCR[0] = PORT_PCR_MUX(2);   //  SCK
    PORTD->PCR[2] = PORT_PCR_MUX(2);   //  MOSI
    PORTD->PCR[3] = PORT_PCR_MUX(2);   //  MISO

    // Configuracion
    SPI0->C1 = 0x50;  // Habilitar el SPI
    SPI0->BR = SPI_BR_SPPR(0) | SPI_BR_SPR(2);  // Establecer la velocidad de reloj en 6MHz
    /* SPPR(0) | SPR(1) -> 12MHz */
    /* SPPR(1) | SPR(3) -> 3MHz */
}

void SPI_Send(uint8_t data) {
    while (!(SPI0->S & SPI_S_SPTEF_MASK));  // Esperar a que el registro de transmisión esté vacío
    SPI0->D = data;  // Enviar
}


uint8_t SPI_Receive(void) {
    while (!(SPI0->S & SPI_S_SPRF_MASK));  // Esperar a que se reciba un byte
    return SPI0->D;  // Leer 
}