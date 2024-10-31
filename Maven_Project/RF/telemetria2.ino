// Codigo para ESP32 del circuito de sensórica para el proyecto SHARKY
// Semillero STEOS - Universidad Nacional de Colombia
// Electeam de STEOS
// Ana María Orozco
// Juan David Palacios

// Este es para el master o ESP remitente

#include <esp_now.h>
#include <WiFi.h>


uint8_t slaveAddress[] = {0xD0, 0xEF, 0x76, 0x15, 0x60, 0x68}; //Address del slave
//D0:EF:76:15:60:68

// Se declara el struct con los datos que se van a enviar
typedef struct datos {
  float dist_recorrida;
  float velocidad;
  float voltaje;
  float corriente;
  float acelerador;
  float temperatura;
};

datos datos_telemetria;


// Callback para seguimiento de mensajes
void Envio(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nEnviar estado del mensaje:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Envio exitoso" : "Envio fallido");
}
 
void setup() {
  // Inicia Serial Monitor
  Serial.begin(9600);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Inicia ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }
  // Registra la funcion de callback para responder al evento
  esp_now_register_send_cb(Envio);
  
  // Registro del slave
  esp_now_peer_info_t slaveInfo;
  memcpy(slaveInfo.peer_addr, slaveAddress, 6);
  slaveInfo.channel = 0;  
  slaveInfo.encrypt = false;
  
  // Add slave        
  if (esp_now_add_peer(&slaveInfo) != ESP_OK){
    Serial.println("There was an error registering the slave");
    return;
  }
}
void loop() {
  // Establecer valores a enviar 
  // Aquí se debe conectar al codigo de Santiago
  datos_telemetria.dist_recorrida = 0.0;
  datos_telemetria.velocidad = 0.0;
  datos_telemetria.voltaje = 0.0;
  datos_telemetria.corriente = 0.0;
  datos_telemetria.acelerador = 0.0;
  datos_telemetria.temperatura = 0.0;
  // Envia mensaje via ESP-NOW
  esp_err_t result = esp_now_send(slaveAddress, (uint8_t *) &datos_telemetria, sizeof(datos_telemetria));
   
  if (result == ESP_OK) {
    Serial.println("The message was sent sucessfully.");
  }
  else {
    Serial.println("There was an error sending the message.");
  }
  delay(2000);
}
