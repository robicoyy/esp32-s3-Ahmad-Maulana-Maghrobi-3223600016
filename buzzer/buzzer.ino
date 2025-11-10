#define BUZZER 46

void taskBuzzer(void *pvParameters) {
  while (1) {
    digitalWrite(BUZZER, HIGH);
    Serial.print("Buzzer task running on Core: ");
    Serial.println(xPortGetCoreID());
    delay(300);
    digitalWrite(BUZZER, LOW);
    delay(300);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);

  // Core 0
  //xTaskCreatePinnedToCore(taskBuzzer, "taskBuzzer_Core0", 1000, NULL, 1, NULL, 0);

  // Core 1
  xTaskCreatePinnedToCore(taskBuzzer, "taskBuzzer_Core1", 1000, NULL, 1, NULL, 1);
}

void loop() {}