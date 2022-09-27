#ifndef MY_BLE_CLASS_H
#define MY_BLE_CLASS_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

constexpr char kConnectionServiceUUID[] = "56a1d506-6543-4e48-b871-0aa3b62ab319";
constexpr char kConnectionCharacteristicUUID[] = "b277478b-9df4-47e3-828b-508195af695a";
constexpr char kWiFiSSIDCharacteristicUUID[] = "ae1992e5-ec92-4db1-acf0-77ad1e200217";

constexpr char kEnvironmentalSensingServiceUUID[] = "181A";
constexpr char kTemperatureCharacteristicUUID[] = "2A1C";
constexpr char kHumidityCharacteristicUUID[] = "2A6F";

class MyBLEClass {
  public:
    BLECharacteristic *pConnectionCharacteristic;
    BLECharacteristic *pWiFiSSIDCharacteristic;
    BLECharacteristic *pTemperatureCharacteristic;
    BLECharacteristic *pHumidityCharacteristic;

    MyBLEClass() {};
    void init() ;
};

#endif