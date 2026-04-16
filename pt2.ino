#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// WIFI
const char* ssid = "besa";
const char* password = "0987654321@";

// SERVER (port 85)
WebServer server(85);

// SERVOS
Servo panServo;
Servo tiltServo;

int panAngle = 90;
int tiltAngle = 90;

// PINS (ESP32)
#define PAN_PIN  4   // GPIO 4
#define TILT_PIN 5   // GPIO 5

void handlePTZ() {
  String dir = server.arg("dir");

  if (dir == "left")  panAngle -= 10;
  if (dir == "right") panAngle += 10;
  if (dir == "up")    tiltAngle += 10;
  if (dir == "down")  tiltAngle -= 10;
  if (dir == "stop")  {} // do nothing

  panAngle = constrain(panAngle, 0, 180);
  tiltAngle = constrain(tiltAngle, 0, 180);

  panServo.write(panAngle);
  tiltServo.write(tiltAngle);

  String json = "{\"status\":\"ok\",\"dir\":\"" + dir + "\"}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  // Attach servos
  panServo.attach(PAN_PIN);
  tiltServo.attach(TILT_PIN);

  // Start WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Route
  server.on("/ptz", handlePTZ);

  server.begin();
}

void loop() {
  server.handleClient();
}
