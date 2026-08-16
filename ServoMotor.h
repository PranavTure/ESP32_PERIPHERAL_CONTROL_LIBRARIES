#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>

#define SERVO_PIN 18
#define SERVO_FRQ 50
#define SERVO_RES 16
#define SERVO_CH  6

#define JOY_MIN  -500
#define JOY_MAX   500
#define MIN_DUTY  3276//0 = 1638
#define MAX_DUTY  6553//180 = 8192

// =================================
//          Initialize Servo
// =================================
void servoInit() {
    ledcSetup(SERVO_CH, SERVO_FRQ, SERVO_RES);
    ledcAttachPin(SERVO_PIN, SERVO_CH);
}

// =================================
//          Update Steering
// =================================
void servoUpdate(int dir) {

    if (dir >= 500) dir = 500;
    else if (dir <= -500) dir = -500;

    int duty = map(dir, -500, 500, MIN_DUTY, MAX_DUTY);
    ledcWrite(SERVO_CH, duty);
}

#endif
