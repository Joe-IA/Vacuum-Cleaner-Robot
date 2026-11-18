#include "src/Logs/Logger.h"
#include "src/Robot/Robot.hpp"
#include "src/Bluetooth/Bluetooth.hpp"
#include "src/Context/RobotContext.hpp"

#define DEVICE_NAME "RoboClean"
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


// Motor izquierdo
const int IN1 = 19;
const int IN2 = 21;

// Motor derecho
const int IN3 = 23;
const int IN4 = 22;

const int ENC_IZQ_A = 4;
const int ENC_IZQ_B = 16;

const int ENC_DER_A = 17;
const int ENC_DER_B = 5;

const int bumper = 0;
const int bobina = 2;
const int powerButton = 18;

const long CUENTAS_GIRO_90 = 436;
const long CUENTAS_CAMBIO_CARRIL = 555;

bool siguienteGiroIzquierda = false;
RobotContext* robotContext = new RobotContext();
Robot* robot = new Robot(
  robotContext,
  new BluetoothClient(
    robotContext,
    DEVICE_NAME,
    SERVICE_UUID,
    CHARACTERISTIC_UUID
  ),
  new MotorHandler(
    IN1,
    IN2,
    IN3,
    IN4,
    ENC_IZQ_A,
    ENC_IZQ_B,
    ENC_DER_A,
    ENC_DER_B,
    CUENTAS_GIRO_90,
    CUENTAS_CAMBIO_CARRIL
  ),
  bumper,
  powerButton,
  bobina
);

void setup() {

  Logger::startLogger(115200);

  delay(1500);


  Logger::info("============================\n");
  Logger::info("INICIANDO ROBOT\n");
  Logger::info("============================\n");
  
  MotorHandler* motorHandler = robot->getMotorHandler();
  pinMode(motorHandler->getIN1Pin(), OUTPUT);
  pinMode(motorHandler->getIN2Pin(), OUTPUT);
  pinMode(motorHandler->getIN3Pin(), OUTPUT);
  pinMode(motorHandler->getIN4Pin(), OUTPUT);

  motorHandler->detener();


  pinMode(motorHandler->getENC_IZQ_APin(), INPUT_PULLUP);
  pinMode(motorHandler->getENC_IZQ_BPin(), INPUT_PULLUP);

  pinMode(motorHandler->getENC_DER_APin(), INPUT_PULLUP);
  pinMode(motorHandler->getENC_DER_BPin(), INPUT_PULLUP);

  pinMode(robot->getVacuumPin(), OUTPUT);
  pinMode(robot->getBumperButton(), INPUT_PULLUP);
  pinMode(robot->getonOffButton(), INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(motorHandler->getENC_IZQ_APin()),
    motorHandler->encoderIzquierdoISR,
    RISING);

  attachInterrupt(
    digitalPinToInterrupt(motorHandler->getENC_DER_APin()),
    motorHandler->encoderDerechoISR,
    RISING);
  
  attachInterrupt(
    digitalPinToInterrupt(robot->getonOffButton()),
    robot->onOffButtonHandleISR,
    FALLING
  );

  Logger::info("Encoders OK\n");
  Logger::info("L298N configurado\n");

  Logger::info("\nRobot comenzara en 3 segundos...\n");
  delay(3000);
  Logger::info("INICIANDO MOVIMIENTO\n");
}


void loop() {
  MotorHandler* motorHandler = robot->getMotorHandler();
  robot->setVacuumState();
  if(robot->getContext()->getState() == RobotState::ON){
      while(digitalRead(robot->getBumperButton()) == HIGH){
        motorHandler->avanzar();
      }
      robot->handleButton(robot->getBumperButton());
      motorHandler->detener();
      Logger::info("\n***** PARED DETECTADA *****\n");
      if (siguienteGiroIzquierda == false) {
        motorHandler->cambioCarrilDerecha();
        siguienteGiroIzquierda = true;
      }

      else {
        motorHandler->cambioCarrilIzquierda();
        siguienteGiroIzquierda = false;
      }
      Logger::info("Continuando siguiente pasada...\n");
      delay(500);
  }
  else{
    motorHandler->detener();
  }
}