#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
class Logger{
    public:
        static void startLogger(int baudRate);
        static void info(char* message);
};
#endif