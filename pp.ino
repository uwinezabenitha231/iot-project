#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

// ================= WIFI =================
const char* ssid = "besa";
const char* password = "12345678";

// ================= SERVER (PORT 81) =================
WebServer server(81);

// ================= CAMERA PINS (AI THINKER ESP32-CAM-MB) =================
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ================= STREAM =================
void handleStream() {
  WiFiClient client = server.client();

  String response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";

  server.sendContent(response);

  while (client.connected()) {

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      continue;
    }

    client.printf("--frame\r\n");
    client.printf("Content-Type: image/jpeg\r\n\r\n");

    client.write(fb->buf, fb->len);
    client.printf("\r\n");

    esp_camera_fb_return(fb);

    delay(60);
    yield();
  }
}

// ================= CAMERA INIT =================
void initCamera() {

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;

  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init FAILED");
    while (true);
  }

  Serial.println("Camera OK");
}

// ================= WIFI =================
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  initCamera();
  connectWiFi();

  server.on("/stream", handleStream);

  server.begin();   // port 81 already set above

  Serial.println("Server started on port 81");
}

// ================= LOOP =================
void loop() {
  server.handleClient();
  }
