#include <ESP32Servo.h>
#define SERVO_PIN 14

Servo servo;

void taskServo(void *pvParameters) {
  while (1) {
    for (int pos = 0; pos <= 180; pos += 10) {
      servo.write(pos);
      Serial.print("Servo angle: ");
      Serial.print(pos);
      Serial.print(" | Core: ");
      Serial.println(xPortGetCoreID());
      delay(300);
    }
    for (int pos = 180; pos >= 0; pos -= 10) {
      servo.write(pos);
      Serial.print("Servo angle: ");
      Serial.print(pos);
      Serial.print(" | Core: ");
      Serial.println(xPortGetCoreID());
      delay(300);
    }
  }
}

void setup() {
  Serial.begin(115200);
  servo.attach(14);

  // Core 0
  //xTaskCreatePinnedToCore(taskServo, "taskServo_Core0", 2000, NULL, 1, NULL, 0);

  // Core 1
  xTaskCreatePinnedToCore(taskServo, "taskServo_Core1", 2000, NULL, 1, NULL, 1);
}

void loop() {}