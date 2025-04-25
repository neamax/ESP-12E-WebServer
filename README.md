# NeaESP8266 Web Server

A simple, fun, and fully functional web server built using ESP8266, LittleFS, and an OLED display (SSD1306) for system monitoring. This project includes a Wi-Fi connected interface to control and monitor the ESP8266 through a web dashboard.

**Developed by**: Neama Kazemi  
**Website**: [LuckyGene Indie Studios](http://luckygene.studios)  
**GitHub**: [neamax](https://github.com/neamax)

## Features
- **Web Dashboard**: Control and monitor the ESP8266 via a web interface.
- **Login System**: Password-protected login page.
- **LED Control**: Toggle onboard LED on/off.
- **Sensor Monitoring**: Display sensor data such as analog readings from pin A0.
- **Wi-Fi Signal Strength**: Display the current Wi-Fi signal strength and show a signal bar.
- **System Info**: View detailed system info like chip ID, core version, free heap memory, etc.
- **OLED Display**: Display boot animation and sensor data on the OLED screen (SSD1306).
- **File System**: Serve static files (HTML, CSS, JavaScript) from the LittleFS file system.
- **Idle State**: Show a default idle screen when not in use.

## Hardware Requirements
- ESP8266 Board (e.g., NodeMCU or Wemos D1 mini)
- OLED Display (SSD1306, 128x32 pixels)
- Breadboard and Jumper Wires
- LED connected to D0 (or any other digital pin)
- Wi-Fi Network for the ESP8266 to connect

## Software Requirements
- Arduino IDE (or PlatformIO for VSCode)
- ESP8266 Board Package installed in Arduino IDE

### Libraries:
- Adafruit_SSD1306
- Adafruit_GFX
- ESP8266WiFi
- ESP8266WebServer
- LittleFS
- Wire
- ArduinoJson

## Installation

1. Clone this repository:
    ```bash
    git clone https://github.com/neamax/NeaESP8266.git
    cd NeaESP8266
    ```

2. Open the project in Arduino IDE (or use PlatformIO in VSCode).

3. Install required libraries:
    You can install the required libraries via the Arduino Library Manager:
    - Adafruit SSD1306
    - Adafruit GFX
    - ESP8266WiFi
    - ESP8266WebServer
    - LittleFS
    - ArduinoJson

4. Upload the code:
    - Open `main.cpp` and upload the sketch to your ESP8266 board.
    - Ensure your ESP8266 board is selected under Tools > Board.
    - Connect your ESP8266 to your computer and click Upload.

## Configuration

### Wi-Fi Settings
Edit the following lines in `main.cpp` to configure your Wi-Fi credentials:

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PWD "YOUR_WIFI_PASSWORD"
```

### Login Password
Set the login password for accessing the web interface:

```cpp
#define LOGIN_PWD "loginpassword"
```

### OLED Display
Ensure the OLED display is wired correctly:
- SDA to D2 (GPIO4)
- SCL to D1 (GPIO5)

## Usage

Once the ESP8266 is powered up and connected to your Wi-Fi network, open a browser and navigate to the IP address displayed in the Serial Monitor (e.g., `192.168.1.100`).

You will be prompted to log in with the password you set in the code.

### Dashboard Features:
- **Display Module**:  
  - Enter text to display on the OLED.
  - Toggle the idle state or show specific messages.
  
- **System Control**:  
  - View the sensor voltage from pin A0.
  - Turn the onboard LED on or off.
  - Check Wi-Fi signal strength.

- **Modules Statistics**:  
  - Get detailed system information, such as:
    - Chip ID
    - Core Version
    - Free Heap Memory
    - Uptime
    - Wi-Fi status

- **Sensor Reading**:  
  - Read the sensor voltage from the ESP8266's A0 pin.

- **Wi-Fi Signal**:  
  - View the Wi-Fi signal strength with a graphical display on the OLED.

## Code Explanation

### Boot Animation
The `fancyBootAnimation()` function shows a cool boot-up animation on the OLED display, which includes a loading bar and status updates.

### Web Server
The web server is created using the `ESP8266WebServer` class. It listens on port 80 and handles different routes like:
- `/toggleLED`: Toggle the onboard LED.
- `/sensor`: Get the analog voltage from A0.
- `/wifiSignal`: Display Wi-Fi signal strength.
- `/sysinfo`: Get system information like chip ID, memory usage, etc.
- `/updateLCD`: Display custom text on the OLED screen.

### Login Mechanism
The `handleLogin()` function verifies the password entered on the web page. If the password matches the one defined in the code, the user is granted access to the control panel.

## Known Issues
- **Limited storage**: The ESP8266 has limited memory and storage, so the static files (HTML, CSS, JS) are served from LittleFS instead of the more typical SPIFFS.
- **Limited resolution**: The SSD1306 display has a small resolution (128x32), so the dashboard interface may not be fully optimized for this screen size.

## Future Improvements
- **Mobile-responsive UI**: Improve the web interface to support smaller screens and mobile devices.
- **Additional sensors**: Integrate other sensors like temperature, humidity, etc.
- **WebSocket support**: Use WebSockets for real-time updates.

## License
This project is not under any specific license. Feel free to use, modify, and distribute it as you like. No warranty is provided.

## Acknowledgements
- Adafruit SSD1306 Library
- ESP8266WebServer
- LittleFS
