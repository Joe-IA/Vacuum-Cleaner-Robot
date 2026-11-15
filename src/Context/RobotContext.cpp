#include "src/Context/RobotContext.hpp"

bool RobotContext::isDeviceConnected(){
    return this->deviceConnected;
}

void RobotContext::setDeviceConnected(bool deviceConnected){
    this->deviceConnected = deviceConnected;
}

RobotState RobotContext::getState(){
    return this->state;
}

void RobotContext::setState(RobotState newState){
    this->state = newState;
}