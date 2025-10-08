const int buttonPin = 3;
const int trigPin = 4;
const int echoPin = 5;
const int greenLED = 9;
const int redLED = 8;

volatile bool buttonPressed = false;
bool greenState = false;  // Green LED controlled by button


bool lastRedState = LOW;
bool lastGreenState = LOW;

void handleButtonInterrupt() {
  buttonPressed = true;
}

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonInterrupt, FALLING);

  // Start with both LEDs OFF
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
}

void loop() {
  // Handle button press
  if (buttonPressed) {
    buttonPressed = false;
    greenState = !greenState;   
  }

  // Ultrasonic check every 100ms
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck >= 100) {
    lastCheck = millis();

    // Trigger ultrasonic pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000); // timeout 30ms
    int distance = duration * 0.034 / 2;

    // Priority logic: ultrasonic overrides button
    if (duration > 0 && distance < 20) {
      // Sensor detects object → RED ON, GREEN OFF
      if (lastRedState != HIGH) {
        digitalWrite(redLED, HIGH);
        lastRedState = HIGH;
        Serial.println("Ultrasonic detected object → Red LED ON, Green LED OFF");
      }
      if (lastGreenState != LOW) {
        digitalWrite(greenLED, LOW);
        lastGreenState = LOW;
      }
    } else {
      // No object → RED OFF, GREEN follows button
      if (lastRedState != LOW) {
        digitalWrite(redLED, LOW);
        lastRedState = LOW;
        Serial.println("No object → Red LED OFF");
      }
      if (lastGreenState != greenState) {
        digitalWrite(greenLED, greenState);
        lastGreenState = greenState;
        Serial.print("Green LED set to: ");
        Serial.println(greenState ? "ON" : "OFF");
      }
    }
  }
}
