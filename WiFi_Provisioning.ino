#include "sdkconfig.h"
#if CONFIG_ESP_WIFI_REMOTE_ENABLED
#error "WiFiProv is only supported in SoCs with native Wi-Fi support"
#endif

#include "WiFiProv.h"
#include "WiFi.h"

#define LED_PIN 2
#define PWM_FREQ 5000
#define PWM_RES 8

const char *pop = "abcd1234";          // Proof of possession (PIN)
const char *service_name = "PROV_123"; // Device name
const char *service_key = NULL;        // Not used for BLE
bool reset_provisioned = false;

// Provisioning event handler (runs in separate FreeRTOS task)
void SysProvEvent(arduino_event_t *sys_event) {
  switch (sys_event->event_id) {

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("\nConnected IP address : ");
      Serial.println(IPAddress(sys_event->event_info.got_ip.ip_info.ip.addr));
      break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("\nDisconnected. Reconnecting...");
      break;

    case ARDUINO_EVENT_PROV_START:
      Serial.println("\nProvisioning started");
      break;

    case ARDUINO_EVENT_PROV_CRED_RECV:
      Serial.println("\nReceived Wi-Fi credentials");
      Serial.print("SSID: ");
      Serial.println((const char *)sys_event->event_info.prov_cred_recv.ssid);
      Serial.print("Password: ");
      Serial.println((const char *)sys_event->event_info.prov_cred_recv.password);
      break;

    case ARDUINO_EVENT_PROV_CRED_FAIL:
      Serial.println("\nProvisioning failed");
      break;

    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
      Serial.println("\nProvisioning successful");
      digitalWrite(LED_PIN, HIGH);
      break;

    case ARDUINO_EVENT_PROV_END:
      Serial.println("\nProvisioning ended");
      break;

    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  WiFi.onEvent(SysProvEvent);
  WiFi.begin();  // Load credentials from NVS

  Serial.println("Starting BLE Provisioning");

  uint8_t uuid[16] = {
    0xb4, 0xdf, 0x5a, 0x1c,
    0x3f, 0x6b, 0xf4, 0xbf,
    0xea, 0x4a, 0x82, 0x03,
    0x04, 0x90, 0x1a, 0x02
  };

  WiFiProv.beginProvision(
    NETWORK_PROV_SCHEME_BLE,
    NETWORK_PROV_SCHEME_HANDLER_FREE_BLE,
    NETWORK_PROV_SECURITY_1,
    pop,
    service_name,
    service_key,
    uuid,
    reset_provisioned
  );

  WiFiProv.printQR(service_name, pop, "ble");
}

void loop() {}
