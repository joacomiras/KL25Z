#include "MKL25Z4.h"

void UART0_Init(uint32_t baud_rate) {
	uint32_t divisor;

	// Habilitar 
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

	// Configurar los pines TX y RX
	PORTB->PCR[22] = PORT_PCR_MUX(3); // TX
	PORTB->PCR[23] = PORT_PCR_MUX(3); // RX

	// Deshabilitar para config
	UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	// Configurar el baud rate
	divisor = (SystemCoreClock / (16 * baud_rate)); // SystemCoreClock es 24 MHz
	UART0->BDH = (divisor >> 8) & UART_BDH_SBR_MASK;
	UART0->BDL = divisor & UART_BDL_SBR_MASK;

	// Configurar UART para 8 bits sin paridad
	UART0->C1 = 0x00;

	// Habilitar post configuración
	UART0->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
}

void UART0_Send(char data) {
	// Esperar hasta que el buffer de transmisión esté vacío
	while (!(UART0->S1 & UART_S1_TDRE_MASK));
	UART0->D = data;  // Enviar 
}

char UART0_Receive(void) {
	// Esperar hasta que haya un dato disponible
	while (!(UART0->S1 & UART_S1_RDRF_MASK));
	return UART0->D;  // Devolver el dato recibido
}

void UART_SendString(const char* str) {
    while (*str) {
        // Esperar a que el registro de transmisión esté listo
        while (!(UART0->S1 & UART_S1_TDRE_MASK));
        // Enviar de a caracteres
        UART0->D = *str++;
    }
}
//Cuando termina de ejecutarse la funcíon se devuelve el string mediante la variable que se
// utilice en palabra
void UART_ReceiveString(char* palabra, uint16_t tMax) {
    int i = 0;
    
    while (i < (maxLength - 1)) {  
        //Espero a que haya un dato
        while (!(UART0->S1 & UART_S1_RDRF_MASK));
        
        // Leer el carácter recibido
        char c = UART0->D;
        
        // Verificar si es un último caracter (salto de linea y fin de carro)
        if (c == '\n' || c == '\r') {
            break;
        }
        
        // Almacenar el carácter en el buffer y avanzar
        palabra[i++] = c;
    }
    
    // Agregar el terminador nulo al final del buffer
    palabra[i] = '\0';
}
