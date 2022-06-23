#include <DHT.h>                            //Librerias humedad
#include <DHT_U.h>                          //       "
#include <stdint.h>                         //Libreria para usar uint16_t
#define SENSORTEM A0                        //Variable de sensor de temperatura


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
  byte a[sizeof(mediciones)];               //Envio de datos
}temp, hum, ult;                            
                                          

void setup(){
  Serial.begin(9600);                       //Inicializar el monitor serie
  
  dht.begin ();                             //Inicialización del sensor de humedad
  
  pinMode (TRIG, OUTPUT);                   //Salida ultrasonido
  pinMode (ECO, INPUT);                     //Entrada ultrasonido
  
  tiempo0 = 0;                              //Inicializamos tiempo
}


void loop() {
    if(Serial.available()){
    letra = Serial.read();
    
  //Datos de sensor temperatura----------------------------------------------   
      if (letra == 't' || letra == 'T'){
      TEM = analogRead(SENSORTEM); 
      int temperatura = (((TEM * 5000.0) / 1023)/10);                                  
      temp.med.valor = temperatura;
      temp.med.tiempo = millis() - tiempo0;
      temp.med.indic = 'T'; 
      for (unsigned long long i = 0; i < sizeof(mediciones); i++){
        Serial.write (temp.a[i]); }
      }
  
  //Datos de sensor humedad--------------------------------------------------     
      if (letra == 'h' || letra == 'H'){
      uint16_t HUMEDAD = dht.readHumidity();  
      hum.med.tiempo = millis() - tiempo0;
      hum.med.valor = HUMEDAD;
      hum.med.indic = 'H'; 
      for (unsigned long long i = 0; i < sizeof(mediciones); i++){
        Serial.write (hum.a[i]); }
      }
  
  //Datos de sensor ultrasonido----------------------------------------------
      if (letra == 'u' || letra == 'U'){
      digitalWrite(TRIG, HIGH);
      delay (1);                                                              //Funcionamiento sensor ultrasonido
      digitalWrite (TRIG, LOW);
      DURACION = pulseIn (ECO, HIGH);                                         //               "
      DISTANCIA = DURACION / 58.2;
  
      ult.med.valor = DISTANCIA;
      ult.med.tiempo = millis() - tiempo0;
      ult.med.indic = 'U';
      for (unsigned long long i = 0; i < sizeof(mediciones); i++){
        Serial.write (ult.a[i]); }
      }
  
  //-------------------------------------------------------------------------
      tiempo0 = millis();                                                     //Valorizacion tiempo respecto a millis
      
    }
}
