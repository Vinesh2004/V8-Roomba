#ifndef MOTORDATATYPES_h
#define MOTORDATATYPES_h

#include "./Motor_Driver/Motor_Driver.h"

typedef struct {
    int forwardPin;
    int backwardPin;
} motor_t;

typedef struct {
    Motor_Driver leftBack;
    Motor_Driver leftFront;

    Motor_Driver rightBack;
    Motor_Driver rightFront;

    bool omniWheel;
    
} chasis_t;

#endif