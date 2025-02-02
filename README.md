# **Smart Home Automation System**

## **Introduction**

The **Smart Home Automation System** is designed to improve the **safety, security, and convenience** of a home. This project integrates **fire detection, wireless appliance control, gesture-based interactions, and RFID-based secure door locking**. It also extends its capabilities with **web-based control using the ESP8266 Wi-Fi module**.  

---

## **Features**

### 🔥 **Fire and Smoke Detection**  
* Detects fire or smoke using flame and smoke sensors.  
* Activates a **buzzer alarm** when danger is detected.  

### 🔌 **Wireless Appliance Control**  
Appliances can be controlled using:  
* **Bluetooth (HC-05):** Mobile app sends commands to Arduino.  
* **ESP8266 Web Server:** Control appliances through a web browser.  

### 🖐 **Gesture-Based Control**  
* **Clap Detection:** Uses a **sound sensor** (microphone) to turn appliances ON/OFF.  

### 🔑 **RFID-Based Secure Locking**  
* **RFID card authentication** allows secure access.  
* Unlocks doors using a **servo motor** and **relay-controlled DC lock**.  
* Unauthorized access triggers a **buzzer alert**.  

---

## **Hardware Components**

### **1️⃣ Processing Units**
| Component | Function |
|-----------|----------|
| **Arduino Mega 2560** | Controls sensors, relays, RFID access, and Bluetooth. |
| **ESP8266 Wi-Fi Module** | Hosts a web server for browser-based appliance control. |

### **2️⃣ Communication Modules**
| Component | Function |
|-----------|----------|
| **Bluetooth Module (HC-05)** | Enables wireless control via a mobile app. |
| **ESP8266 Wi-Fi Module** | Receives HTTP requests for web-based relay control. |

### **3️⃣ Sensors and Security Modules**
| Component | Function |
|-----------|----------|
| **Flame Sensor** | Detects fire using infrared light. |
| **Smoke Sensor** | Detects smoke particles in the air. |
| **Microphone Sensor** | Detects sound-based gestures like claps. |
| **RFID Sensor (MFRC522)** | Reads RFID cards for secure access. |

### **4️⃣ Actuators and Electrical Components**
| Component | Function |
|-----------|----------|
| **Relays** | Toggle appliances ON/OFF. |
| **Servo Motor** | Controls door locking mechanism. |
| **DC Lock Mechanism** | Provides physical security for door access. |
| **Buzzer** | Alerts in case of unauthorized access or danger. |

---

## **ESP8266-Based Web Control Setup**

### **1️⃣ ESP8266 Configuration**
1. **Connect ESP8266 to Wi-Fi**  
   * The ESP8266 connects to the **pre-configured Wi-Fi network** at startup.  
   * The module prints its **IP address** to the Serial Monitor.  
   
2. **Find ESP8266 IP Address**  
   * Open **Arduino Serial Monitor**.  
   * Look for an output like:  
     ```
     Connected to Wi-Fi
     IP Address: 192.168.1.100
     ```

### **2️⃣ Web-Based Appliance Control**
1. **Open a browser** (Chrome, Edge, or Firefox) on a smartphone or PC connected to the **same Wi-Fi network** as the ESP8266.  
2. **Enter ESP8266’s IP Address + Command**  
   * Example:
     * **Turn ON Relay 1:** `http://192.168.1.100/relay1/on`
     * **Turn OFF Relay 1:** `http://192.168.1.100/relay1/off`

### **3️⃣ How It Works**
1. **User sends a request** via a web browser.  
2. **ESP8266 receives the HTTP request** and decodes the command (e.g., `relay1/on`).  
3. **ESP8266 sets the appropriate pin HIGH or LOW** and sends a signal to **Arduino Mega**.  
4. **Arduino Mega controls the relays** to turn appliances ON/OFF.  

---

## **RFID-Based Secure Access**

### **1️⃣ How RFID Access Works**
1. **User taps an RFID card** on the MFRC522 module.  
2. **Arduino reads the card UID** and compares it with stored authorized UIDs.  
3. If **access is granted**:
   * **Servo motor rotates** to unlock the door.  
   * **Relay activates the DC lock** to open.  
4. If **access is denied**:
   * **Buzzer sounds an alert** for a few seconds.  

### **2️⃣ Setting Up RFID Cards**
* Modify the **Arduino code** to store new **authorized UIDs**:  
  ```cpp
  if (cardUID == "0185C626") { // Replace with actual UID
      Serial.println("Access Granted");
  }
  ```
* Each RFID card has a **unique ID**, which can be found by printing the UID to the **Serial Monitor**.

---
