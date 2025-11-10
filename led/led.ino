#define LED_PIN 13

void taskLED(void *pvParameters) {
  while (1) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    Serial.print("Task LED running on Core: ");
    Serial.println(xPortGetCoreID());
    delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // Core 0
  xTaskCreatePinnedToCore(taskLED, "taskLED_Core0", 1000, NULL, 1, NULL, 0);

  // Core 1
  // xTaskCreatePinnedToCore(taskLED, "taskLED_Core1", 1000, NULL, 1, NULL, 1);
}

void loop() {}