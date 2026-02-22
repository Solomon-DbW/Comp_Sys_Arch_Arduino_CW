int ledPin[] = {4, 5, 6};
int playerOneButton = 2;
int whiteLED = 9;

int delayTime = 500; 
volatile int score = 0;
volatile int currentActiveLED = -1;
volatile unsigned long lastButtonPress = 0; // For debouncing

void setup() {
  pinMode(playerOneButton, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(playerOneButton), playerOneInput, FALLING); 
  
  for (int i = 0; i < 3; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(whiteLED, OUTPUT);
  Serial.begin(9600);
  Serial.println("Whack-a-Mole Started! Reach 10 points.");
}

void loop() {
  if (score >= 10) {
    Serial.println("YOU WIN!");
    for (int i = 0; i < 3; i++) digitalWrite(ledPin[i], HIGH);
    digitalWrite(whiteLED, HIGH);
    while(true); 
  }

  int randNumber = random(3); 
  
  currentActiveLED = ledPin[randNumber]; 
  digitalWrite(currentActiveLED, HIGH); 
  delay(delayTime);
  
  digitalWrite(currentActiveLED, LOW);
  currentActiveLED = -1; 
  
  // Keep the white LED on briefly if they scored, then turn off
  delay(100); 
  digitalWrite(whiteLED, LOW);
  delay(delayTime);
}

void playerOneInput() {
  unsigned long currentTime = millis();
  // Debounce: Only allow a hit every 200ms
  if (currentTime - lastButtonPress > 200) {
    if (currentActiveLED != -1) {
      score++; 
      digitalWrite(whiteLED, HIGH);
    }
    lastButtonPress = currentTime;
  }
}