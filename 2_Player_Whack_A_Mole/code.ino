#include <Servo.h>
#include <pitches.h>

Servo gameServo;
int servoPos = 90;

// -------------------- Pin Setup --------------------
int P1LEDPin[] = {6, 5, 4};
int P1Button = 3;
int P1WhiteLED = 7;

int p1ActiveIndex;

int P2LEDPin[] = {10, 11, 12};
int P2Button = 2;
int P2WhiteLED = 13;

int p2ActiveIndex;

// -------------------- Game Variables --------------------
volatile bool p1Pressed = false;
volatile bool p2Pressed = false;

int P1score = 0;
int P2score = 0;

int activeIndex = -1;
unsigned long moleStartTime = 5000;
unsigned long moleInterval = 100;
unsigned long moleOffTime = 5000;

bool moleActive = false;
bool gameOver = false;

// -------------------- Interrupts --------------------
void p1HitISR() { p1Pressed = true; }
void p2HitISR() { p2Pressed = true; }

// -------------------- Setup --------------------
void setup() {
  Serial.begin(9600);

  gameServo.attach(9);
  gameServo.write(servoPos);

  pinMode(P1Button, INPUT_PULLUP);
  pinMode(P2Button, INPUT_PULLUP);
  pinMode(P1WhiteLED, OUTPUT);
  pinMode(P2WhiteLED, OUTPUT);

  for (int i = 0; i < 3; i++) {
    pinMode(P1LEDPin[i], OUTPUT);
    pinMode(P2LEDPin[i], OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(P1Button), p1HitISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(P2Button), p2HitISR, FALLING);

  randomSeed(analogRead(A0));

  Serial.println("Game Start! First to 10 wins.");
}

// -------------------- Main Loop --------------------
void loop() {

  if (gameOver) return;

  unsigned long now = millis();

  // ----- Spawn mole (MIRRORED LED EFFECT) -----
  if (!moleActive && now >= moleOffTime) {

    p1ActiveIndex = random(0, 3);
    p2ActiveIndex = random(0, 3);

    while (p2ActiveIndex == p1ActiveIndex) {
      p2ActiveIndex = random(0, 3);
    }

    digitalWrite(P1LEDPin[p1ActiveIndex], HIGH);
    digitalWrite(P2LEDPin[p2ActiveIndex], HIGH);

    moleStartTime = now;
    moleActive = true;
  }
  // if (!moleActive && now >= moleOffTime) {

  //   activeIndex = random(0, 3);

  //   // Player 1 LED
  //   digitalWrite(P1LEDPin[activeIndex], HIGH);

  //   // Player 2 mirrored LED
  //   int mirroredIndex = 2 - activeIndex;
  //   digitalWrite(P2LEDPin[mirroredIndex], HIGH);

  //   moleStartTime = now;
  //   moleActive = true;
  // }

  // ----- Turn mole off -----
  if (moleActive && now - moleStartTime >= moleInterval) {
    clearMoles();
    activeIndex = -1;
    moleActive = false;
    // moleOffTime = now + 50;
    moleOffTime = now + random(200, 1200);
  }

  // ----- Handle Player 1 Press -----
  if (p1Pressed) {
    p1Pressed = false;
    handleHit(1);
  }

  // ----- Handle Player 2 Press -----
  if (p2Pressed) {
    p2Pressed = false;
    handleHit(2);
  }

  // ----- Win Check -----
  if (P1score >= 10) endGame(1);
  if (P2score >= 10) endGame(2);
}

// -------------------- Hit Handler --------------------
void handleHit(int player) {

  // if (!moleActive) return;
    if (!moleActive) {
      tone(8, 200, 150);   // low buzz sound
      delay(50);
      noTone(8);
      return;
    }

  if (player == 1) {

    P1score++;
    digitalWrite(P1WhiteLED, HIGH);

    servoPos -= 9;
    servoPos = constrain(servoPos, 0, 180);
    gameServo.write(servoPos);

    tone(8, NOTE_C5, 100);

    Serial.print("P1 Score: ");
    Serial.println(P1score);

  } else {

    P2score++;
    digitalWrite(P2WhiteLED, HIGH);

    servoPos += 9;
    servoPos = constrain(servoPos, 0, 180);
    gameServo.write(servoPos);

    tone(8, NOTE_C4, 100);

    Serial.print("P2 Score: ");
    Serial.println(P2score);
  }

  clearMoles();
  moleActive = false;
  activeIndex = -1;
  moleOffTime = millis() + 200;

  // Increase difficulty
  if (moleInterval > 250) {
    moleInterval -= 30;
  }
}

// -------------------- Clear LEDs --------------------
void clearMoles() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(P1LEDPin[i], LOW);
    digitalWrite(P2LEDPin[i], LOW);
  }
  digitalWrite(P1WhiteLED, LOW);
  digitalWrite(P2WhiteLED, LOW);
}

// -------------------- End Game --------------------
void endGame(int winner) {

  gameOver = true;

  Serial.println(winner == 1 ? "P1 WINS!" : "P2 WINS!");

  while (true) {

    digitalWrite(P1WhiteLED, HIGH);
    digitalWrite(P2WhiteLED, HIGH);

    for (int i = 0; i < 3; i++) {
      digitalWrite(P1LEDPin[i], HIGH);
      digitalWrite(P2LEDPin[i], HIGH);
    }

    delay(500);

    digitalWrite(P1WhiteLED, LOW);
    digitalWrite(P2WhiteLED, LOW);

    for (int i = 0; i < 3; i++) {
      digitalWrite(P1LEDPin[i], LOW);
      digitalWrite(P2LEDPin[i], LOW);
    }

    delay(500);
  }
}
