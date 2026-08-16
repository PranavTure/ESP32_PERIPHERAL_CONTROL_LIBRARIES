#include <GreeMQTT.h>

GreeMQTT mqtt;


// ========================================
// ACK received callback
// ========================================

void onAckReceived(const String &message)
{
    Serial.print("Application received ACK: ");
    Serial.println(message);

    // Your ACK processing goes here

    if (message == "ON_ACK")
    {
        Serial.println("Device is ON");
    }

    if (message == "OFF_ACK")
    {
        Serial.println("Device is OFF");
    }
}


// ========================================
// Setup
// ========================================

void setup()
{
    Serial.begin(115200);

    mqtt.onAck(onAckReceived);

    mqtt.begin();
}


// ========================================
// Loop
// ========================================

void loop()
{
    mqtt.loop();

    // Example:
    // mqtt.publish("HELLO");

    delay(10);
}
