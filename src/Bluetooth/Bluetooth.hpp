#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "src/Context/RobotContext.hpp"

class ServerCallbacks : public BLEServerCallbacks {
  private:
    RobotContext* robotContext;
  public:
    void onConnect(BLEServer* server) override;
    void onDisconnect(BLEServer* server) override;
    ServerCallbacks(RobotContext* robotContext);
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  private:
    RobotContext* robotContext;
  public:
    void onWrite(BLECharacteristic* characteristic) override;
    CharacteristicCallbacks(RobotContext* robotContext);
};

class BluetoothClient{
  private:
    BLEServer* bleServer = nullptr;
    BLECharacteristic* bleCharacteristic = nullptr;
    ServerCallbacks serverCallbacks;
    CharacteristicCallbacks characteristicCallbacks;
    RobotContext* robotContext;
    char* deviceName;
    char* serviceUUID;
    char* characteristicUUID;

  public:
    BluetoothClient(RobotContext* robotContext, 
                         char* deviceName, 
                         char* serviceUUID, 
                         char* characteristicUUID);
    void initializeBluetooth();
    void notifyAppStateChange();
    static char messageBuffer[100];
};
#endif