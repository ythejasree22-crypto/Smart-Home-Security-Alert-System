#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "esp_camera.h"

// ================= WIFI =================
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ================ TELEGRAM ==============
#define BOT_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
#define CHAT_ID   "YOUR_TELEGRAM_CHAT_ID"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// ============== SECURITY PINS ===========
#define PIR_PIN     13
#define BUZZER_PIN  12
#define LED_PIN      4

// ============ ESP32-CAM AI THINKER =======
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

bool motionDetected = false;

void startCamera()
{
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

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

  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound())
  {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  }
  else
  {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK)
  {
    Serial.print("Camera initialization failed: ");
    Serial.println(err);
    return;
  }

  Serial.println("Camera initialized successfully");
}

void sendPhoto()
{
  camera_fb_t *fb = esp_camera_fb_get();

  if (!fb)
  {
    Serial.println("Camera capture failed");
    return;
  }

  Serial.println("Photo captured");

  bot.sendPhotoByBinary(
    CHAT_ID,
    "image/jpeg",
    fb->len,
    [fb](uint8_t *buffer, size_t index) -> size_t
    {
      size_t remaining = fb->len - index;
      size_t chunk = remaining > 1024 ? 1024 : remaining;

      memcpy(buffer, fb->buf + index, chunk);

      return chunk;
    },
    nullptr,
    "Security Alert: Motion detected!"
  );

  esp_camera_fb_return(fb);

  Serial.println("Photo sent to Telegram");
}

void setup()
{
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // Start camera
  startCamera();

  // Connect Wi-Fi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Allow HTTPS connection
  client.setInsecure();

  bot.sendMessage(
    CHAT_ID,
    "Smart Home Security System is now online.",
    ""
  );

  Serial.println("Security system ready");
}

void loop()
{
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH && !motionDetected)
  {
    motionDetected = true;

    Serial.println("Motion detected!");

    // Turn ON alarm
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);

    // Send photo
    sendPhoto();

    delay(3000);

    // Turn OFF alarm
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  // Reset after motion stops
  if (motion == LOW)
  {
    motionDetected = false;
  }

  delay(200);
}
