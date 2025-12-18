#include <Arduino.h>

// --- PINES DE CONFIGURACIÓN ---
const int SENSOR_PIN = A0;   // Pin analógico
const int ENA_PIN = 9;       // Pin de velocidad (PWM) para el L298N
const int IN1_PIN = 8;       // Pin de dirección 1
const int IN2_PIN = 7;       // Pin de dirección 2

// --- VARIABLES PARA EL PROMEDIO ---
unsigned long tiempoAnterior = 0;
const int intervaloPromedio = 500; // Intervalo de 500 ms para calcular el promedio

long sumaLecturas = 0;      // Acumulador para la suma de las lecturas. 'long' para evitar desbordamiento.
int cantidadLecturas = 0;   // Contador para el número de lecturas tomadas.

void setup() {
  Serial.begin(9600);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  // Inicia la bomba apagada
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, 0);
}

void loop() {
  // --- LÓGICA DE PROMEDIO DE LECTURAS ---

  // 1. Acumula lecturas continuamente
  sumaLecturas += analogRead(SENSOR_PIN);
  cantidadLecturas++;

  // 2. Cada 500 ms, calcula, envía el promedio y resetea
  if (millis() - tiempoAnterior >= intervaloPromedio) {
    if (cantidadLecturas > 0) {
      // Calcula el promedio
      int promedio = sumaLecturas / cantidadLecturas;
      // Envía el valor promediado al PC
      Serial.println(promedio);
    }
    
    // Resetea las variables para el siguiente intervalo
    sumaLecturas = 0;
    cantidadLecturas = 0;
    tiempoAnterior = millis(); // Actualiza el tiempo para el próximo intervalo
  }

  // --- RECEPCIÓN DE COMANDOS DESDE EL PC ---
  // Esta sección se ejecuta en cada ciclo del loop, independientemente del promedio.
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    // Revisa si el comando recibido es para ajustar la velocidad
    if (comando.startsWith("SPEED,")) {
      int velocidad = comando.substring(6).toInt();
      velocidad = constrain(velocidad, 0, 255);

      if (velocidad > 0) {
        // Enciende el motor con la dirección y velocidad especificadas
        digitalWrite(IN1_PIN, HIGH);
        digitalWrite(IN2_PIN, LOW);
        analogWrite(ENA_PIN, velocidad);
      } else {
        // Si la velocidad es 0, apaga el motor de forma segura
        digitalWrite(IN1_PIN, LOW);
        digitalWrite(IN2_PIN, LOW);
        analogWrite(ENA_PIN, 0);
      }
    }
  }
}
