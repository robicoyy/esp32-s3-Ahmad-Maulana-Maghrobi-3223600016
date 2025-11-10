#define IN1 38  // B-
#define IN2 39  // B+
#define IN3 40  // A+
#define IN4 41  // A-

const int seq[4][4] = {
  {1,0,1,0},
  {0,1,1,0},
  {0,1,0,1},
  {1,0,0,1}
};

void stepForward(int steps) {
  for (int s = 0; s < steps; s++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(IN1, seq[i][0]);
      digitalWrite(IN2, seq[i][1]);
      digitalWrite(IN3, seq[i][2]);
      digitalWrite(IN4, seq[i][3]);
      delay(3);
    }
  }
}

void taskStepper(void *pvParameters) {
  while (1) {
    Serial.print("Stepper Task running on Core: ");
    Serial.println(xPortGetCoreID());
    stepForward(200);
    delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Core 0
  //xTaskCreatePinnedToCore(taskStepper, "taskStepper_Core0", 4000, NULL, 1, NULL, 0);

  // Core 1
  xTaskCreatePinnedToCore(taskStepper, "taskStepper_Core1", 4000, NULL, 1, NULL, 1);
}

void loop() {}