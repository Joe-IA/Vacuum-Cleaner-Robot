#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "src/Bluetooth/Bluetooth.hpp"
#include "src/Motion/Motores.hpp"
#include "src/Context/RobotContext.hpp"

class Robot {
  private:
    RobotContext* context;
    BluetoothClient* btClient;
    MotorHandler* motorHandler;
    int bumperButton, onOffButton, vacuumPin;
    static Robot* instance;
  public:
    Robot(
      RobotContext* context,
      BluetoothClient* btClient,
      MotorHandler* motorHandler,
      int bumperButton,
      int onOffButton,
      int vaccumPin
    );

    static void onOffButtonHandleISR();
    int getBumperButton();
    int getonOffButton();
    int getVacuumPin();
    void handleButton(int button);
    void setVacuumState();
    void IRAM_ATTR onOffButtonHandle();
    MotorHandler* getMotorHandler();
    RobotContext* getContext();

};

#endif