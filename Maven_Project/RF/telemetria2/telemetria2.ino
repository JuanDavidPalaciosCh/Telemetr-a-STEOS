#include <esp_now.h>
#include <WiFi.h>
// Set the SLAVE MAC Address
uint8_t slaveAddress[] = {0xD0, 0xEF, 0x76, 0x15, 0x17, 0x30};
//D0:EF:76:15:17:30


// Structure to keep the temperature and humidity data from a DHT sensor
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
// Callback to have a track of sent messages
void OnSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nEnviar estado del mensaje:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Envio exitoso" : "Envio fallido");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(9600);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }
  // We will register the callback function to respond to the event
  esp_now_register_send_cb(OnSent);
  
  // Register the slave
  esp_now_peer_info_t slaveInfo;
  memset(&slaveInfo, 0, sizeof(slaveInfo));
  memcpy(slaveInfo.peer_addr, slaveAddress, 6);
  slaveInfo.channel = 0;  
  slaveInfo.encrypt = false;
  
  // Add slave        
  if (esp_now_add_peer(&slaveInfo) != ESP_OK){
    Serial.println("There was an error registering the slave");
    return;
  }
}

int flag = 0;

void loop() {
  // Generamos valores aleatorios para los otros parámetros
  datos_telemetria.dist_recorrida = random(0, 1000) / 10.0;  // Distancia recorrida entre 0.0 y 100.0
  datos_telemetria.voltaje = random(3000, 4200) / 1000.0;    // Voltaje entre 3.0V y 4.2V
  datos_telemetria.corriente = random(0, 500) / 1000.0;       // Corriente entre 0.0A y 0.5A
  datos_telemetria.acelerador = random(0, 1024) / 1024.0;     // Acelerador entre 0.0 y 1.0
  datos_telemetria.temperatura = random(150, 400) / 10.0;      // Temperatura entre 15.0 y 40.0 grados

  // Incrementar la velocidad linealmente hasta 100 y luego reiniciar
  if (datos_telemetria.velocidad < 100) {
    datos_telemetria.velocidad += 1;  // Incrementar la velocidad de 1 en 1
  } else {
    datos_telemetria.velocidad = 0;  // Reiniciar la velocidad cuando llegue a 100
  }

  if (flag >= 10){
    // Enviamos los datos via ESP-NOW
    esp_err_t result = esp_now_send(slaveAddress, (uint8_t *) &datos_telemetria, sizeof(datos_telemetria));
    flag = 0;
  } else {
    flag ++;
  }
  

  // Retardo para controlar la frecuencia de envío
  delay(10);
}
