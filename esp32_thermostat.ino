#include <IRremote.hpp>
#include <DHT.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#include "secrets.h"

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC "device/%s/data"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

char awsIoTPublishTopic[50];

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

constexpr int dht_pin = 16;

DHT dht(dht_pin, DHT22);

constexpr unsigned long readMillis = 60 * 1000;

TaskHandle_t EnvironmentalDataPublishingTask;
TaskHandle_t ClientLoopTask;

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["temperature"] = dht.readTemperature();
  doc["humidity"] = dht.readHumidity();
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  Serial.print("Publishing to ");
  Serial.print(awsIoTPublishTopic);
  Serial.print(": ");
  Serial.println(jsonBuffer);
  client.publish(awsIoTPublishTopic, jsonBuffer);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

//  StaticJsonDocument<200> doc;
//  deserializeJson(doc, payload);
//  const char* message = doc["message"];
}

void environmentalDataPublishing_task( void * parameter )
{
  Serial.print("Environmental data publishing task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    // TODO: Figure out why first data point is always null
    publishMessage();
    vTaskDelay(readMillis / portTICK_PERIOD_MS);
  }
}

void clientLoop_task( void * parameter )
{
  Serial.print("Client loop task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    client.loop();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  sprintf(awsIoTPublishTopic, AWS_IOT_PUBLISH_TOPIC, THINGNAME);

  connectAWS(); 

  dht.begin();
  delay(1000);

  xTaskCreatePinnedToCore(environmentalDataPublishing_task, "EnvironmentalDataPublishingTask", 20 * 1024, NULL, 1, &EnvironmentalDataPublishingTask, 0);
  delay(100);                   
  xTaskCreatePinnedToCore(clientLoop_task, "ClientLoopTask", 20 * 1024, NULL, 1, &ClientLoopTask, 0);                         
}

void loop() {

}
