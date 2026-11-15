#ifndef ROBOT_STATE_H
#define ROBOT_STATE_H

#include <Arduino.h>
enum class RobotState {
  OFF,
  ON
};

class RobotContext {
  private:
    RobotState state = RobotState::OFF;
    bool deviceConnected = false;
  public:
    RobotState getState();
    void setState(RobotState newState);
    bool isDeviceConnected();
    void setDeviceConnected(bool deviceConnected);
};

#endif