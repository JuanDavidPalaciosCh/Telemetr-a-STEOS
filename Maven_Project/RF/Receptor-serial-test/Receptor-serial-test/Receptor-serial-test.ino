#include <ArduinoJson.h>  // Asegúrate de tener la librería ArduinoJson instalada

// Estructura para los datos que se enviarán
typedef struct datos {
  float dist_recorrida;
  float velocidad;
  float voltaje;
  float corriente;
  float acelerador;
  float temperatura;
} datos;

datos datos_telemetria;

void setup() {
  Serial.begin(9600);  // Configurar puerto serial a 9600 baudios

  // Inicializar el generador de números aleatorios
  randomSeed(analogRead(0));  // Inicializamos la semilla para random() con ruido de un pin analógico no conectado
}

void loop() {
  // Dejar la distancia constante
  datos_telemetria.dist_recorrida = 50;  // Deja la distancia constante en 50 (o el valor que prefieras)

  // Incrementar la velocidad linealmente hasta 100 y luego reiniciar
  if (datos_telemetria.velocidad < 100) {
    datos_telemetria.velocidad += 1;  // Incrementar la velocidad de 1 en 1
  } else {
    datos_telemetria.velocidad = 0;  // Reiniciar la velocidad cuando llegue a 100
  }

  // Asignar valores aleatorios a los otros parámetros
  datos_telemetria.voltaje = random(11, 15);  // Voltaje entre 11 y 15 voltios
  datos_telemetria.corriente = random(1, 10);  // Corriente entre 1 y 10 amperios
  datos_telemetria.acelerador = random(0, 101) / 100.0;  // Acelerador entre 0% y 100%
  datos_telemetria.temperatura = random(20, 40);  // Temperatura entre 20 y 40 grados Celsius

  // Crear un objeto JSON para enviar los datos
  StaticJsonDocument<200> doc;

  // Llenar el objeto JSON con los datos de telemetría
  doc["Velocidad"] = datos_telemetria.velocidad;
  doc["Distancia"] = datos_telemetria.dist_recorrida;
  doc["Voltaje"] = datos_telemetria.voltaje;
  doc["Corriente"] = datos_telemetria.corriente;
  doc["Aceleracion"] = datos_telemetria.acelerador;
  doc["Temperatura"] = datos_telemetria.temperatura;

  // Convertir el objeto JSON a una cadena
  String output;
  serializeJson(doc, output);

  // Enviar el JSON por el puerto serial
  Serial.println(output);

  delay(10);  // Retraso de 10 ms
}
