#include "I2C.h"

#define I2C_W 0
#define I2C_R 1

void I2C0_Init(void) {
    SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;  // reloj para I2C0
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; 

    // Configurar los pines como I2C
    PORTB->PCR[2] = PORT_PCR_MUX(2);    // SCL
    PORTB->PCR[3] = PORT_PCR_MUX(2);    // SDA 

    // Configuración de velocidad
    I2C0->F = I2C_F_ICR(0x14) | I2C_F_MULT(0); // 100 kHz
    I2C0->C1 |= I2C_C1_IICEN_MASK;             // Habilitar
}
// bit Start
void I2C_Start(void) {
    I2C0->C1 |= I2C_C1_TX_MASK;       // Poner en modo transmisión
    I2C0->C1 |= I2C_C1_MST_MASK;      // START
}
//bit Stop
void I2C_Stop(void) {
    I2C0->C1 &= ~I2C_C1_MST_MASK;     // STOP
    I2C0->C1 &= ~I2C_C1_TX_MASK;      // Poner en modo recepción
}
//Write
void I2C_WriteByte(uint8_t data) {
    I2C0->D = data;                   // Enviar dato
    while (!(I2C0->S & I2C_S_IICIF_MASK)); // Esperar a que se complete la transferencia - flag -
    I2C0->S |= I2C_S_IICIF_MASK;      // Limpio flag
}
//Read 1-> ack 0->nack
uint8_t I2C_ReadByte(uint8_t ack) {
    if (ack) {
        I2C0->C1 &= ~I2C_C1_TXAK_MASK; // Enviar ACK después de recibir un byte
    } else {
        I2C0->C1 |= I2C_C1_TXAK_MASK;  // Enviar NACK después de recibir el último byte
    }

    while (!(I2C0->S & I2C_S_IICIF_MASK)); // Esperar a que se complete la transferencia - flag -
    I2C0->S |= I2C_S_IICIF_MASK;           // Limpio el flag
    return I2C0->D;                   
}
//Write, pero más accesible para poner el address y data
void I2C_WriteRegister(uint8_t slaveAdd, uint8_t regAdd, uint8_t data) {
    I2C_Start();
    I2C_WriteByte((slaveAdd << 1) | I2C_W); 
    I2C_WriteByte(reg_Add);                       // Dirección de memoria donde se guarde
    I2C_WriteByte(data);                              
    I2C_Stop();
}
//Read, pero más accesible para poner el address y data
uint8_t I2C_ReadRegister(uint8_t slaveAdd, uint8_t regAdd) {
    uint8_t data;
    I2C_Start();
    I2C_WriteByte((slaveAdd << 1) | I2C_W); 
    I2C_WriteByte(regAdd);                       // Dirección de memoria donde se guarde
    I2C_Start();                                 
    I2C_WriteByte((slaveAdd << 1) | I2C_R);  
    data = I2C_ReadByte(0);                      // Leer dato con NACK
    I2C_Stop();
    return data;
}
//regAdd es la direccion de memoria donde se almacena el dato. Se encuentra en datasheet (MemoryMap - RegisterMap)