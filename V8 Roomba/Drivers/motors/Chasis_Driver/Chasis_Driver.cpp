#include "Chasis_Driver.h"

Chasis_Driver::Chasis_Driver(motor_t leftBackMotor,
                             motor_t leftFrontMotor,
                             motor_t rightBackMotor,
                             motor_t rightFrontMotor,
                             bool omniDrive = 0
                            )
{
    _chasis.leftBack = Motor_Driver(&leftBackMotor);
    _chasis.leftFront = Motor_Driver(&leftFrontMotor);
    _chasis.rightBack = Motor_Driver(&rightBackMotor);
    _chasis.rightFront = Motor_Driver(&rightBackMotor);

    _chasis.omniWheel = omniDrive; 
}

void Chasis_Driver::chasisDrive(float speedPercent, bool forward = 1)
{
    _chasis.leftBack.driveMotor(speedPercent, forward);
    _chasis.leftFront.driveMotor(speedPercent, forward);
    _chasis.rightBack.driveMotor(speedPercent, forward);
    _chasis.rightFront.driveMotor(speedPercent, forward);
}

void Chasis_Driver::omniDrive(float speedPercent, float direction_angle_deg)
{
    const float ANGLE_TOL = 45;
    const float SPECIFIC_ANGLE_TOL = ANGLE_TOL/2;    
    const float GENERAL_ANGLE_TOL = ANGLE_TOL + SPECIFIC_ANGLE_TOL;

    const float FORWARD = 0;
    const float FORWARD_2 = 360;
    const float BACKWARD = 180;
    const float RIGHT = 90;
    const float LEFT = 270;

    const float FORWARD_RIGHT = 45;
    const float BACKWARD_RIGHT = 135;

    const float FORWARD_LEFT = 315;
    const float BACKWARD_LEFT = 225; 

    // forward
    if (abs(direction_angle_deg - FORWARD) < GENERAL_ANGLE_TOL
        || abs(direction_angle_deg - FORWARD_2) < GENERAL_ANGLE_TOL)
    {
        if (abs(direction_angle_deg - FORWARD_RIGHT) < SPECIFIC_ANGLE_TOL)
        {
            _chasis.rightFront.driveMotor(speedPercent, 1);
            _chasis.leftBack.driveMotor(speedPercent, 1);
        }
        else if (abs(direction_angle_deg - FORWARD_LEFT) < SPECIFIC_ANGLE_TOL)
        {
            _chasis.rightBack.driveMotor(speedPercent, 1);
            _chasis.leftFront.driveMotor(speedPercent, 1);
        }
        else
        {
            this->chasisDrive(speedPercent, 1);
        }
    }
    // backward
    else if (abs(direction_angle_deg - BACKWARD) < GENERAL_ANGLE_TOL)
    {
        if (abs(direction_angle_deg - BACKWARD_LEFT) < SPECIFIC_ANGLE_TOL)
        {
            _chasis.rightFront.driveMotor(speedPercent, 0);
            _chasis.leftBack.driveMotor(speedPercent, 0);
        }
        else if (abs(direction_angle_deg - BACKWARD_RIGHT) < SPECIFIC_ANGLE_TOL)
        {
            _chasis.rightBack.driveMotor(speedPercent, 0);
            _chasis.leftFront.driveMotor(speedPercent, 0);
        }
        else
        {
            this->chasisDrive(speedPercent, 0);
        }
    }
    // right
    else if (abs(direction_angle_deg - RIGHT) < SPECIFIC_ANGLE_TOL)
    {
        _chasis.rightBack.driveMotor(speedPercent, 0);
        _chasis.leftFront.driveMotor(speedPercent, 0);

        _chasis.rightFront.driveMotor(speedPercent, 1);
        _chasis.leftBack.driveMotor(speedPercent, 1);
    }
    else
    {
        _chasis.rightBack.driveMotor(speedPercent, 1);
        _chasis.leftFront.driveMotor(speedPercent, 1);

        _chasis.rightFront.driveMotor(speedPercent, 0);
        _chasis.leftBack.driveMotor(speedPercent, 0);
    }
}

void Chasis_Driver::chasisSpin(bool clockwise = 1)
{
    const float SPEED_PERCENT = 25;

    // left side
    _chasis.leftBack.driveMotor(clockwise, SPEED_PERCENT);
    _chasis.leftFront.driveMotor(clockwise, SPEED_PERCENT);

    // right side
    _chasis.rightBack.driveMotor(!clockwise, SPEED_PERCENT);
    _chasis.rightFront.driveMotor(!clockwise, SPEED_PERCENT);
}

Chasis_Driver::~Chasis_Driver()
{
    delete &_chasis.leftBack;
    delete &_chasis.leftFront;
    delete &_chasis.rightBack;
    delete &_chasis.rightFront;
}