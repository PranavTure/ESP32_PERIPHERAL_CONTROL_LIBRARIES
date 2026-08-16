// ===============================
// Wi-Fi Configuration
// ===============================
#define GREE_WIFI_SSID       "YOUR_WIFI_NAME"
#define GREE_WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

// ===============================
// MQTT Broker Configuration
// ===============================
#define GREE_MQTT_BROKER     "192.168.1.100"
#define GREE_MQTT_PORT       1883

// ===============================
// MQTT Topics
// ===============================
#define GREE_MQTT_PUB_TOPIC  "/gree"
#define GREE_MQTT_SUB_TOPIC  "/ack"

// ===============================
// MQTT Client Configuration
// ===============================
#define GREE_MQTT_CLIENT_ID  "ESP32_GREE"

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "GreeMQTTConfig.h"

class GreeMQTT
{
public:

    typedef void (*MessageCallback)(const String &message);

    GreeMQTT();

    // Initialize Wi-Fi and MQTT
    void begin();

    // Must be called repeatedly from loop()
    void loop();

    // Publish message to /gree
    bool publish(const String &message);

    // Check MQTT connection
    bool connected();

    // Register callback for /ack
    void onAck(MessageCallback callback);

private:

    WiFiClient _wifiClient;
    PubSubClient _mqttClient;

    MessageCallback _ackCallback;

    void connectWiFi();
    void connectMQTT();

    static GreeMQTT *_instance;

    static void mqttCallback(
        char *topic,
        byte *payload,
        unsigned int length
    );
};

#endif
