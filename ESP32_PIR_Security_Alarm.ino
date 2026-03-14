#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define PIR_PIN 13
#define BUZZER_PIN 14
#define LED_PIN 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool motionDetected = false;
unsigned long lastMotionTime = 0;
#define ALERT_DURATION 3000  // alert stays on for 5 seconds

void IRAM_ATTR onMotion() {
  motionDetected = true;
  lastMotionTime = millis();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Attach interrupt on PIR pin
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), onMotion, RISING);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  // Boot screen
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(25, 20);
  display.println("Security Alarm");
  display.setCursor(30, 35);
  display.println("Initializing...");
  display.display();
  delay(3000);  // give PIR time to calibrate
}

void loop() {
  unsigned long now = millis();

  // Check if alert duration has passed
  if (motionDetected && (now - lastMotionTime > ALERT_DURATION)) {
    motionDetected = false;
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (motionDetected) {
    // Flash LED
    digitalWrite(LED_PIN, (now / 300) % 2);

    // Beep buzzer
    tone(BUZZER_PIN, 1000);

    // OLED alert
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.println("! SECURITY ALARM !");
    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    display.setTextSize(2);
    display.setCursor(5, 18);
    display.println("MOTION");
    display.setCursor(5, 36);
    display.println("DETECTED");

    display.setTextSize(1);
    display.setCursor(20, 55);
    display.println("Checking area...");

  } else {
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);

    // OLED clear
    display.setTextSize(1);
    display.setCursor(30, 0);
    display.println("SECURITY ALARM");
    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    display.setTextSize(2);
    display.setCursor(20, 25);
    display.println("ALL");
    display.setCursor(20, 43);
    display.println("CLEAR");

    display.setTextSize(1);
    display.setCursor(25, 57);
    display.println("No motion!");
  }

  display.display();
  delay(100);
}