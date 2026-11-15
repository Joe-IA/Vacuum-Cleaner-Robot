#include "src/Logs/Logger.h"

void Logger::startLogger(int baudRate){
      Serial.begin(baudRate);
}

void Logger::info(char* info){
    Serial.print(info);
}