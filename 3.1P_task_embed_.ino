const int buttonPin = 2;   // Button wired to D2
const int trigPin = 8;     // Ultrasonic trigger pin
const int echoPin = 9;     // Ultrasonic echo pin
const int greenLED = 3;    // Green LED at D3
const int redLED = 4;      // Red LED at D4

volatile bool buttonPressed = false;
bool greenState = false;

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
}

void loop() {
  // Handle button press
  if (buttonPressed) {
    buttonPressed = false;
    greenState = !greenState;
    digitalWrite(greenLED, greenState);
    Serial.println("Button pressed: Green LED toggled");
  }

  // Ultrasonic distance check
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000); // 20ms timeout
  int distance = duration * 0.034 / 2;

  // Print only if distance changes by >1 cm
  static int lastDistance = -1;
  if (distance > 0 && abs(distance - lastDistance) > 1) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    lastDistance = distance;
  }

  // Red LED ON/OFF logic
  if (distance > 0 && distance < 20) {
    digitalWrite(redLED, HIGH);  // stays ON if object close
  } else {
    digitalWrite(redLED, LOW);   // stays OFF if object far
  }

  delay(200); // slow down loop (5 updates per second)
}

