/*
 * Proyecto Balam 2020 - Universidad Galileo - Grupo Intelecto
 * Desarrollando mi primera experiencia IoT 
 * 
 * Código base para utilización de una aplicación de appinventor y el sensor DHT
 * Código desarrollado por: Tesla LAB
 * Rodrigo Canek - Junio 2020
*/

#include "BluetoothSerial.h" //Librería bluetooth a utilizar


BluetoothSerial ESP_com; //Definimos nuestro objeto bluetooth 

int recibido;

//Definimos el tipo de sensor a utilizar
 
// Definimos el pin donde esta conectado el sensor

               
//Inicializamos el sensor y definimos variables globales.


void setup() {
  Serial.begin(115200); 
  ESP_com.begin("Proyecto Balam - 2020"); //El nombre de nuestro dispositivo

 
}


String s_temperatura, s_humedad;
void loop() {
 


 

  if (ESP_com.available()) //Revisamos si existe una conexión activa
  {
    ESP_com.println(1 + ","+ 2);    
  }
  delay(350);
}