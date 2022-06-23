#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"



typedef struct {  
	uint32_t tiempo;
	uint16_t valor;
	char indic;
	char charr; 
}mediciones;

union Umed {
	mediciones med;
	byte a[sizeof(mediciones)];
}medicion;


int main(void) {
	
	int cport_nr = 5; //NUMERO DE PUERTO SERIE (verificar numero)
	int bdrate = 9600;
	char mode[] = {'8', 'N', '1', 0};
	char letra;

	if(RS232_OpenComport(cport_nr, bdrate, mode, 0)){
		printf("No se puede abrir el puerto.\n");
		return (0);}
	
	
while(1){
	printf("Intraduzca la inicial del sensor a leer: ");
	scanf(" %c", &letra);
	RS232_SendByte(cport_nr, letra);

	
	if (RS232_SendByte(cport_nr, letra) == 1){
		printf("ERROR!!!"); }
	
	Sleep(1000);
	
	int cantidadBytes = RS232_PollComport(cport_nr, medicion.a, sizeof(mediciones));
	
	if (cantidadBytes > 0){
	printf("El sensor es %c\n", medicion.med.indic);
	printf("El valor es %d\n", medicion.med.valor);
	printf("Tiempo desde la ultima medicion: %d\n\n", medicion.med.tiempo);
	cantidadBytes = 0;
	}
}

return 0;
}

