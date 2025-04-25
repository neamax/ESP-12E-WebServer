#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Not used
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PWD "YOUR_WIFI_PASSWORD"
#define LOGIN_PWD "loginpassword"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ESP8266WebServer server(80);

const char *ssid = WIFI_SSID;
const char *password = WIFI_PWD;
const char *login_password = LOGIN_PWD;
const int ledPin = D0;
bool ledState = true;

void handleFileRequest(const String &path, const String &contentType)
{
  if (!LittleFS.exists(path))
  {
    server.send(404, "text/plain", "File not found");
    return;
  }
  File file = LittleFS.open(path, "r");
  server.streamFile(file, contentType);
  file.close();
}

void handleLogin()
{
  String inputPassword = server.arg("password");
  if (inputPassword == login_password)
  {
    String jsonResponse = "{\"success\": true}";
    server.send(200, "application/json", jsonResponse);
  }
  else
  {
    String jsonResponse = "{\"success\": false}";
    server.send(200, "application/json", jsonResponse);
  }
}

// boot animation
void fancyBootAnimation()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("NeaESP8266");
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println("by LuckyGene");
  display.display();
  delay(2000);

  // Draw loading bar
  int barWidth = 128;
  int barX = (SCREEN_WIDTH - barWidth) / 2;
  int barY = 28;
  int barHeight = 4;

  display.setCursor(0, 0);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("NeaESP8266\nBooting up...");
  display.drawRect(barX - 1, barY - 1, barWidth + 2, barHeight + 2, SSD1306_WHITE);
  display.display();

  for (int i = 0; i <= barWidth; i += 2)
  {
    display.fillRect(barX, barY, i, barHeight, SSD1306_WHITE);
    display.display();
    delay(50);
  }

  delay(2000);

  // Step-by-step status lines
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("NeaESP8266\n\nConnecting WiFi...");
  display.display();
  delay(1200);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("NeaESP8266\n\nMounting FS...");
  display.display();
  delay(1500);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("NeaESP8266\n\nBooting Web Server...");
  display.display();
  delay(2200);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("NeaESP8266\n\nReady!");
  display.display();
  delay(1000);
}

void displayIdleState()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("NeaESP\n8266");
  display.display();
}

void updateLEDDisplay()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("LED is ");
  display.println(ledState ? "ON" : "OFF");
  display.display();
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // I2C on D2 (SDA), D1 (SCL)
  Wire.begin(D2, D1);

  // init OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("SSD1306 init failed");
    while (true)
      ;
  }
  display.setRotation(2);
  fancyBootAnimation();
  displayIdleState();

  // Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());

  // LittleFS
  if (!LittleFS.begin())
  {
    Serial.println("LittleFS mount failed");
    return;
  }

  // Static file routes
  server.on("/", []()
            { handleFileRequest("/index.html", "text/html"); });
  server.on("/style.css", []()
            { handleFileRequest("/style.css", "text/css"); });
  server.on("/index.js", []()
            { handleFileRequest("/index.js", "application/javascript"); });

  // Login
  server.on("/authLogin", HTTP_GET, handleLogin);

  // LED toggle
  server.on("/toggleLED", []()
            {
    if (server.hasArg("action")) {
      ledState = (server.arg("action") == "on");
      digitalWrite(ledPin, ledState ? LOW : HIGH);
      updateLEDDisplay();
      delay(2000);
      displayIdleState();
    }
    server.send(200, "text/plain", "OK"); });

  server.on("/idleState", []()
            {
    displayIdleState();
    server.send(200, "text/plain", "OK"); });

  // Current LED state
  server.on("/state", []()
            {
    String json = "{\"led\":\"" + String(ledState ? "on" : "off") + "\"}";
    server.send(200, "application/json", json); });

  // Sensor reading
  server.on("/sensor", []()
            {
    float voltage = analogRead(A0) * (3.3 / 1024.0);
    server.send(200, "text/plain", String(voltage, 2)); });

  // wifi signal
  server.on("/wifiSignal", []()
            {
  long rssi = WiFi.RSSI();
  int strength = map(rssi, -100, -50, 0, 4);
  strength = constrain(strength, 0, 4);

  const char* levels[] = {"Very Weak", "Weak", "Okay", "Good", "Strong"};
  String message = "Signal: ";
  message += levels[strength];

  display.clearDisplay();
  for (int i = 0; i <= strength; i++) {
    display.fillRect(10 + i * 10, 20 - i * 4, 8, i * 4 + 1, SSD1306_WHITE);
  }
  display.setCursor(0, 0);
  display.println(levels[strength]);
  display.display();
  server.send(200, "text/plain", message); });

  // Get System Info
  server.on("/sysinfo", []()
            {
    String json = "{";

    json += "\"esp\":{";
    json += "\"chip_id\":\"" + String(ESP.getChipId(), HEX) + "\",";
    json += "\"core_version\":\"" + String(ESP.getCoreVersion()) + "\",";
    json += "\"sdk_version\":\"" + String(ESP.getSdkVersion()) + "\",";
    json += "\"flash_real_size\":" + String(ESP.getFlashChipRealSize()) + ",";
    json += "\"free_heap\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"heap_frag\":" + String(ESP.getHeapFragmentation()) + ",";
    json += "\"max_free_block\":" + String(ESP.getMaxFreeBlockSize()) + ",";
    json += "\"uptime_ms\":" + String(millis());
    json += "},";

    json += "\"wifi\":{";
    json += "\"ssid\":\"" + WiFi.SSID() + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"mac\":\"" + WiFi.macAddress() + "\",";
    json += "\"bssid\":\"" + WiFi.BSSIDstr() + "\",";
    json += "\"channel\":" + String(WiFi.channel()) + ",";
    json += "\"hostname\":\"" + WiFi.hostname() + "\"";
    json += "},";

    json += "\"display\":{";
    json += "\"width\":" + String(display.width()) + ",";
    json += "\"height\":" + String(display.height()) + ",";
    json += "\"rotation\":" + String(display.getRotation()) + ",";
    json += "\"i2c_addr\":\"0x3C\"";
    json += "}";

    json += "}";

    server.send(200, "application/json", json); });

  server.on("/updateLCD", []()
            {
    if (server.hasArg("text")) {
      String txt = server.arg("text");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println(txt);
      display.display();
      server.send(200, "text/plain", "Displayed: " + txt);
    } else {
      server.send(400, "text/plain", "No text provided");
    } });

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}
