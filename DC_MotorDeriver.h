#ifndef GENERIC_DC_MOTOR_H
#define GENERIC_DC_MOTOR_H

#include <Arduino.h>

class GenericDCMotor
{
public:

    // =================================================
    // Constructor
    // =================================================

    GenericDCMotor(
        uint8_t pwmPin,
        uint8_t dirPin,
        uint8_t pwmChannel = 0,
        uint32_t pwmFrequency = 20000,
        uint8_t pwmResolution = 8
    )
    {
        _pwmPin = pwmPin;
        _dirPin = dirPin;

        _pwmChannel = pwmChannel;
        _pwmFrequency = pwmFrequency;
        _pwmResolution = pwmResolution;

        _speed = 0;
        _direction = 0;
    }


    // =================================================
    // Initialize motor
    // =================================================

    void begin()
    {
        pinMode(
            _dirPin,
            OUTPUT
        );

        digitalWrite(
            _dirPin,
            LOW
        );

        ledcSetup(
            _pwmChannel,
            _pwmFrequency,
            _pwmResolution
        );

        ledcAttachPin(
            _pwmPin,
            _pwmChannel
        );

        ledcWrite(
            _pwmChannel,
            0
        );

        _speed = 0;
        _direction = 0;
    }


    // =================================================
    // Set speed
    //
    // Positive = Forward
    // Negative = Reverse
    // Zero     = Stop
    //
    // Example:
    //
    // motor.setSpeed(200);
    // motor.setSpeed(-200);
    // motor.setSpeed(0);
    // =================================================

    void setSpeed(int speed)
    {
        int maxPWM =
            (1 << _pwmResolution) - 1;

        speed = constrain(
            speed,
            -maxPWM,
            maxPWM
        );

        _speed = speed;

        if (_speed > 0)
        {
            _direction = 1;
        }
        else if (_speed < 0)
        {
            _direction = -1;
        }
        else
        {
            _direction = 0;
        }

        apply();
    }


    // =================================================
    // Forward
    // =================================================

    void forward(uint8_t speed)
    {
        setSpeed(
            abs((int)speed)
        );
    }


    // =================================================
    // Reverse
    // =================================================

    void reverse(uint8_t speed)
    {
        setSpeed(
            -abs((int)speed)
        );
    }


    // =================================================
    // Stop
    // =================================================

    void stop()
    {
        _speed = 0;
        _direction = 0;

        ledcWrite(
            _pwmChannel,
            0
        );
    }


    // =================================================
    // Brake
    //
    // Generic behavior = PWM 0.
    //
    // Actual electrical braking depends on
    // the motor driver.
    // =================================================

    void brake()
    {
        _speed = 0;
        _direction = 0;

        ledcWrite(
            _pwmChannel,
            0
        );
    }


    // =================================================
    // Get current speed
    // =================================================

    int getSpeed()
    {
        return _speed;
    }


    // =================================================
    // Get direction
    //
    //  1 = Forward
    //  0 = Stop
    // -1 = Reverse
    // =================================================

    int getDirection()
    {
        return _direction;
    }


    // =================================================
    // Get PWM value
    // =================================================

    int getPWM()
    {
        return abs(_speed);
    }


private:

    // =================================================
    // Hardware configuration
    // =================================================

    uint8_t _pwmPin;
    uint8_t _dirPin;

    uint8_t _pwmChannel;
    uint32_t _pwmFrequency;
    uint8_t _pwmResolution;


    // =================================================
    // Motor state
    // =================================================

    int _speed;
    int _direction;


    // =================================================
    // Apply speed to motor
    // =================================================

    void apply()
    {
        int pwmValue =
            abs(_speed);


        // -----------------------------
        // Forward
        // -----------------------------

        if (_speed > 0)
        {
            digitalWrite(
                _dirPin,
                LOW
            );
        }


        // -----------------------------
        // Reverse
        // -----------------------------

        else if (_speed < 0)
        {
            digitalWrite(
                _dirPin,
                HIGH
            );
        }


        // -----------------------------
        // Stop
        // -----------------------------

        else
        {
            pwmValue = 0;
        }


        // -----------------------------
        // Apply PWM
        // -----------------------------

        ledcWrite(
            _pwmChannel,
            pwmValue
        );
    }
};

#endif
