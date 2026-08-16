#ifndef GREE_CONTROLLER_H
#define GREE_CONTROLLER_H

#include <Arduino.h>
#include <Bluepad32.h>

class GreeController
{
public:

    GreeController(
        int motorPwmPin,
        int motorDirPin,
        int pwmChannel = 0,
        int pwmFrequency = 20000,
        int pwmResolution = 8
    );

    void begin();
    void update();

    float getSpeed();
    float getInputForce();
    float getMaxLimit();

private:

    // =================================================
    // Controller
    // =================================================

    ControllerPtr _controllers[BP32_MAX_GAMEPADS];

    static GreeController *_instance;

    static void onConnectedController(ControllerPtr ctl);
    static void onDisconnectedController(ControllerPtr ctl);

    void handleConnectedController(ControllerPtr ctl);
    void handleDisconnectedController(ControllerPtr ctl);

    void processControllers();
    void processGamepad(ControllerPtr ctl);

    // =================================================
    // Motor
    // =================================================

    int _motorPwmPin;
    int _motorDirPin;

    int _pwmChannel;
    int _pwmFrequency;
    int _pwmResolution;

    // =================================================
    // Speed
    // =================================================

    float _currentSpeed;
    float _inputForce;
    float _maxLimit;

    // =================================================
    // Speed modes
    // =================================================

    static constexpr float MODE_30 = 77.0f;
    static constexpr float MODE_60 = 153.0f;
    static constexpr float MODE_90 = 230.0f;

    int _mode;

    unsigned long _lastModeChange;

    // =================================================
    // Internal functions
    // =================================================

    void setSpeed();

    void setMode(int mode);
    void updateControllerLED(ControllerPtr ctl);

    float getThrottle(ControllerPtr ctl);
    float getBrake(ControllerPtr ctl);
};

#endif
