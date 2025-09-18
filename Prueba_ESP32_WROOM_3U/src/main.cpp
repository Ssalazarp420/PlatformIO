#include <Arduino.h>

// Definir el pin del LED integrado
// La mayoría de las placas ESP32 tienen el LED en el pin GPIO 2
const int ledPin = 2;

// La función 'setup()' se ejecuta una vez cuando el programa se inicia
void setup() {
  // Inicializar la comunicación serial para depuración
  Serial.begin(115200);
  Serial.println("¡Iniciando prueba de ESP32!");

  // Configurar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
}

// La función 'loop()' se ejecuta repetidamente
void loop() {
  Serial.println("LED encendido");
  // Encender el LED
  digitalWrite(ledPin, HIGH); 
  // Esperar 1 segundo (1000 milisegundos)
  delay(1000); 

  Serial.println("LED apagado");
  // Apagar el LED
  digitalWrite(ledPin, LOW);
  // Esperar 1 segundo
  delay(1000); 
}