#include "esp_bt_main.h"
#include "esp_bt_device.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <ArduinoJson.h>

#define BLE_NAME "Noraxon-Trigger"
#define SERVICE_UUID "0000b181-0000-1000-8000-00805f9b34fb"
#define BLE_READ_CHR "0000cbf6-0000-1000-8000-00805f9b34fb"

const int syncPin = 2; // on firebeetle (D9)
const unsigned long triggerDuration = 1000; // Duration of the trigger signal in milliseconds (adjust as needed)
void triggerNoraxon();

bool deviceConnected = false;
BLECharacteristic triggerCharacteristic(BLE_READ_CHR, BLECharacteristic::PROPERTY_READ);

class BLECallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify...");
  }
};

 class BLEIOCallbacks : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    triggerNoraxon();
    pCharacteristic->setValue("Triggered");
    Serial.println("Sent trigger and notified twincentral");
  }
 };

void setup() {
  Serial.begin(115200);
  Serial.println(BLE_NAME);

  BLEDevice::init(BLE_NAME);
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BLECallbacks());
  
  triggerCharacteristic.setCallbacks(new BLEIOCallbacks());

  BLEService* triggerService = pServer->createService(SERVICE_UUID);
  triggerService->addCharacteristic(&triggerCharacteristic);
  triggerService->start();

  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void triggerNoraxon() {
  // Generate the trigger signal
  digitalWrite(syncPin, HIGH);
  delay(triggerDuration);
  digitalWrite(syncPin, LOW);
}

void loop() {
  while(deviceConnected) {
    delay(1000);
  }
}
