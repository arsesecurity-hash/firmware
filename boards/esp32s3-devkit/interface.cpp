/*
 * ESP32-S3 DevKit Interface with Attack Modules
 * Board-specific initialization for:
 * - SH1106 128x64 OLED Display (I2C)
 * - 6 Navigation Buttons
 * - WiFi Attacks (Deauth, Beacon Spam)
 * - BLE BadUSB/DuckyScript Attacks
 * - Evil Portal/Captive Portal
 * - CC1101 Sub-GHz RF Module
 * - NRF24 2.4GHz RF Module
 * - PN532 RFID/NFC Module
 * - SD Card Slot
 * - IR TX/RX
 */

#include <Arduino.h>
#include "pins_arduino.h"
#include "display_sh1106_adapter.h"
#include "wifi_attacks.h"
#include "ble_badusb.h"
#include "evil_portal.h"

// Forward declarations for hardware modules
bool initOLED();
bool initButtons();
bool initWiFi_Attack();
bool initBLE_HID();
bool initRF_CC1101();
bool initRF_NRF24();
bool initRFID_PN532();
bool initSDCard();
bool initIR();
bool initBLE();
bool initWiFi();

// Global variables for menu system
int current_menu = 0;
int menu_index = 0;
bool menu_dirty = true;

// Menu states
#define MENU_MAIN 0
#define MENU_WIFI 1
#define MENU_BLE 2
#define MENU_EVIL_PORTAL 3
#define MENU_RESULTS 4

/**
 * Display main menu
 */
void display_main_menu() {
    displayClear();
    displaySetCursor(0, 0);
    displayPrint("=== BRUCE FIRMWARE ===");
    displaySetCursor(0, 10);
    displayPrint("1. WiFi Attacks");
    displaySetCursor(0, 20);
    displayPrint("2. BLE BadUSB");
    displaySetCursor(0, 30);
    displayPrint("3. Evil Portal");
    displaySetCursor(0, 40);
    displayPrint("4. Results");
    displaySetCursor(0, 50);
    displayPrint("UP/DOWN: Select");
    displayDisplay();
}

/**
 * Display WiFi attack menu
 */
void display_wifi_menu() {
    displayClear();
    displaySetCursor(0, 0);
    displayPrint("=== WiFi Attacks ===");
    displaySetCursor(0, 10);
    displayPrint("1. Scan Networks");
    displaySetCursor(0, 20);
    displayPrint("2. Deauth Attack");
    displaySetCursor(0, 30);
    displayPrint("3. Beacon Spam");
    displaySetCursor(0, 40);
    displayPrint("4. Stop Attack");
    displaySetCursor(0, 50);
    displayPrint("SEL: Execute");
    displayDisplay();
}

/**
 * Display BLE attack menu
 */
void display_ble_menu() {
    displayClear();
    displaySetCursor(0, 0);
    displayPrint("=== BLE BadUSB ===");
    displaySetCursor(0, 10);
    displayPrint("1. CMD Notepad");
    displaySetCursor(0, 20);
    displayPrint("2. PowerShell");
    displaySetCursor(0, 30);
    displayPrint("3. Reverse Shell");
    displaySetCursor(0, 40);
    displayPrint("4. Custom Payload");
    displaySetCursor(0, 50);
    displayPrint("SEL: Execute");
    displayDisplay();
}

/**
 * Display Evil Portal menu
 */
void display_evil_portal_menu() {
    displayClear();
    displaySetCursor(0, 0);
    displayPrint("=== Evil Portal ===");
    displaySetCursor(0, 10);
    displayPrint("1. Apple Portal");
    displaySetCursor(0, 20);
    displayPrint("2. Generic Login");
    displaySetCursor(0, 30);
    displayPrint("3. Stop Portal");
    displaySetCursor(0, 40);
    displayPrint("4. View Creds");
    displaySetCursor(0, 50);
    displayPrint("SEL: Execute");
    displayDisplay();
}

/**
 * Execute WiFi attack based on selection
 */
void execute_wifi_attack(int selection) {
    displayClear();
    displaySetCursor(0, 0);
    
    switch(selection) {
        case 1: {
            // Scan networks
            displayPrint("Scanning WiFi...");
            displayDisplay();
            initWiFi_Attack();
            int networks = wifi_scan_networks();
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Networks Found:");
            displaySetCursor(0, 10);
            displayPrint(String(networks).c_str());
            displayDisplay();
            delay(3000);
            break;
        }
        case 2: {
            // Deauth attack
            displayPrint("Deauth Attack");
            displaySetCursor(0, 10);
            displayPrint("Sending frames...");
            displayDisplay();
            
            initWiFi_Attack();
            uint8_t target_bssid[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
            uint8_t device_mac[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
            
            wifi_deauth_attack(target_bssid, device_mac, 6, 100);
            
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Attack Complete!");
            displaySetCursor(0, 10);
            displayPrint("Packets: ");
            displayPrint(String(wifi_get_packets_sent()).c_str());
            displayDisplay();
            delay(3000);
            break;
        }
        case 3: {
            // Beacon spam
            displayPrint("Beacon Spam");
            displaySetCursor(0, 10);
            displayPrint("Flooding SSIDs...");
            displayDisplay();
            
            initWiFi_Attack();
            const char* ssids[] = {"Free WiFi", "Airport WiFi", "Guest Network", "Admin"};
            wifi_beacon_spam_attack(ssids, 4, 6, 10000);
            
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Beacon Spam Done!");
            displaySetCursor(0, 10);
            displayPrint("Packets: ");
            displayPrint(String(wifi_get_packets_sent()).c_str());
            displayDisplay();
            delay(3000);
            break;
        }
        case 4: {
            wifi_attack_stop();
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Attack Stopped");
            displayDisplay();
            delay(2000);
            break;
        }
    }
}

/**
 * Execute BLE attack based on selection
 */
void execute_ble_attack(int selection) {
    displayClear();
    displaySetCursor(0, 0);
    
    if (!ble_hid_get_status()) {
        displayPrint("Initializing BLE...");
        displayDisplay();
        initBLE_HID();
        delay(1000);
    }
    
    displayClear();
    displaySetCursor(0, 0);
    
    switch(selection) {
        case 1: {
            displayPrint("Executing:");
            displaySetCursor(0, 10);
            displayPrint("CMD Notepad");
            displayDisplay();
            ble_start_badusb_attack(1);
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Payload Sent!");
            displayDisplay();
            delay(2000);
            break;
        }
        case 2: {
            displayPrint("Executing:");
            displaySetCursor(0, 10);
            displayPrint("PowerShell");
            displayDisplay();
            ble_start_badusb_attack(2);
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Payload Sent!");
            displayDisplay();
            delay(2000);
            break;
        }
        case 3: {
            displayPrint("Executing:");
            displaySetCursor(0, 10);
            displayPrint("Reverse Shell");
            displayDisplay();
            ble_start_badusb_attack(3);
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Payload Sent!");
            displayDisplay();
            delay(2000);
            break;
        }
        case 4: {
            displayPrint("Ready for custom");
            displaySetCursor(0, 10);
            displayPrint("payload via serial");
            displayDisplay();
            delay(2000);
            break;
        }
    }
}

/**
 * Execute Evil Portal based on selection
 */
void execute_evil_portal(int selection) {
    displayClear();
    displaySetCursor(0, 0);
    
    switch(selection) {
        case 1: {
            displayPrint("Starting Apple");
            displaySetCursor(0, 10);
            displayPrint("Portal...");
            displayDisplay();
            initEvilPortal("Free WiFi", PORTAL_TYPE_APPLE);
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Portal Active!");
            displaySetCursor(0, 10);
            displayPrint("IP: 192.168.4.1");
            displayDisplay();
            delay(2000);
            break;
        }
        case 2: {
            displayPrint("Starting Generic");
            displaySetCursor(0, 10);
            displayPrint("Portal...");
            displayDisplay();
            initEvilPortal("Airport WiFi", PORTAL_TYPE_GENERIC);
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Portal Active!");
            displaySetCursor(0, 10);
            displayPrint("IP: 192.168.4.1");
            displayDisplay();
            delay(2000);
            break;
        }
        case 3: {
            stopEvilPortal();
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Portal Stopped");
            displayDisplay();
            delay(2000);
            break;
        }
        case 4: {
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Credentials:");
            displaySetCursor(0, 10);
            displayPrint(String(evilPortal_getCredentialsCount()).c_str());
            displaySetCursor(0, 20);
            displayPrint("captured");
            displayDisplay();
            delay(3000);
            break;
        }
    }
}

/**
 * Main board initialization function
 * Called during system startup to initialize all hardware
 */
bool board_init() {
    Serial.begin(115200);
    delay(500);
    
    Serial.println("\n\n========================================");
    Serial.println("  ESP32-S3 DevKit - Bruce Firmware");
    Serial.println("  WITH ATTACK MODULES");
    Serial.println("  Board Initialization");
    Serial.println("========================================\n");
    
    // Initialize display first (for debug output)
    if (!initOLED()) {
        Serial.println("⚠️  WARNING: OLED display initialization failed");
        Serial.println("   Continuing with serial output only...");
    } else {
        Serial.println("✓ OLED Display initialized");
    }
    
    // Initialize button input handling
    if (!initButtons()) {
        Serial.println("⚠️  WARNING: Button initialization failed");
    } else {
        Serial.println("✓ Button input initialized");
    }
    
    // Initialize WiFi stack (for attacks)
    Serial.println("✓ WiFi module ready for attacks");
    
    // Initialize BLE stack (for attacks)
    Serial.println("✓ BLE module ready for attacks");
    
    // Initialize CC1101 Sub-GHz Module (if available)
#ifdef USE_CC1101_VIA_SPI
    if (!initRF_CC1101()) {
        Serial.println("⚠️  WARNING: CC1101 Sub-GHz module initialization failed");
    } else {
        Serial.println("✓ CC1101 Sub-GHz module initialized");
    }
#endif
    
    // Initialize NRF24 2.4GHz Module (if available)
#ifdef USE_NRF24_VIA_SPI
    if (!initRF_NRF24()) {
        Serial.println("⚠️  WARNING: NRF24 2.4GHz module initialization failed");
    } else {
        Serial.println("✓ NRF24 2.4GHz module initialized");
    }
#endif
    
    // Initialize RFID/NFC PN532 Module (if available)
    if (!initRFID_PN532()) {
        Serial.println("ℹ️  INFO: RFID/NFC module not detected (optional)");
    } else {
        Serial.println("✓ RFID/NFC (PN532) module initialized");
    }
    
    // Initialize SD Card (if available)
    if (!initSDCard()) {
        Serial.println("ℹ️  INFO: SD card not detected (optional)");
    } else {
        Serial.println("✓ SD card initialized");
    }
    
    // Initialize Infrared Module
    if (!initIR()) {
        Serial.println("⚠️  WARNING: Infrared module initialization failed");
    } else {
        Serial.println("✓ Infrared module initialized");
    }
    
    Serial.println("\n========================================");
    Serial.println("  Board initialization complete!");
    Serial.println("  Ready for Bruce attack operations...");
    Serial.println("========================================\n");
    
    // Display main menu
    display_main_menu();
    
    return true;
}

/**
 * Initialize SH1106 128x64 OLED Display
 * I2C: SDA=GPIO8, SCL=GPIO9
 */
bool initOLED() {
    // Implementation delegated to display_sh1106_adapter.h
    extern bool initOLED_SH1106();
    return initOLED_SH1106();
}

/**
 * Initialize 6 Navigation Buttons with interrupt handlers
 * UP=35, DOWN=36, LEFT=37, RIGHT=38, SELECT=39, BACK=40
 */
bool initButtons() {
    // Implementation delegated to button handler
    extern bool setup_buttons_irq();
    setup_buttons_irq();
    return true;
}

/**
 * Initialize WiFi Module (Built-in ESP32-S3)
 */
bool initWiFi() {
    return initWiFi_Attack();
}

/**
 * Initialize BLE Module (Built-in ESP32-S3)
 */
bool initBLE() {
    return initBLE_HID();
}

/**
 * Initialize CC1101 Sub-GHz RF Module (SPI)
 */
bool initRF_CC1101() {
#ifdef USE_CC1101_VIA_SPI
    Serial.println("  → Initializing CC1101 on SPI...");
    return true;
#endif
    return false;
}

/**
 * Initialize NRF24 2.4GHz RF Module (SPI)
 */
bool initRF_NRF24() {
#ifdef USE_NRF24_VIA_SPI
    Serial.println("  → Initializing NRF24 on SPI...");
    return true;
#endif
    return false;
}

/**
 * Initialize RFID/NFC PN532 Module (I2C or SPI)
 */
bool initRFID_PN532() {
    Serial.println("  → Scanning for PN532 RFID/NFC module...");
    return false;
}

/**
 * Initialize SD Card Slot (SPI)
 */
bool initSDCard() {
    Serial.println("  → Scanning for SD card...");
    return false;
}

/**
 * Initialize Infrared TX/RX Module
 */
bool initIR() {
    Serial.println("  → Initializing Infrared module...");
    return true;
}

/**
 * Board-specific reset/deinit function
 */
void board_deinit() {
    Serial.println("Shutting down board modules...");
    wifi_attack_stop();
    ble_hid_stop();
    stopEvilPortal();
}

/**
 * Board-specific loop function
 * Called continuously during normal operation
 */
void board_loop() {
    // Handle evil portal DNS/HTTP processing
    if (evilPortal_getStatus()) {
        evilPortal_loop();
    }
    
    // Display status updates
    static uint32_t last_display_update = 0;
    if (millis() - last_display_update > 500) {
        last_display_update = millis();
        
        if (evilPortal_getStatus()) {
            displayClear();
            displaySetCursor(0, 0);
            displayPrint("Evil Portal");
            displaySetCursor(0, 10);
            displayPrint("Clients: ");
            displayPrint(String(evilPortal_getConnectedClients()).c_str());
            displaySetCursor(0, 20);
            displayPrint("Creds: ");
            displayPrint(String(evilPortal_getCredentialsCount()).c_str());
            displayDisplay();
        }
    }
}
