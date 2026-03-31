#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define TRIG D5   
#define ECHO D6   
// ===== WiFi credentials =====
const char* ssid = "besa";
const char* password = "0987654321@";
// ===== Server URL =====
const char* endpointName = "http://192.168.137.1/SARAH/insert.php"; 
void setup() {
  pinMode(TRIG, OUTPUT);
pinMode(ECHO, INPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}
void loop(){
  digitalWrite(TRIG, LOW);
delayMicroseconds(2);
digitalWrite(TRIG, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  long distance = duration * 0.034 / 2;

  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    HTTPClient http;
    // ===== Create JSON using ArduinoJson =====
    StaticJsonDocument<200> doc;
    doc["distance"] = distance; 
    String jsonString;
    serializeJson(doc, jsonString);
    Serial.println("Sending JSON:");
    Serial.println(jsonString);

    // ===== Send HTTP POST =====
    http.begin(client, endpointName);  
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonString);
    Serial.print("HTTP Response: ");
    Serial.println(httpResponseCode);
    http.end();
  }
  delay(10000); // send every 10 seconds
}