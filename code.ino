int ledCount = 9;
int selected;
int i;
int buttonPrev = 12;
int buttonNext = 11;
int buttonRandom = 10;
int noButton = 0;
int clickDelay = 300;
int randomizeStepCount = 5;
int randomDelay = 200;
int randomCountdown = 0;

void setup() {
  randomSeed(analogRead(0));

  initButtons();
  initLeds();
  moveRandom();
}

void loop() {
  if(isRandomizing()){
    continueRandomize();
  }
  int pressedButton = getPressedButton();
  if(pressedButton != noButton){
    handleButtonPress(pressedButton);
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

int getPressedButton(){
  if(isNextPressed())
    return buttonNext;
  if(isPrevPressed())
    return buttonPrev;
  if(isRandomPressed())
    return buttonRandom;
  return noButton;
}

void handleButtonPress(int pressedButton){
  if (pressedButton == buttonNext)
    moveNext();
  else if (pressedButton == buttonPrev)
    movePrev();
  else if (pressedButton == buttonRandom)
    startRandomize();
}

bool isNextPressed(){
  return isButtonPressed(buttonNext);
}

bool isPrevPressed(){
  return isButtonPressed(buttonPrev);
}

bool isRandomPressed(){
  return isButtonPressed(buttonRandom);
}

bool isButtonPressed(int button){
  return digitalRead(button) == HIGH;
}

void initButtons(){
  pinMode(buttonNext, INPUT);
  pinMode(buttonPrev, INPUT);
  pinMode(buttonRandom, INPUT);
}

void initLeds(){
  for (i = 1; i <= ledCount; i++)
    pinMode(i, OUTPUT);
}

int getRandom(){
  int rnd = random(1, ledCount);
  return rnd != selected ? rnd : getRandom();
}

void moveNext(){
  int newValue = selected + 1;
  if(newValue > ledCount)
    newValue = ledCount;
  move(newValue);
  moveDelay();
}

void movePrev(){
  int newValue = selected - 1;
  if(newValue < 1)
    newValue = 1;
  move(newValue);
  moveDelay();
}

void moveRandom(){
  move(getRandom());
  randomizeDelay();
}

void move(int newValue){
  int oldValue = selected;
  digitalWrite(oldValue, LOW);
  digitalWrite(newValue, HIGH);
  selected = newValue;
}

void moveDelay(){
  delay(clickDelay);
}

void randomizeDelay(){
  delay(randomDelay);
}
