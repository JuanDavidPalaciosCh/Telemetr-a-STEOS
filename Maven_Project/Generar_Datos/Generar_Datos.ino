#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Iphone de Santiago ";
const char* password = "123456789";
const char* mqttServer = "172.20.10.3";
const int mqttPort = 1883;
const char* mqttUser = "smarinbe";
const char* mqttPassword = "SMBmue53";

WiFiClient espClient;
PubSubClient client(espClient);

// Variables para almacenar los datos generados
float velocidad;
float distancia;
float corriente;
float temperatura;
float voltaje;
float porcentajeAceleracion;
bool HM;

void setup() {
  // Inicialización de pines, configuraciones, etc.
  // ...
   Serial.begin(115200);
   //Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("...................................");

  Serial.print("Connecting to WiFi.");
  while (WiFi.status() != WL_CONNECTED)
       {  delay(500);
          Serial.print(".") ;
       }
  Serial.println("Connected to the WiFi network");
    client.setServer(mqttServer, mqttPort);
while (!client.connected())
{      Serial.println("Connecting to MQTT...");
       if (client.connect("ESP32Client", mqttUser, mqttPassword ))
           Serial.println("connected");
       else
       {   Serial.print("failed with state ");
           Serial.print(client.state());
           delay(2000);
       }
}
}

void loop() {

  client.loop();
  /*
     char str[16];
     sprintf(str, "%u", random(100));

     client.publish("Prosensor/CO2", str);
     Serial.println(str);
     delay(500);
  */
  // Generar datos aleatorios
  
  velocidad = random(0, 100); // Velocidad en km/h (ejemplo)
  distancia = velocidad / 3600; // Distancia recorrida en km (acumulativa)
  corriente = random(0, 10); // Corriente en amperios (ejemplo)
  temperatura = random(20, 40); // Temperatura en grados Celsius (ejemplo)
  voltaje = random(44.4, 55.4); // Voltaje en voltios (ejemplo)
  porcentajeAceleracion = random(0, 100); // Porcentaje de aceleración (ejemplo)
  HM = static_cast<bool>(random(0, 2)); // Genera true o false

  // Imprimir los datos generados (puedes enviarlos por Bluetooth, mostrar en una pantalla, etc.)
  Serial.print("Velocidad: ");
  Serial.print(velocidad);
  client.publish("Telemetría/Velocidad",String(velocidad).c_str(),true);
  Serial.print(" km/h | Distancia: ");

  Serial.print(distancia);
  client.publish("Telemetría/Distancia",String(distancia).c_str(),true);
  Serial.print(" km | Corriente: ");
 
  Serial.print(corriente);
  client.publish("Telemetría/Corriente",String(corriente).c_str(),true);
  Serial.print(" A | Temperatura: ");
  
  Serial.print(temperatura);
  client.publish("Telemetría/Temperatura",String(temperatura).c_str(),true);
  Serial.print(" °C | Voltaje: ");
  
  Serial.print(voltaje);
  client.publish("Telemetría/Voltaje",String(voltaje).c_str(),true);
  Serial.print(" V | Aceleración: ");
  
  Serial.print(porcentajeAceleracion);
  client.publish("Telemetría/Aceleración(%)",String(porcentajeAceleracion).c_str(),true);
  Serial.println("%");

  Serial.print(HM ? "true" : "false");
  client.publish("Telemetría/Hombre Muerto", HM ? "true" : "false", true);
  Serial.println("|");
  delay(1000); // Esperar 1 segundo antes de generar nuevos datos
}