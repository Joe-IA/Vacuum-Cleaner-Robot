#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>
class MotorHandler{
    private:
        volatile long cuentasIzq = 0;
        volatile long cuentasDer = 0;

        // Motor izquierdo
        int ENA;
        int IN1;
        int IN2;

        // Motor derecho
        int ENB;
        int IN3;
        int IN4;

        // Encoder izquierdo
        int ENC_IZQ_A;
        int ENC_IZQ_B;

        //Encoder derecho
        int ENC_DER_A;
        int ENC_DER_B;

        int CUENTAS_GIRO_90;
        int CUENTAS_CAMBIO_CARRIL;

        static MotorHandler* instance;

    public:
        MotorHandler(
            int IN1,
            int IN2,
            int IN3,
            int IN4,
            int ENC_IZQ_A,
            int ENC_IZQ_B,
            int ENC_DER_A,
            int ENC_DER_B,
            int CUENTAS_GIRO_90,
            int CUENTAS_CAMBIO_CARRIL
        );

        int getIN1Pin();
        int getIN2Pin();
        int getIN3Pin();
        int getIN4Pin();
        int getENC_IZQ_APin();
        int getENC_IZQ_BPin();
        int getENC_DER_APin();
        int getENC_DER_BPin();
        static void encoderIzquierdoISR();
        static void encoderDerechoISR();
        void IRAM_ATTR encoderIzquierdoHandle();
        void IRAM_ATTR encoderDerechoHandle();
        void resetEncoders();
        long leerEncoderIzquierdo();
        long leerEncoderDerecho();
        void detener();
        void avanzar();
        void girarDerecha();
        void girarIzquierda();
        void avanzarCuentas(long objetivo);
        void giro90Derecha();
        void giro90Izquierda();
        void cambioCarrilDerecha();
        void cambioCarrilIzquierda();
        static char messageBuffer[100];
};
#endif
