#define POT_PIN 1

void taskPot(void *pvParameters) {
  while (1) {
    int val = analogRead(POT_PIN);
    Serial.print("Potentiometer value: ");
    Serial.print(val);
    Serial.print(" | Running on Core: ");
    Serial.println(xPortGetCoreID());
    delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(POT_PIN, INPUT);

  // Core 0
  //xTaskCreatePinnedToCore(taskPot, "taskPot_Core0", 2000, NULL, 1, NULL, 0);

  // Core 1
  xTaskCreatePinnedToCore(taskPot, "taskPot_Core1", 2000, NULL, 1, NULL, 1);
}

void loop() {}