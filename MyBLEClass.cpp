#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "MyBLEClass.h"

void MyBLEClass::init() {
  Serial.println("Initalizing Device");

  BLEDevice::init("ESP32-Thermostat");

  Serial.println("Initializing Server");

  BLEServer *pServer = BLEDevice::createServer();

  Serial.println("Initializing Services and Characteristics");

  BLEService *pConnectionService = pServer->createService(kConnectionServiceUUID);
  pConnectionCharacteristic = pConnectionService->createCharacteristic(
    kConnectionCharacteristicUUID,
    BLECharacteristic::PROPERTY_READ);
  pWiFiSSIDCharacteristic = pConnectionService->createCharacteristic(
    kWiFiSSIDCharacteristicUUID,
    BLECharacteristic::PROPERTY_READ);

  BLEService *pEnvironmentalSensingService = pServer->createService(kEnvironmentalSensingServiceUUID);
  pTemperatureCharacteristic = pEnvironmentalSensingService->createCharacteristic(
    kTemperatureCharacteristicUUID,
    BLECharacteristic::PROPERTY_READ);
  pHumidityCharacteristic = pEnvironmentalSensingService->createCharacteristic(
    kHumidityCharacteristicUUID,
    BLECharacteristic::PROPERTY_READ);

  Serial.println("Starting Services");

  pConnectionService->start();
  pEnvironmentalSensingService->start();

  Serial.println("Initializing Advertising");

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(kConnectionServiceUUID);
  pAdvertising->addServiceUUID(kEnvironmentalSensingServiceUUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);

  Serial.println("Advertising");
  BLEDevice::startAdvertising();
}