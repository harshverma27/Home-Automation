## **Smart Home Automation System**

### **Introduction**

The Smart Home Automation System is a versatile project designed to enhance the safety, convenience, and efficiency of a modern home. It incorporates fire detection, wireless and gesture-based appliance control, and secure RFID-based locking, now extended with web-based control using an ESP8266 module.

---

### **Features**

1. **Fire Detection and Emergency Response**  
   * Detects fire or smoke using flame and smoke sensors and triggers an audible alert via a buzzer.  
2. **Wireless Appliance Control**  
   * Devices such as bulbs and fans can be controlled wirelessly using:  
     * **Bluetooth module:** Commands sent from a smartphone app.  
     * **Web Server:** Commands sent via a browser from a smartphone or computer connected to the same Wi-Fi network as the ESP8266.  
3. **Gesture Control**  
   * Appliances can be operated using gestures like clapping or voice commands via a microphone sensor.  
4. **RFID Locking System**  
   * Securely locks and unlocks doors using an RFID card reader.

   ---

### **Components**

#### **1\. Processing Units**

* **Arduino Mega 2560:**  
  * Central controller for sensor data, relay management, RFID access control, and Bluetooth communication.  
  * Processes input from the ESP8266 to control relays.

**2\. Communication Modules**

* **Bluetooth Module (HC-05):**  
  * Provides wireless communication for controlling appliances using a smartphone app.  
* **ESP8266 Wi-Fi Module:**  
  * Hosts a web server to enable browser-based control of relays.  
  * Receives HTTP requests and communicates with the Arduino to toggle appliances ON/OFF.

**3\. Sensing Modules**

* **Flame Sensor:**  
  * Detects fire by sensing infrared light.  
* **Smoke Sensor:**  
  * Monitors air quality for smoke particles.  
* **Microphone Sensor:**  
  * Detects sound-based gestures, such as clapping, to control appliances.  
* **RFID Sensor (MFRC522):**  
  * Reads RFID tags for secure door access.

**4\. Electric Components**

* **Relays:**  
  * Control appliances by toggling them ON/OFF.  
* **DC Lock Mechanism:**  
  * Provides physical locking/unlocking functionality based on RFID authentication.

 **5\. Other Components**

* Jumper Wires  
* Buzzer
  ---

**Steps to Use ESP8266 for Web-Based Control**

#### **1\. ESP8266 Setup**

1. Connect the ESP8266 module to your Wi-Fi network:  
   * On startup, the ESP will connect to the pre-configured Wi-Fi network.  
   * The ESP will display its IP address in the Serial Monitor.  
2. Note the IP address of the ESP (e.g., `192.168.1.100`).

**2\. Controlling Relays Using the Web Server**

1. Open a browser on a smartphone or PC connected to the same Wi-Fi network.  
2. Enter the ESP's IP address in the browser followed by a specific command to control a relay:  
   * Example:  
     * To turn ON Relay 1: `http://192.168.1.100/relay1/on`  
     * To turn OFF Relay 1: `http://192.168.1.100/relay1/off`

**3\. How It Works**

* The ESP8266 receives the HTTP request and identifies the command (e.g., `relay1/on`).  
* The ESP sets the corresponding pin HIGH or LOW and notifies the Arduino Mega through connected pins.  
* The Arduino processes this input and activates or deactivates the corresponding relay.  
  ---
