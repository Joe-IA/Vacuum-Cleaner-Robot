#include <stdio.h>
#include "src/Motion/Motores.hpp"
#include "src/Logs/Logger.h"

MotorHandler* MotorHandler::instance = nullptr;
char MotorHandler::messageBuffer[100];

MotorHandler::MotorHandler(
                                int IN1,
                                int IN2,
                                int IN3,
                                int IN4,
                                int ENC_IZQ_A,
                                int ENC_IZQ_B,
                                int ENC_DER_A,
                                int ENC_DER_B,
                                int CUENTAS_GIRO_90,
                                int CUENTAS_CAMBIO_CARRIL){
  this->IN1 = IN1;
  this->IN2 = IN2;
  this->IN3 = IN3;
  this->IN4 = IN4;
  this->ENC_IZQ_A = ENC_IZQ_A;
  this->ENC_IZQ_B = ENC_IZQ_B;
  this->ENC_DER_A = ENC_DER_A;
  this->ENC_DER_B = ENC_DER_B;
  this->CUENTAS_GIRO_90 = CUENTAS_GIRO_90;
  this->CUENTAS_CAMBIO_CARRIL = CUENTAS_CAMBIO_CARRIL;
  MotorHandler::instance = this;

}

int MotorHandler::getIN1Pin(){
  return this->IN1;
}
int MotorHandler::getIN2Pin(){
  return this->IN2;
}
int MotorHandler::getIN3Pin(){
  return this->IN3;
}
int MotorHandler::getIN4Pin(){
  return this->IN4;
}
int MotorHandler::getENC_IZQ_APin(){
  return this->ENC_IZQ_A;
}
int MotorHandler::getENC_IZQ_BPin(){
  return this->ENC_IZQ_B;
}
int MotorHandler::getENC_DER_APin(){
  return this->ENC_DER_A;
}
int MotorHandler::getENC_DER_BPin(){
  return this->ENC_DER_B;
}

void MotorHandler::encoderDerechoISR(){
  if(MotorHandler::instance != nullptr)
    MotorHandler::instance->encoderDerechoHandle();
}

void MotorHandler::encoderIzquierdoISR(){
  if(MotorHandler::instance != nullptr)
    MotorHandler::instance->encoderIzquierdoHandle();
}

void IRAM_ATTR MotorHandler::encoderIzquierdoHandle() {
  this->cuentasIzq++;
}

void IRAM_ATTR MotorHandler::encoderDerechoHandle() {
  this->cuentasDer++;
}

void MotorHandler::resetEncoders() {
  noInterrupts();
  this->cuentasIzq = 0;
  this->cuentasDer = 0;
  interrupts();
}

long MotorHandler::leerEncoderIzquierdo() {
  noInterrupts();
  long valor = this->cuentasIzq;
  interrupts();
  return valor;
}

long MotorHandler::leerEncoderDerecho() {
  noInterrupts();
  long valor = this->cuentasDer;
  interrupts();
  return valor;
}

void MotorHandler::detener() {
  digitalWrite(this->IN1, LOW);
  digitalWrite(this->IN2, LOW);

  digitalWrite(this->IN3, LOW);
  digitalWrite(this->IN4, LOW);
}

void MotorHandler::avanzar() {

  // Izquierdo adelante
  digitalWrite(this->IN1, LOW);
  digitalWrite(this->IN2, HIGH);

  // Derecho adelante
  digitalWrite(this->IN3, LOW);
  digitalWrite(this->IN4, HIGH);
}

void MotorHandler::girarDerecha() {

  // Izquierdo adelante
  digitalWrite(this->IN1, LOW);
  digitalWrite(this->IN2, HIGH);

  // Derecho atrás
  digitalWrite(this->IN3, HIGH);
  digitalWrite(this->IN4, LOW);
}

void MotorHandler::girarIzquierda() {

  // Izquierdo atrás
  digitalWrite(this->IN1, HIGH);
  digitalWrite(this->IN2, LOW);

  // Derecho adelante
  digitalWrite(this->IN3, LOW);
  digitalWrite(this->IN4, HIGH);
}

void MotorHandler::avanzarCuentas(long objetivo) {
  Logger::info("Avanzando cambio de carril...\n");
  resetEncoders();
  avanzar();
  while (true) {

    long izq = leerEncoderIzquierdo();
    long der = leerEncoderDerecho();
    sprintf(MotorHandler::messageBuffer, "IZQ: %ld, DER: %ld\n", izq, der);
    Logger::info(MotorHandler::messageBuffer);
    if (izq >= objetivo && der >= objetivo) {
      break;
    }

    delay(5);
  }
  detener();
  Logger::info("Desplazamiento terminado.\n");
  delay(500);
}

void MotorHandler::giro90Derecha() {

  Logger::info("Girando 90 grados DERECHA\n");

  resetEncoders();

  girarDerecha();

  while (true) {

    long izq = leerEncoderIzquierdo();
    long der = leerEncoderDerecho();
    sprintf(MotorHandler::messageBuffer, "GIRO D -> IZQ: %ld, DER: %ld\n", izq, der);
    Logger::info(MotorHandler::messageBuffer);

    if (izq >= this->CUENTAS_GIRO_90 && der >= this->CUENTAS_GIRO_90) {
      break;
    }
    delay(5);
  }

  detener();

  Logger::info("Giro derecha terminado.\n");
  delay(500);
}

void MotorHandler::giro90Izquierda() {

  Logger::info("Girando 90 grados IZQUIERDA\n");
  resetEncoders();
  girarIzquierda();

  while (true) {

    long izq = leerEncoderIzquierdo();
    long der = leerEncoderDerecho();
    sprintf(MotorHandler::messageBuffer, "GIRO I -> IZQ: %ld, DER: %ld\n", izq, der);
    Logger::info(MotorHandler::messageBuffer);
    if (izq >= this->CUENTAS_GIRO_90 && der >= this->CUENTAS_GIRO_90) {
      break;
    }

    delay(5);
  }

  detener();

  Logger::info("Giro izquierda terminado.\n");

  delay(500);
}

void MotorHandler::cambioCarrilDerecha() {
  Logger::info("\n============================\n");
  Logger::info("CAMBIO DE CARRIL DERECHA\n");
  Logger::info("============================\n");

  giro90Derecha();

  avanzarCuentas(this->CUENTAS_CAMBIO_CARRIL);

  giro90Derecha();

  Logger::info("Cambio derecha completado.\n");
}


void MotorHandler::cambioCarrilIzquierda() {

  Logger::info("\n============================\n");
  Logger::info("CAMBIO DE CARRIL IZQUIERDA\n");
  Logger::info("============================\n");

  giro90Izquierda();

  avanzarCuentas(this->CUENTAS_CAMBIO_CARRIL);

  giro90Izquierda();

  Logger::info("Cambio izquierda completado.\n");
}