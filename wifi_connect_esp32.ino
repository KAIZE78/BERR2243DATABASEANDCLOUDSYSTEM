#include <WiFi.h>
#include <WiFiClientSecure.h> 
#include <PubSubClient.h>
#include <ArduinoJson.h> 

// WiFi Config
const char* ssid = "OPPO A98 5G";
const char* password = "abcd1234";

// MQTT Config
const char* mqtt_server = "z661651a.ala.asia-southeast1.emqxsl.com"; 
const int mqtt_port = 8883;                  
const char* device_id = "ESP32C3_Device_01";  
const char* cmd_topic = "device/esp32_c3_01/cmd";

// EMQX Credentials
const char* mqtt_username = "TEH"; 
const char* mqtt_password = "56258365421&Abc"; 

// Ai-Thinker ESP-C3-13 Onboard RGB Pins
const int RED_PIN = 3;
const int GREEN_PIN = 4;
const int BLUE_PIN = 5;

// Variables for Blinking Cycle Mode
bool blinkMode = false;
unsigned long lastBlinkTime = 0;
int blinkInterval = 1000; 
int colorStage = 0;
bool singleColorToggle = false; 

// Custom Selection Storage (UPGRADED to 12 to accommodate new colors)
String activeColors[12];   
int totalActiveColors = 0; 

WiFiClientSecure espClient; 
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

// Clears the LED using analog values
void turnOffLED() {
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
}

// Sets precise color mixing using analog values (0 to 255)
void setNamedColor(String color) {
  turnOffLED();
  if (color == "RED")          { analogWrite(RED_PIN, 255); }
  else if (color == "GREEN")   { analogWrite(GREEN_PIN, 255); }
  else if (color == "BLUE")    { analogWrite(BLUE_PIN, 255); }
  else if (color == "YELLOW")  { analogWrite(RED_PIN, 255); analogWrite(GREEN_PIN, 255); }
  else if (color == "CYAN")    { analogWrite(GREEN_PIN, 255); analogWrite(BLUE_PIN, 255); }
  else if (color == "MAGENTA") { analogWrite(RED_PIN, 255); analogWrite(BLUE_PIN, 255); }
  else if (color == "WHITE")   { analogWrite(RED_PIN, 255); analogWrite(GREEN_PIN, 255); analogWrite(BLUE_PIN, 255); }
  else if (color == "ORANGE")  { analogWrite(RED_PIN, 255); analogWrite(GREEN_PIN, 60); } 
  
  // 🌟 NEW COLOR MIXES ADDED HERE:
  else if (color == "PINK")    { analogWrite(RED_PIN, 255); analogWrite(GREEN_PIN, 40);  analogWrite(BLUE_PIN, 130); }
  else if (color == "PURPLE")  { analogWrite(RED_PIN, 140); analogWrite(GREEN_PIN, 0);   analogWrite(BLUE_PIN, 255); }
  else if (color == "GREY")    { analogWrite(RED_PIN, 35);  analogWrite(GREEN_PIN, 35);  analogWrite(BLUE_PIN, 35);  } // Dim white
  else if (color == "GOLD")    { analogWrite(RED_PIN, 255); analogWrite(GREEN_PIN, 150); analogWrite(BLUE_PIN, 15);  }
}

void resetToDefaultRotation() {
  blinkMode = true;
  blinkInterval = 1000; 
  totalActiveColors = 12; // Updated from 8 to 12
  activeColors[0] = "RED";    activeColors[1] = "GREEN";   activeColors[2] = "BLUE";
  activeColors[3] = "YELLOW"; activeColors[4] = "CYAN";    activeColors[5] = "MAGENTA"; 
  activeColors[6] = "WHITE";  activeColors[7] = "ORANGE";
  
  // Added to default loop rotation
  activeColors[8] = "PINK";   activeColors[9] = "PURPLE";  activeColors[10] = "GREY";   
  activeColors[11] = "GOLD";

  colorStage = 0;
  singleColorToggle = true;
  lastBlinkTime = millis();
  setNamedColor(activeColors[colorStage]);
}

void parseCustomColors(String rawColors) {
  totalActiveColors = 0;
  colorStage = 0;
  singleColorToggle = true;
  
  int commaIndex;
  // Updated limit checks from 8 to 12
  while ((commaIndex = rawColors.indexOf(',')) >= 0 && totalActiveColors < 12) {
    activeColors[totalActiveColors] = rawColors.substring(0, commaIndex);
    activeColors[totalActiveColors].trim(); 
    totalActiveColors++;
    rawColors = rawColors.substring(commaIndex + 1);
  }
  if (rawColors.length() > 0 && totalActiveColors < 12) {
    rawColors.trim();
    activeColors[totalActiveColors] = rawColors;
    totalActiveColors++;
  }
  
  lastBlinkTime = millis();
  if(totalActiveColors > 0) {
    setNamedColor(activeColors[colorStage]);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived: ");
  
  JsonDocument doc; 
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return; 
  }

  String cmd = doc["cmd"] | "";
  String speed = doc["speed"] | "";
  String colors = doc["colors"] | "";

  if (cmd == "START") {
    blinkMode = false;
    setNamedColor("WHITE");
  } 
  else if (cmd == "BLINK") {
    if (colors != "") {
      blinkMode = true;
      parseCustomColors(colors);
    } else {
      resetToDefaultRotation();
    }
  } 
  else if (cmd == "STOP") {
    blinkMode = false;
    turnOffLED();
  }
  else if (cmd != "") {
    blinkMode = false;
    turnOffLED();
    setNamedColor(cmd);
  }

  if (speed == "NORMAL") { blinkInterval = 1000; }
  else if (speed == "FAST")   { blinkInterval = 200; }
  else if (speed == "SLOW")   { blinkInterval = 2000; }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection... ");
    boolean connected = strlen(mqtt_username) > 0 ? client.connect(device_id, mqtt_username, mqtt_password) : client.connect(device_id);

    if (connected) { 
      Serial.println("CONNECTED!");
      client.subscribe(cmd_topic);
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  resetToDefaultRotation(); 
  
  setup_wifi();
  espClient.setInsecure(); 
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (blinkMode && totalActiveColors > 0) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentMillis;
      
      if (totalActiveColors == 1) {
        singleColorToggle = !singleColorToggle; 
        if (singleColorToggle) {
          setNamedColor(activeColors[0]); 
        } else {
          turnOffLED(); 
        }
      } 
      else {
        colorStage++;
        if (colorStage >= totalActiveColors) {
          colorStage = 0; 
        }
        setNamedColor(activeColors[colorStage]);
      }
    }
  }
}