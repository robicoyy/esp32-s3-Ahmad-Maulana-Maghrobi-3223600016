#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void taskOLED(void *pvParameters) {
  while (1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println("ESP32-S3 OLED OK!");
    display.display();
    Serial.print("OLED Task running on Core: ");
    Serial.println(xPortGetCoreID());
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(5, 4); // SDA, SCL
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Core 0
  //xTaskCreatePinnedToCore(taskOLED, "taskOLED_Core0", 4000, NULL, 1, NULL, 0);

  // Core 1
  xTaskCreatePinnedToCore(taskOLED, "taskOLED_Core1", 4000, NULL, 1, NULL, 1);
}

void loop() {}