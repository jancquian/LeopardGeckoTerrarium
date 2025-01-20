#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//DEFINIMOS LOS PUERTOS DEL TERRARIO

#define DHTPIN_L 32
#define DHTPIN_C 33
#define DHTPIN_R 4 
#define DHTTYPE DHT11
#define KY_L 23
#define KY_R 22
#define LAMPARA 25
#define CALENTADOR 15

// Creamos los sensores KY-001 y la instancia de comuniciacion OneWire

OneWire oneWireL(KY_L);
DallasTemperature sensorL(&oneWireL);

OneWire oneWireR(KY_R);
DallasTemperature sensorR(&oneWireR);

// Creamos los sensores DHT11
DHT dht_i(DHTPIN_L, DHTTYPE);
DHT dht_c(DHTPIN_C, DHTTYPE);
DHT dht_d(DHTPIN_R, DHTTYPE);

// Creamos las varibales para alamcenar los datos de los DHT11

float humedad_izquierda = 0.0;
float temperatura_izquierda = 0.0;
float humedad_central = 0.0;
float temperatura_central = 0.0;
float humedad_derecha = 0.0;
float temperatura_derecha = 0.0;
float temperatura_sus_derecha = 0.0;
float temperatura_sus_izquierda = 0.0;
float temperatura_min = 0.0;
float temperatura_max = 20.0;

int pwm_lampara = 0;


// Aqui se pone las credenciales de la red, así como el puerto
const char* ssid = "iPhone de Juan Carlos";
const char* password = "JANC123XD";
const char* serverAddress = "172.20.10.2";
const int serverPort = 5000;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 120000;

void setup() {

  // Ponemos en marcha los sensores DHT
  dht_i.begin();
  dht_c.begin();
  dht_d.begin();

  //Inicializamos la lampara
  pinMode(LAMPARA, OUTPUT);
  digitalWrite(LAMPARA, LOW);

  //Inicilizador del calentador
  pinMode(CALENTADOR, OUTPUT);
  digitalWrite(CALENTADOR, LOW);

  //Inicializamos los KY
  sensorL.begin();
  sensorR.begin();

  //Definimos el boundrate de serial
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("._.");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  
  //PARTE DE LOS DH11
  static unsigned long tiempoAnterior = 0;  // Variable estática para almacenar el tiempo anterior
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoAnterior >= 5000) {
    // Han pasado 5 segundos, realiza las acciones necesarias
    // Leemos la humedad relativa
    humedad_izquierda = dht_i.readHumidity();
    humedad_central = dht_c.readHumidity();
    humedad_derecha = dht_d.readHumidity();
    // Leemos la temperatura en grados centígrados (por defecto)
    temperatura_izquierda = dht_i.readTemperature();
    temperatura_central = dht_c.readTemperature();
    temperatura_derecha = dht_d.readTemperature();
    // Agregado KY-001
    sensorL.requestTemperatures();
    temperatura_sus_izquierda = sensorL.getTempCByIndex(0);
    if (temperatura_sus_izquierda != DEVICE_DISCONNECTED_C){
      Serial.println(temperatura_sus_izquierda);
    }else{
      Serial.println("ERR");
    }

    sensorR.requestTemperatures();
    temperatura_sus_derecha = sensorR.getTempCByIndex(0);
    if (temperatura_sus_derecha != DEVICE_DISCONNECTED_C){
      Serial.println(temperatura_sus_derecha);
    }else{
      Serial.println("ERR");
    }
    


    // Reiniciar el contador de tiempo
    tiempoAnterior = tiempoActual;
  }
  // FIN PARTE DH11

  StaticJsonDocument<250> jsonTempAmb;
  jsonTempAmb["dato_1"] = temperatura_izquierda;
  jsonTempAmb["dato_2"] = humedad_izquierda;
  jsonTempAmb["dato_3"] = temperatura_central;
  jsonTempAmb["dato_4"] = humedad_central;
  jsonTempAmb["dato_5"] = temperatura_derecha;
  jsonTempAmb["dato_6"] = humedad_derecha;
  jsonTempAmb["dato_7"] = temperatura_sus_izquierda;
  jsonTempAmb["dato_8"] = temperatura_sus_derecha;

  String dheleven_json;
  serializeJson(jsonTempAmb, dheleven_json);

  // Configurar y enviar la solicitud HTTP
  HTTPClient http;
  String url = "http://" + String(serverAddress) + ":" + String(serverPort) + "/enviar_datos_dh";
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(dheleven_json);

  if (httpCode > 0) {
    Serial.printf("[HTTP] Código de respuesta: %d\n", httpCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.printf("[HTTP] Error al conectar: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

  // SOLICITUD GET DEL PWM DE LA LAMPARA

  String url_a = "http://" + String(serverAddress) + ":" + String(serverPort) + "/esp32_get_pwm";
  http.begin(url_a);

  int httpCode_a = http.GET();

  if (httpCode_a > 0) {
    Serial.printf("[HTTP] Código de respuesta: %d\n", httpCode_a);
    String payload_a = http.getString();
    Serial.println("El valor pwm retornado fue: ");
    Serial.println(payload_a);

    if (payload_a.length() > 0){
    pwm_lampara = payload_a.toInt();
    pwm_lampara = pwm_lampara * (255/100);
    }

  } else {
    Serial.printf("[HTTP] Error al conectar: %s\n", http.errorToString(httpCode_a).c_str());
  }

  http.end();

  // ACCION CON DATOS DE LAMPARA PWM

  analogWrite(LAMPARA, pwm_lampara);

  //PETICIONES GET DE TEMPERATURAS MAXIMA Y MINIMA 

  String url_b = "http://" + String(serverAddress) + ":" + String(serverPort) + "/esp32_get_max";
  http.begin(url_b);

  int httpCode_b = http.GET();

  if (httpCode_b > 0) {
    Serial.printf("[HTTP] Código de respuesta: %d\n", httpCode_b);
    String payload_b = http.getString();
    Serial.println("El valor max retornado fue: ");
    Serial.println(payload_b);

    if (payload_b.length() > 0){
    temperatura_max = payload_b.toFloat();
    }

  } else {
    Serial.printf("[HTTP] Error al conectar: %s\n", http.errorToString(httpCode_b).c_str());
  }

  http.end();

  String url_c = "http://" + String(serverAddress) + ":" + String(serverPort) + "/esp32_get_min";
  http.begin(url_c);

  int httpCode_c = http.GET();

  if (httpCode_c > 0) {
    Serial.printf("[HTTP] Código de respuesta: %d\n", httpCode_c);
    String payload_c = http.getString();
    Serial.println("El valor min retornado fue: ");
    Serial.println(payload_c);

    if (payload_c.length() > 0){
    temperatura_min = payload_c.toFloat();
    }

  } else {
    Serial.printf("[HTTP] Error al conectar: %s\n", http.errorToString(httpCode_c).c_str());
  }

  http.end();

  // ACCIONES DE TEMPERATURA MAX Y MINIMA (ZONA CAIENTE SENSOR R)
  if (temperatura_sus_derecha >= temperatura_max){
    digitalWrite(CALENTADOR, LOW);
  }else{
    digitalWrite(CALENTADOR, HIGH);
  }


  delay(5000);

}
