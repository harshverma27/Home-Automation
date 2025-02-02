# ESP8266 Relay Control Web Server

This project allows you to control four relays using an ESP8266 module through a web server. The ESP8266 connects to a Wi-Fi network with a **static IP** and hosts a simple web server with endpoints to turn relays **ON** and **OFF**.

## Features
- Connects to a Wi-Fi network.
- Uses a **static IP address** (192.168.137.138).
- Provides a web server to control relays via HTTP requests.
- Relays can be turned ON or OFF using simple **URL commands**.

## Hardware Requirements
- ESP8266 (e.g., NodeMCU)
- Arduino Mega 2560 (optional, if controlling relays separately)
- Relay Module (4-channel)
- Jumper wires
- Power supply (5V for relays, 3.3V for ESP8266)

## How It Works
1. **Wi-Fi Connection**  
   The ESP8266 connects to the specified Wi-Fi network with **SSID** and **password**. It also assigns a **static IP** so that the mobile app can always communicate with it at the same address.

2. **Web Server Initialization**  
   The ESP8266 starts an HTTP server on port **80** and defines multiple endpoints to control the relays.

3. **Relay Control via HTTP**  
   Users can send HTTP requests from a browser or a mobile app to control the relays.

## Code Explanation
### 1. Wi-Fi Configuration
```cpp
const char* ssid = "Samsung A15 5g";  
const char* password = "ABCD1234567890"; 
