#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h> // Ganti ESP32Servo dengan Servo

#define BTN 9
#define LED 13
#define BUZZER 46
#define CLK 21
#define DT 47
#define SW 48
#define POT_PIN 34  // Sesuaikan dengan pin yang ada di Wokwi
#define SERVO_PIN 14
#define IN1 32  // Sesuaikan dengan pin yang ada di Wokwi
#define IN2 33  // Sesuaikan dengan pin yang ada di Wokwi
#define IN3 34  // Sesuaikan dengan pin yang ada di Wokwi
#define IN4 35  // Sesuaikan dengan pin yang ada di Wokwi

Adafruit_SSD1306 display(128, 64, &Wire, -1);
Servo servo;

int counter = 0;
int lastCLK = HIGH;

const int seq[4][4] = {
  {1, 0, 1, 0},
  {0, 1, 1, 0},
  {0, 1, 0, 1},
  {1, 0, 0, 1}
};

// Task for Button press
void taskButton(void *pvParameters) {
  while (1) {
    if (digitalRead(BTN) == LOW) {
      digitalWrite(LED, HIGH); // Turn on LED when button is pressed
      Serial.print("Button Pressed - Core: ");
    } else {
      digitalWrite(LED, LOW); // Turn off LED when button is released
      Serial.print("Button Released - Core: ");
    }
    Serial.println(xPortGetCoreID());
    delay(200);
  }
}

// Task for Buzzer
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

// Task for Encoder (rotation count)
void taskEncoder(void *pvParameters) {
  while (1) {
    int newCLK = digitalRead(CLK);
    if (newCLK != lastCLK) {
      if (digitalRead(DT) != newCLK) counter++;
      else counter--;
      Serial.print("Encoder counter: ");
      Serial.print(counter);
      Serial.print(" | Running on Core: ");
      Serial.println(xPortGetCoreID());
    }
    lastCLK = newCLK;
    delay(10);
  }
}

// Task for LED blinking
void taskLED(void *pvParameters) {
  while (1) {
    digitalWrite(LED, !digitalRead(LED)); // Toggle LED state
    Serial.print("Task LED running on Core: ");
    Serial.println(xPortGetCoreID());
    delay(500); // 500ms delay
  }
}

// Task for OLED display
void taskOLED(void *pvParameters) {
  while (1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println("ESP32 OLED OK!");
    display.display();
    Serial.print("OLED Task running on Core: ");
    Serial.println(xPortGetCoreID());
    delay(1000); // 1 second delay for display refresh
  }
}

// Task for reading potentiometer value
void taskPot(void *pvParameters) {
  while (1) {
    int val = analogRead(POT_PIN);
    Serial.print("Potentiometer value: ");
    Serial.print(val);
    Serial.print(" | Running on Core: ");
    Serial.println(xPortGetCoreID());
    delay(500); // Read potentiometer value every 500ms
  }
}

// Task for controlling servo motor
void taskServo(void *pvParameters) {
  while (1) {
    for (int pos = 0; pos <= 180; pos += 10) {
      servo.write(pos);
      Serial.print("Servo angle: ");
      Serial.print(pos);
      Serial.print(" | Core: ");
      Serial.println(xPortGetCoreID());
      delay(300); // 300ms delay to slow servo movement
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

// Function for controlling stepper motor
void stepForward(int steps) {
  for (int s = 0; s < steps; s++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(IN1, seq[i][0]);
      digitalWrite(IN2, seq[i][1]);
      digitalWrite(IN3, seq[i][2]);
      digitalWrite(IN4, seq[i][3]);
      delay(3); // Short delay to step the motor
    }
  }
}

// Task for controlling stepper motor
void taskStepper(void *pvParameters) {
  while (1) {
    Serial.print("Stepper Task running on Core: ");
    Serial.println(xPortGetCoreID());
    stepForward(200); // Step forward for 200 steps
    delay(500); // Delay between steps
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BTN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  servo.attach(SERVO_PIN);
  Wire.begin(5, 4);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Core 0
  xTaskCreatePinnedToCore(taskButton, "taskButton_Core0", 2000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskBuzzer, "taskBuzzer_Core0", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskEncoder, "taskEncoder_Core0", 2000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskLED, "taskLED_Core0", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskOLED, "taskOLED_Core0", 4000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskPot, "taskPot_Core0", 2000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskServo, "taskServo_Core0", 2000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskStepper, "taskStepper_Core0", 4000, NULL, 1, NULL, 0);

  // Core 1
  // Additional tasks for Core 1 can be added here if needed
}

void loop() {
  // The main loop is empty because all tasks are handled by RTOS tasks
}
