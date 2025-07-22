#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Water Level Indicator"
#define BLYNK_AUTH_TOKEN ""

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "Name of the wifi";
char pass[] = "Password";

// Ultrasonic Sensor pins
#define TRIG_PIN D5
#define ECHO_PIN D6

// Define tank dimensions
#define TANK_HEIGHT_CM 20  // Change based on your tank height

BlynkTimer timer;

void sendWaterLevel() {
  long duration;
  float distance;

  // Trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo time
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; // cm

  // Calculate water level
  float waterLevel = TANK_HEIGHT_CM - distance;
  if (waterLevel < 0) waterLevel = 0;
  if (waterLevel > TANK_HEIGHT_CM) waterLevel = TANK_HEIGHT_CM;

  int percentage = (waterLevel / TANK_HEIGHT_CM) * 100;

  // Send to Blynk virtual pin V0
  Blynk.virtualWrite(V0, percentage);

  Serial.print("Water Level: ");
  Serial.print(percentage);
  Serial.println("%");
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Run the function every 2 seconds
  timer.setInterval(2000L, sendWaterLevel);
}

void loop() {
  Blynk.run();
  timer.run();
}
