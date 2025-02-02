# **Smart Home Automation - Web-Based Control Interface**

## **Introduction**
The **Web-Based Control Interface** for the **Smart Home Automation System** allows users to wirelessly control appliances using a simple **HTML, CSS, and JavaScript** interface. This interface communicates with an **ESP8266 Web Server** to toggle **relays ON/OFF**, enabling remote control of home devices.

---

## **Features**
✅ **Relay Control via Web Browser** – Switch appliances ON/OFF using buttons.  
✅ **Real-Time Status Update** – Button colors update based on the relay state.  
✅ **ESP8266 Communication** – Uses HTTP requests to send commands to the ESP8266 module.  
✅ **Responsive UI** – Works on smartphones, tablets, and PCs.  

---

## **Website Components**

### **1️⃣ Frontend (HTML + CSS)**
The website consists of a simple, user-friendly **HTML interface** styled with **CSS**.

- **HTML Structure**:
  - A centered **title**: `"Relay Control"`
  - **4 pairs of buttons** for controlling 4 relays (ON/OFF for each relay).
  - Buttons change color dynamically when pressed.

- **CSS Styling**:
  - Uses **flexbox** for centering the UI.
  - **Red & Blue buttons** indicate ON/OFF states.
  - Smooth hover and transition effects.

### **2️⃣ JavaScript (Dynamic Relay Control)**
- **`toggleRelay(relayNum, state)`**
  - Constructs the HTTP request URL for ESP8266.
  - Sends a `fetch()` request to ESP8266 Web Server.
  - Updates **button colors and text** dynamically.

- **`updateButtonColor(relayNum)`**
  - Changes button color based on relay state.
  - Ensures **ON buttons turn blue** and **OFF buttons turn red**.

---

## **How It Works**
1. **User opens the webpage** on a smartphone, tablet, or PC.  
2. **User clicks a button** to turn a relay ON or OFF.  
3. **JavaScript sends an HTTP request** to ESP8266 via `fetch()`.  
4. **ESP8266 processes the request** and controls the corresponding relay.  
5. **Response is sent back to the webpage**, confirming the action.  
6. **Button color updates** dynamically to reflect the relay's status.  

---

## **ESP8266 HTTP Request Handling**
Each button sends a **specific URL request** to the ESP8266:

| Action | URL Format |
|--------|------------|
| Relay 1 ON | `http://192.162.137.138/relay1/on` |
| Relay 1 OFF | `http://192.162.137.138/relay1/off` |
| Relay 2 ON | `http://192.162.137.138/relay2/on` |
| Relay 2 OFF | `http://192.162.137.138/relay2/off` |
| Relay 3 ON | `http://192.162.137.138/relay3/on` |
| Relay 3 OFF | `http://192.162.137.138/relay3/off` |
| Relay 4 ON | `http://192.162.137.138/relay4/on` |
| Relay 4 OFF | `http://192.162.137.138/relay4/off` |

The ESP8266 **reads the URL parameters** (`relayX/on` or `relayX/off`) and toggles the corresponding relay.

---

## **Steps to Deploy the Web Interface**
### **1️⃣ Upload HTML File to ESP8266**
1. Open **Arduino IDE**.
2. Include the **ESP8266WebServer** library.
3. Store the HTML file as a **string** inside the ESP8266 sketch.
4. Configure ESP8266 to serve the webpage over **Wi-Fi**.

### **2️⃣ Access the Webpage**
1. Find the ESP8266’s **IP address** in the Serial Monitor.
2. Open a browser and enter the IP (e.g., `http://192.162.137.138`).
3. The webpage will load, allowing **relay control**.

---

## **Future Improvements**
✅ **Real-Time Feedback** – Display the actual relay state fetched from ESP8266.  
✅ **Google Assistant Integration** – Use voice commands to trigger relay actions.  
✅ **Custom Mobile App** – Replace the browser interface with an Android app.  

---

## **Conclusion**
The **web-based relay control system** makes home automation more **accessible and efficient**. With **ESP8266 handling HTTP requests**, users can control appliances wirelessly, eliminating the need for physical switches. This system can be expanded for **advanced IoT applications**, making smart home automation even more powerful. 🚀  
