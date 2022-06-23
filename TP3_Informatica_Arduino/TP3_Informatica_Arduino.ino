#include <DHT.h>                          //Librerias humedad
#include <DHT_U.h>                        //       "
#include <stdint.h> 
#define SENSORTEM A0


//size_t n;                                  //
//uint8_t letra[1];
char letra;
int SENSOR = 2, i;
DHT dht (SENSOR, DHT11);

unsigned long tiempo0;                      //Variable que almacena la función millis. 

float TEM;                                  //Valor analogico sensor temperatura
//float valtem = ((TEM * 5000.0) / 1024)/10;

int TRIG = 10;                              //Disparador sensor ultrasonido
int ECO = 9;                                //Receptor sensor ultrasonido
int DURACION;                               //Variable que almacena el tiempo de rebote (ultrasonido)
int DISTANCIA;                              //Calculo de distancia (ultrasonido)


struct mediciones {  
    uint32_t tiempo;
    uint16_t valor;
    char indic;
    char charr; 
}; 

union Umed {
  mediciones med;
  byte a[sizeof(mediciones)];
}temp, hum, ult;
                                          

void setup(){
  Serial.begin(9600);                         //Inicializar el monitor serie
  
  dht.begin ();                               //Inicialización del sensor de humedad
  
  pinMode (TRIG, OUTPUT);                     //Salida ultrasonido
  pinMode (ECO, INPUT);                       //Entrada ultrasonido
  
  tiempo0 = 0;                                //Inicializamos tiempo
}

void loop() {
    if(Serial.available()){
//      letra = 'J';
    letra = Serial.read();
    
  //Datos de sensor temperatura----------------------------------------------   
      if (letra == 'T'){
      TEM = analogRead(SENSORTEM);                                   
      temp.med.valor = ((TEM * 5000.0) / 1024)/10;
      temp.med.tiempo = millis() - tiempo0;
      temp.med.indic = 'T'; 
      for (unsigned long long i = 0; i < sizeof(mediciones); i++){
        Serial.write (temp.a[i]); }
      }
  
  //Datos de sensor humedad--------------------------------------------------     
      if (letra == 'H'){
      uint16_t HUMEDAD = dht.readHumidity();  
      hum.med.tiempo = millis() - tiempo0;
      hum.med.valor = HUMEDAD;
      hum.med.indic = 'H'; 
      for (unsigned long long i = 0; i < sizeof(mediciones); i++){
        Serial.write (hum.a[i]); }
      }
  
  //Datos de sensor ultrasonido----------------------------------------------
      if (letra == 'U'){
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
