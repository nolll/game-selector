const int ledCount = 9;
const int inputCount = 3;
const int buttonNext = 11;
const int buttonPrev = 12;
const int buttonRandom = 10;

const int randomDelay = 200;
const long debounceDelay = 50;

int selectedLed = 0;
int randomCountdown = 0;
int randomizeStepCount = 5;
int inputPins[inputCount] = {buttonNext, buttonPrev, buttonRandom};
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
  if (button == buttonNext)
    moveNext();
  else if (button == buttonPrev)
    movePrev();
  else if (button == buttonRandom)
    startRandomize();
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
  for (i = 1; i <= ledCount; i++)
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
}

int getRandom(){
  int rnd = random(1, ledCount);
  return rnd != selectedLed ? rnd : getRandom();
}

void moveNext(){
  int newLed = selectedLed + 1;
  if(newLed > ledCount)
    newLed = 1;
  move(newLed);
}

void movePrev(){
  int newLed = selectedLed - 1;
  if(newLed < 1)
    newLed = ledCount;
  move(newLed);
}

void moveRandom(){
  move(getRandom());
  randomizeDelay();
}

void move(int newLed){
  int oldLed = selectedLed;
  digitalWrite(oldLed, LOW);
  digitalWrite(newLed, HIGH);
  selectedLed = newLed;
}

void randomizeDelay(){
  delay(randomDelay);
}
