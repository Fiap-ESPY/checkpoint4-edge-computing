/*
------------------ FIAP --------------------
CP4 - VINHERIA
EDGE COMPUTING & COMPUTER SYSTEMS
Prof. Paulo Marcotti
Beatriz Cortez - RM561431
Bruno Alves - RM563986
Gabriel Augusto - RM564126
Gustavo Moura - RM566190
Pedro Henrique - RM563281
TEMPERATURA (), UMIDADE(Check) E LUMINOSIDADE- LDR(check)
--------------------------------------------
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

 // Pino GPIO35 do ESP32 para o DHT22
#define DHTTYPE DHT22 // Tipo de sensor DHT (DHT22)
DHT dht(15, DHTTYPE);

#define LDR_PIN 34  //LDR com resistor na porta Digital3

WiFiClient client; 
HTTPClient http;

// Credenciais
const char* ssid = "Wokwi-GUEST"; // Rede Wi-Fi
const char* password = ""; // Senha da rede Wi-Fi
const char* apiKey = "C7NRHQN984DC4CQJ"; // Write API Key
String server = "http://api.thingspeak.com/update?api_key=C7NRHQN984DC4CQJ"; // Servidor ThingSpeak

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Conectando Wifi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Wifi Conectado, Endereço IP:");
  Serial.println(WiFi.localIP());

  pinMode(LDR_PIN, INPUT);

  dht.begin();
}


void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    float temperature = dht.readTemperature();       // To store the values of tempreature
    float humidity = dht.readHumidity();    
    float luminosity = map(analogRead(LDR_PIN), 0, 4095, 0, 100);

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println(temperature);
      return;
    }

    String serverPath = server + "&field1=" + String(temperature) + "&field2=" + String(humidity) + "&field3=" + String(luminosity);

    http.begin(client, serverPath.c_str());

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("Código HTTP: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else{
      Serial.print("Código de erro: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Wifi desconectado");
  }

 delay(2000);
}