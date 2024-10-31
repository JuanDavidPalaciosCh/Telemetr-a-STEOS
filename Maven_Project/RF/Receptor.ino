#include <esp_now.h>
#include <WiFi.h>
// Structure to keep the temperature and humidity data
// Is also required in the client to be able to save the data directly

typedef struct datos {
  float dist_recorrida;
  float velocidad;
  float voltaje;
  float corriente;
  float acelerador;
  float temperatura;
};
// Create a struct_message called myData
datos datos_telemetria;
// callback function executed when data is received
void OnRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&datos_telemetria, incomingData, sizeof(datos_telemetria));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Distancia: ");
  Serial.println(datos_telemetria.dist_recorrida);
  Serial.print("Velocidad: ");
  Serial.println(datos_telemetria.velocidad);
  Serial.print("Voltaje: ");
  Serial.println(datos_telemetria.voltaje);
  Serial.print("Corriente: ");
  Serial.println(datos_telemetria.corriente);
  Serial.print("Acelerador: ");
  Serial.println(datos_telemetria.acelerador);
  Serial.print("Temperatura: ");
  Serial.println(datos_telemetria.temperatura);
}
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("There was an error initializing ESP-NOW");
    return;
  } else {
    Serial.println("Ok");
  }
  
  // Once the ESP-Now protocol is initialized, we will register the callback function
  // to be able to react when a package arrives in near to real time without pooling every loop.
  esp_now_register_recv_cb(OnRecv);
}
void loop() {
}
