#include <IRremote.hpp>
#include <DHT.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "html.h"

constexpr int dht_pin = 16;

DHT dht(dht_pin, DHT22);

constexpr unsigned long readMillis = 2000;
unsigned long lastRead = 0;

float temperatureBuf[1440];
size_t temperatureBuf_tail = 0;
bool temperatureBuf_full = false;

float humidityBuf[1440];
size_t humidityBuf_tail = 0;
bool humidityBuf_full = false;

// Instatiate the http server on port 80
AsyncWebServer server(80);

void readSensors() {
  temperatureBuf[temperatureBuf_tail] = dht.readTemperature();
    temperatureBuf_tail++;
    if (temperatureBuf_tail == sizeof(temperatureBuf)/sizeof(float)) {
      temperatureBuf_tail = 0;
      temperatureBuf_full = true;
    }

    float h = dht.readHumidity();
    humidityBuf[humidityBuf_tail] = dht.readTemperature();
    humidityBuf_tail++;
    if (humidityBuf_tail == sizeof(humidityBuf)/sizeof(float)) {
      humidityBuf_tail = 0;
      humidityBuf_full = true;
    }
}

void writeBuffer() {
  if (temperatureBuf_full) {
    for (size_t i=temperatureBuf_tail; i<sizeof(temperatureBuf)/sizeof(float); i++) {
      // temperatureBuf[i];
    }    
  }
  for (size_t i=0; i < temperatureBuf_tail; i++) {
    // temperatureBuf[i];
  }

  if (humidityBuf_full) {
    for (size_t i=humidityBuf_tail; i<sizeof(humidityBuf)/sizeof(float); i++) {
      // humidityBuf[i];
    }    
  }
  for (size_t i=0; i < humidityBuf_tail; i++) {
    // humidityBuf[i];
  }
}

// Replaces placeholders in html
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
     return String(temperatureBuf[temperatureBuf_tail]);
  }
  return String();
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void router() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.onNotFound(notFound);
}

void setup() {
  Serial.begin(115200);

  dht.begin();

  router();
  server.begin();
}

void loop() {
  unsigned long timeNow = millis();
  if (timeNow - lastRead > readMillis) {
    readSensors();
  }
}
