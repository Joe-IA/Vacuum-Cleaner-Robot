
#include "src/Robot/Robot.hpp"
#include "src/Logs/Logger.h"


Robot* Robot::instance = nullptr;

Robot::Robot(
  RobotContext* context,
  BluetoothClient* btClient,
  MotorHandler* motorHandler,
  int bumperButton,
  int onOffButton,
  int vaccumPin
){
  this->context = context;
  this->btClient = btClient;
  this->motorHandler = motorHandler;
  this->bumperButton = bumperButton;
  this->onOffButton = onOffButton;
  this->vacuumPin = vaccumPin;
  Robot::instance = this;
}

int Robot::getBumperButton(){
  return this->bumperButton;
}

int Robot::getonOffButton(){
  return this->onOffButton;
}

int Robot::getVacuumPin(){
  return this->vacuumPin;
}

void Robot::handleButton(int button){
  delay(50);
  while(!digitalRead(button)){}
  delay(50);
}

void Robot::setVacuumState() {
  bool isRobotRunning = this->context->getState() == RobotState::ON;
  digitalWrite(
    vacuumPin,
     isRobotRunning? HIGH : LOW
  );
}

void IRAM_ATTR Robot::onOffButtonHandle(){
  RobotState previousState = this->context->getState(), newState;
  newState = previousState == RobotState::ON ? RobotState::OFF : RobotState::ON;
  this->context->setState(newState);
  this->btClient->notifyAppStateChange();
  handleButton(this->onOffButton);
}

MotorHandler* Robot::getMotorHandler(){
  return this->motorHandler;
}

RobotContext* Robot::getContext(){
  return this->context;
}

void Robot::onOffButtonHandleISR(){
  if(Robot::instance != nullptr)
    Robot::instance->onOffButtonHandle();
}