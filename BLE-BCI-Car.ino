#include <Arduino.h>
#include "BLEDevice.h"

static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID charUUID_1("beb5483e-36e1-4688-b7f5-ea07361b26a8");
static BLEUUID charUUID_2("1c95d5e3-d8f7-413a-bf3d-7a2e5d7be87e");

static boolean doConnect = false;
static boolean connected = false;
static boolean notify = false;
static boolean doScan = false;

const int leftWheelForward = 0;
const int leftWheelBackward = 1;
const int rightWheelForward = 21;
const int rightWheelBackward = 2;

static BLEAdvertisedDevice* myDevice;
BLERemoteCharacteristic* pRemoteChar_1;
BLERemoteCharacteristic* pRemoteChar_2;

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                           uint8_t* pData,
                           size_t length,
                           bool isNotify) {
  if (pBLERemoteCharacteristic->getUUID().toString() == charUUID_1.toString()) {
    uint32_t counter = pData[0];
    for (int i = 1; i < length; i++) {
      counter = counter | (pData[i] << i * 8);
    }
    Serial.print("Characteristic 1 (Notify) from server: ");
    Serial.println(counter);
    if (counter == 1) { 
      digitalWrite(leftWheelForward, HIGH);
      digitalWrite(leftWheelBackward, LOW);
      digitalWrite(rightWheelForward, LOW);
      digitalWrite(rightWheelBackward, LOW);

    } else if (counter == 2) { 
      digitalWrite(rightWheelForward, HIGH);
      digitalWrite(rightWheelBackward, LOW);
      digitalWrite(leftWheelForward, LOW);
      digitalWrite(leftWheelBackward, LOW);
    } else if (counter == 3) {
      digitalWrite(leftWheelForward, HIGH);
      digitalWrite(leftWheelBackward, LOW);
      digitalWrite(rightWheelForward, HIGH);
      digitalWrite(rightWheelBackward, LOW);
    } else if (counter == 4) {  
      digitalWrite(leftWheelForward, LOW);
      digitalWrite(leftWheelBackward, HIGH);
      digitalWrite(rightWheelForward, LOW);
      digitalWrite(rightWheelBackward, HIGH);
    } else {  
      digitalWrite(leftWheelForward, LOW);
      digitalWrite(leftWheelBackward, LOW);
      digitalWrite(rightWheelForward, LOW);
      digitalWrite(rightWheelBackward, LOW);
    }
  }
}
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }
  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());
  BLEClient* pClient = BLEDevice::createClient();
  Serial.println(" - Created client");
  pClient->setClientCallbacks(new MyClientCallback());
  pClient->connect(myDevice);
  Serial.println(" - Connected to server");
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");

  connected = true;
  pRemoteChar_1 = pRemoteService->getCharacteristic(charUUID_1);
  pRemoteChar_2 = pRemoteService->getCharacteristic(charUUID_2);
  if (connectCharacteristic(pRemoteService, pRemoteChar_1) == false)
    connected = false;
  else if (connectCharacteristic(pRemoteService, pRemoteChar_2) == false)
    connected = false;
  if (connected == false) {
    pClient->disconnect();
    Serial.println("At least one characteristic UUID not found");
    return false;
  }
  return true;
}
bool connectCharacteristic(BLERemoteService* pRemoteService, BLERemoteCharacteristic* l_BLERemoteChar) {
  if (l_BLERemoteChar == nullptr) {
    Serial.print("Failed to find one of the characteristics");
    Serial.print(l_BLERemoteChar->getUUID().toString().c_str());
    return false;
  }
  Serial.println(" - Found characteristic: " + String(l_BLERemoteChar->getUUID().toString().c_str()));
  if (l_BLERemoteChar->canNotify())
    l_BLERemoteChar->registerForNotify(notifyCallback);
  return true;
}
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } 
  }  
};   
void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
  pinMode(leftWheelForward, OUTPUT);
  pinMode(leftWheelBackward, OUTPUT);
  pinMode(rightWheelForward, OUTPUT);
  pinMode(rightWheelBackward, OUTPUT);
  digitalWrite(leftWheelForward, LOW);
  digitalWrite(leftWheelBackward, LOW);
  digitalWrite(rightWheelForward, LOW);
  digitalWrite(rightWheelBackward, LOW);
}

void loop() {
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
  if (doScan) { BLEDevice::getScan()->start(0); }
  delay(1000);
}