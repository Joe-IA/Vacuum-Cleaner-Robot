#include <stdio.h>
#include "src/Bluetooth/Bluetooth.hpp"
#include "src/Logs/Logger.h"

char BluetoothClient::messageBuffer[100];

BluetoothClient::BluetoothClient(RobotContext* robotContext,
                                      char* deviceName, 
                                      char* serviceUUID, 
                                      char* characteristicUUID):     
                                      serverCallbacks(robotContext),
                                      characteristicCallbacks(robotContext){
  this->deviceName = deviceName;
  this->serviceUUID = serviceUUID;
  this->characteristicUUID = characteristicUUID;
  this->robotContext = robotContext;
}

void BluetoothClient::initializeBluetooth() {
  Logger::info("Initializing BLE...\n");

  BLEDevice::init(this->deviceName);
  this->bleServer = BLEDevice::createServer();
  this->bleServer->setCallbacks(new ServerCallbacks(this->robotContext));
  BLEService* service = this->bleServer->createService(this->serviceUUID);
  this->bleCharacteristic = service->createCharacteristic(
    this->characteristicUUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );
  this->bleCharacteristic->setCallbacks(
    new CharacteristicCallbacks(this->robotContext)
  );
  this->bleCharacteristic->setValue("STOP");
  this->bleCharacteristic->addDescriptor(
    new BLE2902()
  );
  service->start();
  BLEAdvertising* advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(this->serviceUUID);
  advertising->setScanResponse(true);
  advertising->start();

  Logger::info("BLE initialized successfully\n");
  Logger::info("Device name: RoboClean\n");
  Logger::info("Waiting for connections...\n");
}

ServerCallbacks::ServerCallbacks(RobotContext* robotContext){
  this->robotContext = robotContext;
}

void ServerCallbacks::onConnect(BLEServer* server){
    this->robotContext->setDeviceConnected(true);
    Logger::info("BLE DEVICE CONNECTED\n");
}

void ServerCallbacks::onDisconnect(BLEServer* server){
    this->robotContext->setDeviceConnected(false);
    Logger::info("BLE DEVICE DISCONNECTED\n");
}

CharacteristicCallbacks::CharacteristicCallbacks(RobotContext* robotContext){
  this->robotContext = robotContext;
}

void CharacteristicCallbacks::onWrite(BLECharacteristic* characteristic){
  String receivedCommand = characteristic->getValue().c_str();
  receivedCommand.trim();
  sprintf(BluetoothClient::messageBuffer, "Received command: %s\n", receivedCommand);
  Logger::info(BluetoothClient::messageBuffer);
  if (receivedCommand == "ON") {
    this->robotContext->setState(RobotState::ON);
  }
  else if (receivedCommand == "OFF") {
    this->robotContext->setState(RobotState::OFF);
  }
  else {
    Logger::info("Unknown command\n");
  }
}

void BluetoothClient::notifyAppStateChange(){
  bool isRobotRunning = this->robotContext->getState() == RobotState::ON;
  String status = isRobotRunning ? "ON" : "OFF";
  if (bleCharacteristic != nullptr) {
    bleCharacteristic->setValue(status.c_str());
    if(this->robotContext->isDeviceConnected())
      bleCharacteristic->notify();
  }
}