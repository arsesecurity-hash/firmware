#ifndef WIFI_ATTACKS_H
#define WIFI_ATTACKS_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>

// WiFi Attack Structures
typedef struct {
    uint8_t bssid[6];
    uint8_t ssid[32];
    uint8_t ssid_len;
    uint8_t channel;
    int8_t rssi;
} wifi_ap_info_t;

typedef struct {
    uint8_t dst[6];
    uint8_t src[6];
    uint8_t bssid[6];
    uint16_t seq;
} deauth_frame_t;

// WiFi Attack Functions
bool initWiFiAttacks();
void startDeauthAttack(uint8_t* bssid, uint8_t channel);
void stopDeauthAttack();
void startBeaconSpam(const char** ssids, uint8_t count);
void stopBeaconSpam();
void startEvilPortal(const char* ssid, const char* password = "password");
void stopEvilPortal();
void wifiScanNetworks();
void wifiSniff();

// Internal helpers
void IRAM_ATTR sendDeauthFrame(uint8_t* bssid, uint8_t channel, uint16_t count);
void createFakeBeacon(const char* ssid, uint8_t channel);

#endif // WIFI_ATTACKS_H
