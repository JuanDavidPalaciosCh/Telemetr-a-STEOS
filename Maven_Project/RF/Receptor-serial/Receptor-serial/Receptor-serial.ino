#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>  // Asegúrate de tener la librería ArduinoJson instalada

// Estructura para los datos que se enviarán
typedef struct datos {
  float dist_recorrida;
  float velocidad;
  float voltaje;
  float corriente;
  float acelerador;
  float temperatura;
};

// Crear un struct para los datos de telemetría
datos datos_telemetria;

// Callback function ejecutada cuando se reciben datos
void OnRecv(const esp_now_recv_info* recvInfo, const uint8_t *incomingData, int len) {
  // Copiar los datos recibidos en la estructura
  memcpy(&datos_telemetria, incomingData, sizeof(datos_telemetria));

  // Crear un objeto JSON para enviar los datos
  StaticJsonDocument<200> doc;

  // Llenar el objeto JSON con los datos recibidos
  doc["Distancia"] = datos_telemetria.dist_recorrida;
  doc["Velocidad"] = datos_telemetria.velocidad;
  doc["Voltaje"] = datos_telemetria.voltaje;
  doc["Corriente"] = datos_telemetria.corriente;
  doc["Acelerador"] = datos_telemetria.acelerador;
  doc["Temperatura"] = datos_telemetria.temperatura;

  // Convertir el objeto JSON a una cadena
  String output;
  serializeJson(doc, output);

  Serial.println(output);  // Mostrar el JSON enviado en el monitor serial
}

void setup() {
  // Inicializar el puerto serial
  Serial.begin(9600);

  // Configurar ESP32 en modo estación
  WiFi.mode(WIFI_STA);

  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP-NOW");
    return;
  }

  // Registrar la función de callback para recibir los datos
  esp_now_register_recv_cb(OnRecv);
}

void loop() {
}
