#ifndef BLE_BADUSB_H
#define BLE_BADUSB_H

#include <Arduino.h>

// Function declarations for BLE BadUSB/DuckyScript attacks

/**
 * Initialize BLE HID Device
 * @return true if initialization successful
 */
bool initBLE_HID();

/**
 * Send a single HID keyboard key
 * @param modifier Modifier key (CTRL, SHIFT, ALT, etc.)
 * @param keycode Key scan code
 * @return true if sent successfully
 */
bool ble_send_key(uint8_t modifier, uint8_t keycode);

/**
 * Type a string via BLE HID
 * @param text String to type
 * @return true if successful
 */
bool ble_type_string(const char* text);

/**
 * Load and parse DuckyScript payload
 * DuckyScript format:
 *   DELAY <milliseconds>
 *   STRING <text>
 *   ENTER
 *   TAB
 *   ESC
 *   CTRL <key>
 *   ALT <key>
 * 
 * Example:
 *   DELAY 1000
 *   STRING notepad.exe
 *   ENTER
 *   STRING Hello World
 *
 * @param script DuckyScript payload string
 * @return true if script loaded successfully
 */
bool ble_load_duckyscript(const char* script);

/**
 * Execute currently loaded DuckyScript
 * @return true if execution successful
 */
bool ble_execute_duckyscript();

/**
 * Start BadUSB attack with predefined payload
 * @param payload_type 1=CMD Open, 2=PowerShell, 3=Reverse Shell
 * @return true if attack successful
 */
bool ble_start_badusb_attack(int payload_type);

/**
 * Execute custom payload
 * @param payload Custom DuckyScript payload
 * @return true if successful
 */
bool ble_execute_custom_payload(const char* payload);

/**
 * Get BLE HID status
 * @return true if BLE HID is active
 */
bool ble_hid_get_status();

/**
 * Stop BLE HID
 */
void ble_hid_stop();

// Keyboard modifier keys
#define KEY_MOD_LCTRL   0x01
#define KEY_MOD_LSHIFT  0x02
#define KEY_MOD_LALT    0x04
#define KEY_MOD_LGUI    0x08
#define KEY_MOD_RCTRL   0x10
#define KEY_MOD_RSHIFT  0x20
#define KEY_MOD_RALT    0x40
#define KEY_MOD_RGUI    0x80

// Example payloads
extern const char* example_payload_cmd_open;
extern const char* example_payload_powershell;
extern const char* example_payload_cmd_reverse_shell;

#endif // BLE_BADUSB_H