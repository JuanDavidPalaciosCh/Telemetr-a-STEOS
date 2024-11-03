#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_now.h>

// Estructura para los datos que se recibirán
typedef struct datos {
  float dist_recorrida;
  float velocidad;
  float voltaje;
  float corriente;
  float acelerador;
  float temperatura;
} datos;

datos datos_telemetria;

const char* ssid = "FAMILIA_ERASO_CHAVEZ";
const char* password = "Juanda280904";
const char* mqttServer = "192.168.1.2";
const int mqttPort = 1883;
const char* mqttUser = "jpalaciosch";
const char* mqttPassword = "123456";

WiFiClient espClient;
PubSubClient client(espClient);

void OnRecv(const esp_now_recv_info* info, const uint8_t* incomingData, int len) {
  memcpy(&datos_telemetria, incomingData, sizeof(datos_telemetria));
}

void setup() {
  Serial.begin(9600);
  
  // Configuración de Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Inicializar MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }

  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("There was an error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnRecv);
}

void loop() {
  client.loop();

  // Publicar los valores recibidos por ESP-NOW
  if (datos_telemetria.velocidad != 0) { // Verifica si se recibieron datos
    client.publish("Telemetría/Velocidad", String(datos_telemetria.velocidad).c_str(), true);
    client.publish("Telemetría/Distancia", String(datos_telemetria.dist_recorrida).c_str(), true);
    client.publish("Telemetría/Corriente", String(datos_telemetria.corriente).c_str(), true);
    client.publish("Telemetría/Temperatura", String(datos_telemetria.temperatura).c_str(), true);
    client.publish("Telemetría/Voltaje", String(datos_telemetria.voltaje).c_str(), true);
    client.publish("Telemetría/Aceleración(%)", String(datos_telemetria.acelerador).c_str(), true);

    client.publish("Telemetría/Hombre Muerto", 0 ? "true" : "false", true);
  }

  delay(1000); // Publicar cada segundo
}
