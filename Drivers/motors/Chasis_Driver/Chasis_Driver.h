#ifndef CHASISDRIVER_h
#define CHASISDRIVER_h

#include "Arduino.h"
#include "../Motor_Data_types.h"
#include "../Motor_Driver/Motor_Driver.h"

class Chasis_Driver
{
    public:
        Chasis_Driver(motor_t leftBackMotor,
                      motor_t leftFrontMotor,
                      motor_t rightBackMotor,
                      motor_t rightFrontMotor,
                      bool omniDrive
                     );
        ~Chasis_Driver();
        
        void chasisDrive( float speedPercent, bool forward);
        void omniDrive(float speedPercent, float direction_angle_deg);

        void chasisSpin(bool clockwise);

    private:
        chasis_t _chasis;
};

#endif