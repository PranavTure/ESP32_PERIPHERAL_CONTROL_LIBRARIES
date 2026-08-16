#ifndef GREE_CONTROLLER_H
#define GREE_CONTROLLER_H

#include <Arduino.h>
#include <Bluepad32.h>

class GreeController
{
public:

    GreeController();

    // Initialize Bluepad32
    void begin();

    // Update controller state
    bool update();

    // Controller status
    bool connected();

    // =================================================
    // Buttons
    // =================================================

    bool buttonA();
    bool buttonB();
    bool buttonX();
    bool buttonY();

    bool dpadUp();
    bool dpadDown();
    bool dpadLeft();
    bool dpadRight();

    bool buttonL1();
    bool buttonR1();

    bool buttonL3();
    bool buttonR3();

    bool buttonSelect();
    bool buttonStart();

    // =================================================
    // Triggers
    // Returns 0.0 -> 1.0
    // =================================================

    float leftTrigger();
    float rightTrigger();

    // =================================================
    // Analog sticks
    // Returns -1.0 -> +1.0
    // =================================================

    float leftStickX();
    float leftStickY();

    float rightStickX();
    float rightStickY();

    // =================================================
    // Raw values
    // =================================================

    int rawLeftTrigger();
    int rawRightTrigger();

    int rawLeftStickX();
    int rawLeftStickY();

    int rawRightStickX();
    int rawRightStickY();

    // =================================================
    // Controller access
    // =================================================

    ControllerPtr getController();


private:

    ControllerPtr _controller;

    static GreeController *_instance;

    static void onConnected(
        ControllerPtr ctl
    );

    static void onDisconnected(
        ControllerPtr ctl
    );

    void handleConnected(
        ControllerPtr ctl
    );

    void handleDisconnected(
        ControllerPtr ctl
    );
};

#endif
