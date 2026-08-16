#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiProv.h>

class ESP32BLEWiFiProvisioner {
public:

    // Callback type for Wi-Fi/provisioning events
    using EventCallback = void (*)(arduino_event_t *event);

    // Provisioning configuration
    struct Config {

        // Proof of Possession
        const char *pop = "abcd1234";

        // BLE provisioning service name
        const char *serviceName = "PROV_123";

        // Not required for BLE provisioning
        const char *serviceKey = nullptr;

        // Transport name used by QR code
        const char *transport = "ble";

        // Erase existing credentials?
        bool resetProvisioned = false;

        // Provisioning UUID
        uint8_t uuid[16] = {
            0xb4, 0xdf, 0x5a, 0x1c,
            0x3f, 0x6b, 0xf4, 0xbf,
            0xea, 0x4a, 0x82, 0x03,
            0x04, 0x90, 0x1a, 0x02
        };
    };

    // Constructor
    explicit ESP32BLEWiFiProvisioner(uint8_t ledPin = 2);

    // Initialize provisioning
    bool begin(
        const Config &config = Config(),
        EventCallback callback = nullptr,
        bool connectStoredCredentials = true
    );

    // Print provisioning QR code
    void printQR() const;

    // Check Wi-Fi connection
    bool isConnected() const;

    // Get local IP address
    IPAddress localIP() const;

    // Manually control LED
    void setLED(bool state);

    // Enable/disable automatic LED control
    void setAutoLED(bool enabled);

    // Convert event ID to readable name
    static const char *eventName(arduino_event_id_t eventId);

private:

    uint8_t _ledPin;
    bool _autoLED;

    EventCallback _userCallback;

    Config _config;

    // Pointer to the currently active object
    static ESP32BLEWiFiProvisioner *_instance;

    // Static event handler required by ESP32 Wi-Fi API
    static void handleEvent(arduino_event_t *event);

    // Instance-specific event processing
    void processEvent(arduino_event_t *event);
};
