#include <BluepadController.h>

GreeController controller;


void setup()
{
    Serial.begin(115200);

    controller.begin();
}


void loop()
{
    controller.update();

    if (!controller.connected())
    {
        delay(10);
        return;
    }

    // ================================
    // Buttons
    // ================================

    if (controller.buttonA())
    {
        Serial.println("A pressed");
    }

    if (controller.buttonB())
    {
        Serial.println("B pressed");
    }

    if (controller.buttonX())
    {
        Serial.println("X pressed");
    }

    if (controller.buttonY())
    {
        Serial.println("Y pressed");
    }


    // ================================
    // Triggers
    // ================================

    float throttle = controller.rightTrigger();
    float brake    = controller.leftTrigger();

    Serial.printf(
        "Throttle: %.2f | Brake: %.2f\n",
        throttle,
        brake
    );


    // ================================
    // Analog sticks
    // ================================

    float lx = controller.leftStickX();
    float ly = controller.leftStickY();

    float rx = controller.rightStickX();
    float ry = controller.rightStickY();

    Serial.printf(
        "LX: %.2f | LY: %.2f | RX: %.2f | RY: %.2f\n",
        lx, ly, rx, ry
    );

    delay(50);
}
