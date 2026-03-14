# 🚨 ESP32 PIR Security Alarm

Real-time motion detection security alarm using HC-SR501 PIR sensor with LED alert, buzzer and OLED display on ESP32.

## 📸 All Clear

![Image](https://github.com/user-attachments/assets/92db5af9-afaa-4391-a0fb-203cb29e17c6)

## 📸 Motion Detected

![Image](https://github.com/user-attachments/assets/d22c862a-c641-409c-9d4a-42730b15ad47)

## 🎥 Demo

https://github.com/user-attachments/assets/ee1bb671-83b2-451d-a83d-873951e86eba

## 🛠️ Hardware Requirements

- ESP32 Development Board
- HC-SR501 PIR Motion Sensor
- Passive Buzzer Module
- LED (any color)
- 220Ω Resistor
- SSD1306 OLED Display (128x64, I2C)
- Breadboard
- Jumper Wires

## 🔌 Wiring Table

### OLED Display (SSD1306)

| OLED Pin | ESP32 Pin |
|----------|-----------|
| VCC      | 3.3V      |
| GND      | GND       |
| SDA      | GPIO 21   |
| SCL      | GPIO 22   |

### HC-SR501 PIR Sensor

| PIR Pin | ESP32 Pin |
|---------|-----------|
| VCC     | 5V (VIN)  |
| GND     | GND       |
| OUT     | GPIO 13   |

### Passive Buzzer Module

| Buzzer Pin | ESP32 Pin |
|------------|-----------|
| VCC        | 3.3V      |
| GND        | GND       |
| I/O        | GPIO 14   |

### LED

| LED Pin      | ESP32 Pin               |
|--------------|-------------------------|
| Long leg (+) | GPIO 2                  |
| Short leg (-) | GND (via 220Ω resistor) |

> **Tip:** Use the breadboard power rails (red = 3.3V, blue = GND) to share power across all components. PIR must use 5V (VIN) not 3.3V.

## 📦 Libraries Required

Install these via Arduino IDE → Library Manager:

- `Adafruit SSD1306`
- `Adafruit GFX Library`

## 💻 Code

```cpp
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
#define ALERT_DURATION 3000

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

  attachInterrupt(digitalPinToInterrupt(PIR_PIN), onMotion, RISING);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(25, 20);
  display.println("Security Alarm");
  display.setCursor(30, 35);
  display.println("Initializing...");
  display.display();
  delay(3000);
}

void loop() {
  unsigned long now = millis();

  if (motionDetected && (now - lastMotionTime > ALERT_DURATION)) {
    motionDetected = false;
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (motionDetected) {
    digitalWrite(LED_PIN, (now / 300) % 2);
    tone(BUZZER_PIN, 1000);

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
```

## 🚦 Getting Started

1. Clone this repository.
2. Open the `.ino` file in Arduino IDE.
3. Install the required libraries via Library Manager.
4. Select your ESP32 board under **Tools → Board → ESP32 Dev Module**.
5. Connect your hardware following the wiring table above.
6. Upload the code and wait 3 seconds for the PIR sensor to calibrate.
7. Wave your hand in front of the PIR sensor to trigger the alarm.
