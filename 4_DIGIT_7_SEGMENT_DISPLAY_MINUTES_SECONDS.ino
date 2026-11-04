int segmentPins[] = {2, 3, 4, 5, 6, 7, 8};
int digitPins[] = {12, 11, 10, 9};
int dpPin = 13;
int buzzerPin = 22;
int buttonPin = 23;

const byte digits[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

int minutes = 12;
int seconds = 0;
unsigned long lastUpdate = 0;
bool lastButtonState = HIGH;

void setup() {
  for (int i = 0; i < 7; i++) pinMode(segmentPins[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(digitPins[i], OUTPUT);
  pinMode(dpPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(dpPin, LOW);
}

void displayDigit(int digit, int number) {
  for (int i = 0; i < 4; i++) digitalWrite(digitPins[i], HIGH);
  for (int i = 0; i < 7; i++) digitalWrite(segmentPins[i], LOW);
  digitalWrite(digitPins[digit], LOW);
  for (int i = 0; i < 7; i++) digitalWrite(segmentPins[i], digits[number][i]);
  delay(5);
}

void displayTime(int mm, int ss) {
  int total = mm * 100 + ss;
  bool skipThousands = (total < 1000); // Skip leading zero if less than 1000

  int digitsArray[4] = {
    (total / 1000) % 10,
    (total / 100) % 10,
    (total / 10) % 10,
    total % 10
  };

  for (int repeat = 0; repeat < 5; repeat++) {
    for (int i = 0; i < 4; i++) {
      if (i == 0 && skipThousands) continue; // Don’t show leading zero
      displayDigit(i, digitsArray[i]);
    }
  }
}

void loop() {
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    seconds++;
    if (seconds >= 60) {
      seconds = 0;
      minutes++;
      if (minutes > 12) minutes = 1; // Roll back to 1 after 12
    }
  }

  // Buzzer alert at 8:30
  if (minutes == 8 && seconds == 30) {
    tone(buzzerPin, 1000, 500);
  }

  // Button to add 1 minute
  bool currentButtonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    minutes++;
    if (minutes > 12) minutes = 1;
    delay(200); // debounce
  }
  lastButtonState = currentButtonState;

  displayTime(minutes, seconds);
}
