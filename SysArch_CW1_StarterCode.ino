// assign LEDs and button to pins
int ledPin[] = {4,5,6};
int playerOneButton = 2;
int whiteLED = 9;

// declare variables
int delayTime; // time delay between lights on/off
int randNumber;
int whiteLEDOn; 

//setup interrupt, button input and LED outputs
void setup() {
  attachInterrupt(0, playerOneInput, FALLING); // specify interrupt routine
  for (int i=0; i<3; i++){
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(playerOneButton, INPUT);
  pinMode(whiteLED, OUTPUT);
}

//run main program loop
void loop() {
  randNumber = random(3); // select a random number
  digitalWrite(ledPin[randNumber], HIGH); // light the LED with this number
  delay(delayTime);
  digitalWrite(ledPin[randNumber], LOW);
  delay(delayTime);
  whiteLEDOn = digitalRead(whiteLED);
  if(whiteLEDOn=HIGH){
    digitalWrite(whiteLED, LOW);
  }  //if whiteLED on = turn it off
}


void playerOneInput() {
  digitalWrite(whiteLED, HIGH); 
  /*change this code so that white LED only switches on when button is pressed 
  at the right time*/
}

