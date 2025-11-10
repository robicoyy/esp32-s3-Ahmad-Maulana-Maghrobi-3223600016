#define BTN 9
#define LED 13

void taskButton(void *pvParameters) {
  while (1) {
    if (digitalRead(BTN) == LOW) {
      digitalWrite(LED, HIGH);
      Serial.print("Button Pressed - Core: ");
    } else {
      digitalWrite(LED, LOW);
      Serial.print("Button Released - Core: ");
    }
    Serial.println(xPortGetCoreID());
    delay(200);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  // Core 0
  //xTaskCreatePinnedToCore(taskButton, "taskButton_Core0", 2000, NULL, 1, NULL, 0);

  // Core 1
  xTaskCreatePinnedToCore(taskButton, "taskButton_Core1", 2000, NULL, 1, NULL, 1);
}

void loop() {}