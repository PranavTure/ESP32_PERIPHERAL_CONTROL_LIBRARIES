#include <Arduino.h>

// ===== IR Pins =====
#define IR_PIN  34

// ===== State =====
static bool Detected  = false;

// ===== Init =====
void objectDetectInit()
{
    pinMode(IR_PIN, INPUT);
}

// ===== Call this in loop() =====
bool isObjectBlocking()
{
    bool sensorData  = digitalRead(IR_PIN);
    bool verdict = false;

    if (sensorData == LOW)
    {
        verdict = true;
    }
    return verdict;
}


#endif
