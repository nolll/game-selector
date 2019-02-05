int ledCount = 9;
int selected;
int i;
int buttonPrev = 12;
int buttonNext = 11;
int buttonRandom = 10;
int clickDelay = 300;

void setup() {
  initButtons();
  initLeds();
  
  randomSeed(analogRead(0));
  move(getRandom());
}

void loop() {
  if (isNextPressed())
    move(selected + 1);
  else if (isPrevPressed())
    move(selected - 1);
  else if (isRandomPressed())
    move(getRandom());
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

bool isNextPressed(){
  return digitalRead(buttonNext) == HIGH;
}

bool isPrevPressed(){
  return digitalRead(buttonPrev) == HIGH;
}

bool isRandomPressed(){
  return digitalRead(buttonRandom) == HIGH;
}

int getRandom(){
  return random(1, ledCount + 1);
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
