int ledCount = 9;
int selected;
int i;
int buttonPrev = 12;
int buttonNext = 11;
int buttonRandom = 10;
int noButton = 0;
int clickDelay = 300;

void setup() {
  initButtons();
  initLeds();
  
  randomSeed(analogRead(0));
  move(getRandom());
}

void loop() {
  int pressedButton = getPressedButton();
  if(pressedButton != noButton){
    handleButtonPress(pressedButton);
  }
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
    move(selected + 1);
  else if (pressedButton == buttonPrev)
    move(selected - 1);
  else if (pressedButton == buttonRandom)
    move(getRandom());
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

void move(int newValue){
  if(newValue < 1)
    newValue = 1;
  if(newValue > ledCount)
    newValue = ledCount;
  if(newValue != selected){
    int oldValue = selected;
    digitalWrite(oldValue, LOW);
    digitalWrite(newValue, HIGH);
    selected = newValue;
    delay(clickDelay);
  }
}
