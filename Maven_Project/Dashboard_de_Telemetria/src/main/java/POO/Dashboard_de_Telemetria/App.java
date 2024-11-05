package POO.Dashboard_de_Telemetria;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.json.JSONObject;

import com.fazecast.jSerialComm.SerialPort;

import Datos.Pista;
import Datos.Telemetria;
import Datos.Vehiculo;
import IU.GUI;

public class App {

    public static void main(String[] args) {
        // Establecemos la ruta de la imagen para los vehículos
        String path = System.getProperty("user.dir");
        Telemetria telemetria = new Telemetria(0, 0, 0, 0, 0, false);
        Vehiculo Sharky = new Vehiculo("Sharky", "VTE", 3, telemetria, path + "/" + "Maven_Project/Dashboard_de_Telemetria/Img/Sharky.png", "Santiago", 80.0);
        Vehiculo McUN = new Vehiculo("McUN", "VTE", 3, telemetria, path + "/" + "Maven_Project/Dashboard_de_Telemetria/Img/McUN.png", "Juliana", 65.0);
        GUI gui = new GUI(telemetria, Sharky, McUN);
        gui.setVisible(true);

        // Configuración del puerto serial
        SerialPort[] ports = SerialPort.getCommPorts();
        System.out.println("Puertos seriales disponibles:");
        for (SerialPort port : ports) {
            System.out.println(port.getSystemPortName());
        }
        
        // Asumiendo que el primer puerto disponible es el correcto
        SerialPort comPort = ports[0];  // Cambia esto si tienes otro puerto que usar
        
        // Abre el puerto serial
        if (!comPort.openPort()) {
            System.out.println("No se pudo abrir el puerto: " + comPort.getSystemPortName());
            return;
        }
        
        System.out.println("Puerto abierto: " + comPort.getSystemPortName());

        // Configuración de parámetros de comunicación
        comPort.setComPortParameters(9600, 8, 1, 0);  // 9600 baudios, 8 bits, 1 bit de parada, sin paridad
        // Espera un momento para que el puerto serial se configure correctamente
        try {
            Thread.sleep(5000);  // 1 segundo de espera
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        comPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING, 500, 0);  // Timeout de 10 ms para lectura

        try {
            // Crear un InputStream con un BufferedReader para leer línea por línea
            InputStream inputStream = comPort.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
            String dataReceived;

            while (true) {
                // Intentamos leer una línea completa
                dataReceived = reader.readLine();

                if (dataReceived != null && !dataReceived.trim().isEmpty()) {
                    System.out.println("Datos recibidos: " + dataReceived);

                    // Validar si los datos recibidos son un JSON válido
                    try {
                        // Verificar si el texto recibido empieza con '{' y termina con '}'
                        if (dataReceived.trim().startsWith("{") && dataReceived.trim().endsWith("}")) {
                            // Crear un objeto JSON a partir de los datos recibidos
                            JSONObject json = new JSONObject(dataReceived);

                            // Actualizar los valores de telemetría
                            if (json.has("Velocidad")) {
                                telemetria.setVelocidad(json.getDouble("Velocidad"));
                            }
                            if (json.has("Distancia")) {
                                telemetria.setDistanciaRecorrida(json.getDouble("Distancia"));
                            }
                            if (json.has("Voltaje")) {
                                telemetria.setVoltaje(json.getDouble("Voltaje"));
                            }
                            if (json.has("Corriente")) {
                                telemetria.setCorriente(json.getDouble("Corriente"));
                            }
                            // El código para Aceleracion lo dejamos comentado como ya habías indicado
                            // if (json.has("Aceleracion")) {
                            //     telemetria.setAceleracion(json.getDouble("Aceleracion"));
                            // }
                            if (json.has("Temperatura")) {
                                telemetria.setTemperatura(json.getDouble("Temperatura"));
                            }

                            // Calcular potencia si se tienen los valores de corriente y voltaje
                            telemetria.setPotencia(telemetria.getCorriente(), telemetria.getVoltaje());
                            telemetria.actualizarPotencia(telemetria.getPotencia());

                            // Actualizar la interfaz gráfica
                            gui.updateData(telemetria);
                            gui.actualizarDatosTelemetria();

                            // Imprimir los datos actualizados en la consola
                            System.out.println("Updated Telemetria: " +
                                    "Velocidad=" + telemetria.getVelocidad() +
                                    ", DistanciaRecorrida=" + telemetria.getDistanciaRecorrida() +
                                    ", Corriente=" + telemetria.getCorriente() +
                                    ", Voltaje=" + telemetria.getVoltaje() +
                                    ", Temperatura=" + telemetria.getTemperatura() +
                                    ", Aceleracion=" + 0 +
                                    ", Potencia=" + telemetria.getPotencia());
                        } else {
                            System.err.println("Error: JSON mal formado o incompleto recibido.");
                        }
                    } catch (Exception e) {
                        System.err.println("Error al procesar el JSON: " + e.getMessage());
                    }
                } else {
                    System.out.println("Esperando datos...");
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            comPort.closePort();  // Cerrar el puerto serial cuando termine
        }
    }

    public static void definirPista(String Nombre, String Ubicacion, double Distancia) {
        Pista pista = new Pista(Nombre, Ubicacion, Distancia);
    }
}
