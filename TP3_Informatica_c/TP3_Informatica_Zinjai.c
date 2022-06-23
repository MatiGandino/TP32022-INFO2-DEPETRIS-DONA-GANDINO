#include <stdlib.h>                         //Libreria para flujo de datos
#include <stdint.h>                         //Libreria para usar uint16_t
#include <stdio.h>                          //Libreria main

#ifdef _WIN32                               //Librerias para la utilizacion del
#include <Windows.h>                        // programa teniendo Windows o Linux
#else                                       //          "
#include <unistd.h>                         //          "
#endif                                      //          "

#include "rs232.h"                          //Libreria RS232


typedef struct {                            //Estructura donde se guardaran los valores de los sensores
	uint32_t tiempo;                        //Int donde se guarda el tiempo desde la ultima medicion
	uint16_t valor;                         //Int donde se guarda el valor del sensor
	char indic;                             //Char donde se guarda el inicial del sensor
	char charr;                             //Char para evitar diferencia de tamaño de la estructura entre C y C++
}mediciones;

union Umed {                                //Union donde se guardaran las mediciones y el envio de datos
	mediciones med;                         //Estructura con los valores de las mediciones
	byte a[sizeof(mediciones)];             //Envio de datos
}medicion;


int main(void) {
	
	int cport_nr = 3;                       //NUMERO DE PUERTO SERIE (verificar numero)
	int bdrate = 9600;                      //Velocidad del puerto: 9600 Baudios por segundo.
	char mode[] = {'8', 'N', '1', 0};       //Configuracion del orden de envio de bytes
	char letra;                             //Char que recibe la letra del sensor
	
	if(RS232_OpenComport(cport_nr, bdrate, mode, 0)){         //Si no lee el puerto el programa se cierra
		printf("No se puede abrir el puerto.\n");             //          "
		return (0);}                                          //          "

	while(1){                                                                               //While creando un loop en cada introduccion de letra
		printf("Intraduzca la inicial del sensor a leer: ");
		scanf(" %s", &letra);                                                               //Introduccion de letra del sensor (T, H, U)
		RS232_SendByte(cport_nr, letra);                                                    //Envio de la letra por el puerto
		Sleep(1000);		

		int cantidadBytes = RS232_PollComport(cport_nr, medicion.a, sizeof(mediciones));    //Importacion de los datos del sensor requerido

		if (cantidadBytes > 0){                                                             //Lectura de bytes recibidos
			printf("El sensor es %c\n", medicion.med.indic);                                //Lectura de datos recibidos 
			printf("El valor es %d\n", medicion.med.valor);                                 //          "      
			printf("Tiempo desde la ultima medicion: %d\n\n", medicion.med.tiempo);         //          "
		}	
	}

	return 0; //Cierre del programa exitoso
}

