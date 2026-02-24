int ledPin[] = {6, 5, 4}; // Red, Blue, Green
int playerOneButton = 2;
int playerTwoButton = 3;
int whiteLED = 9;

int delayTime = 700; // Starting speed
volatile int P1score = 0;
volatile int P2score = 0;
volatile int currentActiveLED = -1;

// Separate debounce timers
volatile unsigned long lastP1Press = 0;
volatile unsigned long lastP2Press = 0;

void setup() {
  pinMode(playerOneButton, INPUT_PULLUP); 
  pinMode(playerTwoButton, INPUT_PULLUP);

  // Use interrupts for high-speed reaction
  attachInterrupt(digitalPinToInterrupt(playerOneButton), playerOneInput, FALLING); 
  attachInterrupt(digitalPinToInterrupt(playerTwoButton), playerTwoInput, FALLING); 
  
  for (int i = 0; i < 3; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(whiteLED, OUTPUT);
  
  Serial.begin(9600);
  randomSeed(analogRead(0)); 
  Serial.println("Whack-a-Mole Started! First to 10 wins.");
}

void loop() {
  // Check for a winner
  if (P1score >= 10) {
    gameOver("PLAYER 1 WINS!");
  } else if (P2score >= 10) {
    gameOver("PLAYER 2 WINS!");
  }

  // Pick a random LED (0, 1, or 2)
  int randIndex = random(0, 3); 
  currentActiveLED = ledPin[randIndex]; 
  
  digitalWrite(currentActiveLED, HIGH); 
  delay(delayTime); // Mole stays up
  
  digitalWrite(currentActiveLED, LOW);
  currentActiveLED = -1; // Mole is hidden
  
  digitalWrite(whiteLED, LOW); // Turn off hit indicator for next round
  delay(delayTime / 2); // Short pause between moles
}

void playerOneInput() {
  unsigned long currentTime = millis();
  if (currentTime - lastP1Press > 250) { // 250ms Debounce
    if (currentActiveLED != -1) {
      P1score++; 
      digitalWrite(whiteLED, HIGH);
      Serial.print("P1 Hit! Score: "); Serial.println(P1score);
    }
    lastP1Press = currentTime;
  }
}

void playerTwoInput() {
  unsigned long currentTime = millis();
  if (currentTime - lastP2Press > 250) { 
    if (currentActiveLED != -1) {
      P2score++; 
      digitalWrite(whiteLED, HIGH);
      Serial.print("P2 Hit! Score: "); Serial.println(P2score);
    }
    lastP2Press = currentTime;
  }
}

void gameOver(String winner) {
  Serial.println("*****************");
  Serial.println(winner);
  Serial.println("*****************");
  // Light up everything
  for (int i = 0; i < 3; i++) digitalWrite(ledPin[i], HIGH);
  digitalWrite(whiteLED, HIGH);
  while(true); // Stop the game
}
