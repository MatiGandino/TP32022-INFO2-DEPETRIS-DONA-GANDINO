#include <DHT.h>                            //Librerias humedad
#include <DHT_U.h>                          //       "
#include <stdint.h>                         //Libreria para usar uint16_t
#define SENSORTEM A0                        //Definimos PIN de entrada analogica para la variable sensor de temperatura


char letra;                                 //Char que recibe la letra del sensor

int SENSOR = 2, i;                          //Variables de sensor humedad
DHT dht (SENSOR, DHT11);                    //        "

unsigned long tiempo0;                      //Variable que almacena la función millis. 

float TEM;                                  //Valor analogico sensor temperatura

int TRIG = 10;                              //Disparador sensor ultrasonido
int ECO = 9;                                //Receptor sensor ultrasonido
int DURACION;                               //Variable que almacena el tiempo de rebote (ultrasonido)
int DISTANCIA;                              //Calculo de distancia (ultrasonido)


struct mediciones{                          //Estructura donde se guardaran los valores de los sensores
    uint32_t tiempo;                        //Int donde se guarda el tiempo desde la ultima medicion
    uint16_t valor;                         //Int donde se guarda el valor del sensor 
    char indic;                             //Char donde se guarda el inicial del sensor
    char charr;                             //Char para evitar diferencia de tamaño de la estructura entre C y C++
}; 

union Umed {                                //Union donde se guardaran las mediciones y el envio de datos      
  mediciones med;                           //Estructura con los valores de las mediciones
  byte a[sizeof(mediciones)];               //Arreglo de tipo Byte que se utiliza para el envio de datos
}temp, hum, ult;                            
                                          

void setup(){
  Serial.begin(9600);                       //Inicializamos el puerto serie en 9600 Baudios
  
  dht.begin ();                             //Inicialización del sensor de humedad
  
  pinMode (TRIG, OUTPUT);                   //Salida ultrasonido
  pinMode (ECO, INPUT);                     //Entrada ultrasonido
  
  tiempo0 = 0;                              //Inicializamos el referenciamiento del tiempo
}


void loop() {
    if(Serial.available()){                 //Si existe una lectura de datos desde el puerto serie habilita lo que hay dentro, sino no
    letra = Serial.read();                  //Realiza la lectura de lo que se envia por el puerto serie y lo almacena como char
    
  //Datos de sensor temperatura----------------------------------------------   
      if (letra == 't' || letra == 'T'){                                //Evalua la letra recibida
      TEM = analogRead(SENSORTEM);                                      //Lee y guarda el valor del sensor de temperatura
      int temperatura = (((TEM * 5000.0) / 1023)/10);                   //Convierte el valor del sensor y lo guarda en otra varialble                      
      temp.med.valor = temperatura;                                     //Asigna el valor de "temperatura" a la variable de la estructura
      temp.med.tiempo = millis() - tiempo0;                             //Define el tiempo desde la ultima medicion
      temp.med.indic = 'T';                                             //Guarda el caracter al que indica el sensor
      for (unsigned long long i = 0; i < sizeof(mediciones); i++){
        Serial.write (temp.a[i]); }                                     //Envia los datos guardados y compartidos en la union a traves de un arreglo
      }
  
  //Datos de sensor humedad--------------------------------------------------     
      if (letra == 'h' || letra == 'H'){                                //Evalua la letra recibida
      uint16_t HUMEDAD = dht.readHumidity();                            //Lee y guarda el valor del sensor de humedad
      hum.med.tiempo = millis() - tiempo0;                              //Define y asigna el tiempo desde la ultima medicion
      hum.med.valor = HUMEDAD;                                          //Asigna el valor de "HUMEDAD" a la variable de la estructura
      hum.med.indic = 'H';                                              //Asigna el caracter al que indica el sensor
      for (unsigned long long i = 0; i < sizeof(mediciones); i++){          
        Serial.write (hum.a[i]); }                                      //Envia los datos guardados y compartidos en la union a traves de un arreglo
      }
  
  //Datos de sensor ultrasonido----------------------------------------------
      if (letra == 'u' || letra == 'U'){                                //Evalua la letra recibida
      digitalWrite(TRIG, HIGH);                                         //Funcionamiento sensor ultrasonido
      delay (1);                                                        //               "
      digitalWrite (TRIG, LOW);                                         //               " 
      DURACION = pulseIn (ECO, HIGH);                                   //               "
      DISTANCIA = DURACION / 58.2;                                      //Asigna la medicion del sensor ultrasonido a la variable "DISTANCIA"
  
      ult.med.valor = DISTANCIA;                                        //Asigna el valor de "DISTANCIA" a la variable de la estructura                               
      ult.med.tiempo = millis() - tiempo0;                              //Define y asigna el tiempo desde la ultima medicion
      ult.med.indic = 'U';                                              //Asigna el caracter al que indica el sensor
      for (unsigned long long i = 0; i < sizeof(mediciones); i++){
        Serial.write (ult.a[i]); }                                      //Envia los datos guardados y compartidos en la union a traves de un arreglo
      }
  
  //-------------------------------------------------------------------------
      tiempo0 = millis();                                                     //Valorizacion tiempo respecto a millis
      
    }
}
