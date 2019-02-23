const int ledCount = 9;
const int inputCount = 6;
const int buttonNext = 3;
const int buttonPrev = 4;
const int buttonRandom = 2;
const int remoteButtonNext = 14;
const int remoteButtonPrev = 15;
const int remoteButtonRandom = 16;
const int randomDelay = 200;
const long debounceDelay = 50;
const int firstLed = 0;
int lastLed = ledCount - 1;
int selectedLed = 0;
int randomCountdown = 0;
int randomizeStepCount = 10;
int ledPins[ledCount] = {5, 6, 7, 8, 9, 10, 11, 12, 13};
int inputPins[inputCount] = {buttonNext, buttonPrev, buttonRandom, remoteButtonNext, remoteButtonPrev, remoteButtonRandom};
int inputState[inputCount];
int lastInputState[inputCount];
bool buttonState[inputCount];
long lastDebounceTime[inputCount];

void setup() {
  randomSeed(analogRead(0));

  initInputs();
  initLeds();
  moveRandom();
}

void loop() {
  if(isRandomizing()){
    continueRandomize();
  } else {
    readInputs();
    handleButtonPress();
  }
}

void readInputs(){
  int i;
  for(i = 0; i < inputCount; i++){
    int reading = digitalRead(inputPins[i]);
    if(reading != lastInputState[i]){
      lastDebounceTime[i] = millis();
    }
    if((millis() - lastDebounceTime[i]) > debounceDelay){
      if(reading != inputState[i]){
        inputState[i] = reading;
        if(inputState[i] == HIGH){
          buttonState[i] = true;
        }
      }
    }
    lastInputState[i] = reading;
  }
}

bool isRandomizing(){
  return randomCountdown > 0;
}

void startRandomize(){
  randomCountdown = randomizeStepCount;
  moveRandom();
}

void continueRandomize(){
  randomCountdown -= 1;
  moveRandom();
}

void handleButtonPress(){
  int i;
  for(i = 0; i < inputCount; i++){
    if(buttonState[i]){
      performButtonAction(i);
      buttonState[i] = false;
    }
  }
}

void performButtonAction(int i){
  int button = inputPins[i];
  if (isNextButton(button))
    moveNext();
  else if (isPrevButton(button))
    movePrev();
  else if (isRandomButton(button))
    startRandomize();
}

int isNextButton(int button){
  return button == buttonNext || button == remoteButtonNext;
}

int isPrevButton(int button){
  return button == buttonPrev || button == remoteButtonPrev;
}

int isRandomButton(int button){
  return button == buttonRandom || button == remoteButtonRandom;
}

void initInputs(){
  int i;
  for (i = 0; i < inputCount; i++){
    initInput(i);
  }
}

void initInput(int i){
  int input = inputPins[i];
  pinMode(input, INPUT);
  digitalWrite(input, LOW);
  lastInputState[i] = LOW;
  buttonState[i] = false;
  lastDebounceTime[i] = 0;
}

void initLeds(){
  int i;
  for (i = 0; i < ledCount; i++)
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
}

int getRandom(){
  int rnd = random(firstLed, lastLed);
  return rnd != selectedLed ? rnd : getRandom();
}

void moveNext(){
  int newLed = selectedLed + 1;
  if(newLed > lastLed)
    newLed = firstLed;
  move(newLed);
}

void movePrev(){
  int newLed = selectedLed - 1;
  if(newLed < firstLed)
    newLed = lastLed;
  move(newLed);
}

void moveRandom(){
  move(getRandom());
  randomizeDelay();
}

void move(int newLed){
  int oldLed = selectedLed;
  digitalWrite(ledPins[oldLed], LOW);
  digitalWrite(ledPins[newLed], HIGH);
  selectedLed = newLed;
}

void randomizeDelay(){
  delay(randomDelay);
}
