#ifndef WIFI_ATTACKS_H
#define WIFI_ATTACKS_H

#include <Arduino.h>
#include <vector>
#include <stdint.h>

// WiFi Network Structure
struct WiFiNetwork {
    uint8_t bssid[6];
    char ssid[33];
    uint8_t channel;
    int8_t rssi;
};

// Function declarations for WiFi attacks

/**
 * Initialize WiFi in attack mode
 */
bool initWiFi_Attack();

/**
 * Scan for available WiFi networks
 * @return Number of networks found
 */
int wifi_scan_networks();

/**
 * Send deauthentication frames to disconnect devices
 * @param target_bssid Target network BSSID (MAC address)
 * @param device_mac Device MAC address to disconnect
 * @param channel WiFi channel
 * @param num_packets Number of deauth frames to send
 * @return true if attack successful
 */
bool wifi_deauth_attack(uint8_t* target_bssid, uint8_t* device_mac, uint8_t channel, int num_packets);

/**
 * Beacon spam attack - flood network with fake SSIDs
 * @param ssid_list Array of SSID strings to spam
 * @param num_ssids Number of SSIDs in list
 * @param channel WiFi channel to broadcast on
 * @param duration_ms Duration of attack in milliseconds
 * @return true if attack successful
 */
bool wifi_beacon_spam_attack(const char** ssid_list, int num_ssids, uint8_t channel, int duration_ms);

/**
 * Stop active WiFi attack
 */
void wifi_attack_stop();

/**
 * Get current attack status
 * @return true if attack is active
 */
bool wifi_get_attack_status();

/**
 * Get number of packets sent in current attack
 * @return Packet count
 */
int wifi_get_packets_sent();

/**
 * Get list of scanned networks
 * @return Vector of WiFiNetwork structures
 */
std::vector<WiFiNetwork>& wifi_get_networks();

#endif // WIFI_ATTACKS_H