// IRremote - Version: Latest 
#include <IRremote.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

const int ledCount = 9;
const int inputCount = 3;
const int buttonNext = 2;
const int buttonPrev = 4;
const int buttonRandom = 3;
const int randomPin = A4;
const int remoteIR = A5;
const int buzzer = 17;
const int randomDelay = 200;
const int debounceDelay = 50;
const int tonePause = 60;
const int firstLed = 0;
int lastLed = ledCount - 1;
int selectedLed = 0;
int randomCountdown = 0;
int randomizeMinStepCount = 7;
int randomizeMaxStepCount = 15;
int ledPins[ledCount] = {5, 6, 7, 8, 9, 10, 11, 12, 13};
int notes[ledCount] = {4186, 4186, 2093, 1047, 523, 262, 131, 65, 33};
int inputPins[inputCount] = {buttonNext, buttonPrev, buttonRandom};
int buttonNextIndex = 0;
int buttonPrevIndex = 1;
int buttonRandomIndex = 2;
int inputState[inputCount];
int lastInputState[inputCount];
bool buttonState[inputCount];
long lastDebounceTime[inputCount];

// hardware
//#define IR_UP 0x00FF9867
//#define IR_DOWN 0x00FF38C7
//#define IR_SELECT 0x00FF18E7

// simulator
#define IR_UP 0x00FD50AF
#define IR_DOWN 0x00FD10EF
#define IR_SELECT 0x00FD906F

IRrecv irrecv(remoteIR);
decode_results results;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(randomPin));
  initIR();
  initInputs();
  initLeds();
  startSequence();
}

void loop() {
  readInputs();
  handleButtonPress();
}

void startSequence(){
  for(int i = ledCount - 1; i >= 0; i--){
    delay(200);
    moveAndMakeSound(i);
  }
  delay(500);
  moveAndMakeSound(getRandomLed());
}

void readInputs(){
  bool irRead = tryReadIR();
  if(!irRead)
    readButtons();
}

void readButtons(){
  for(int i = 0; i < inputCount; i++){
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

bool tryReadIR(){
  int buttonIndex = -1;
  if (irrecv.decode(&results))
  {
    if (results.value == IR_UP){
      buttonIndex = buttonPrevIndex;
    }
    else if (results.value == IR_DOWN){
      buttonIndex = buttonNextIndex;
    }
    else if (results.value == IR_SELECT){
      buttonIndex = buttonRandomIndex;
    }

    irrecv.resume();
  }

  if(buttonIndex > -1){
    buttonState[buttonIndex] = true;
  }
  
  return false;
}

void makeSound(int i){
  //tone(buzzer, notes[i], tonePause);
}

void stopSound(){
  //noTone(buzzer);
}

void handleButtonPress(){
  for(int i = 0; i < inputCount; i++){
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
    randomize();
}

void randomize(){
  int randomCountdown = getNumberOfRandomSteps();
  while(randomCountdown > 0){
    moveRandom();
    randomCountdown -= 1;
  }
}

int getNumberOfRandomSteps(){
  int countdownSpread = randomizeMaxStepCount - randomizeMinStepCount;
  int rnd = random(0, countdownSpread);
  return randomizeMinStepCount + rnd;
}

int isNextButton(int button){
  return button == buttonNext;
}

int isPrevButton(int button){
  return button == buttonPrev;
}

int isRandomButton(int button){
  return button == buttonRandom;
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

void initIR(){
  irrecv.enableIRIn();
}

int getRandomLed(){
  int randomLed = random(0, ledCount);
  return randomLed == selectedLed ? getRandomLed() : randomLed;
}

void moveNext(){
  int newLed = selectedLed + 1;
  if(newLed > lastLed)
    newLed = firstLed;
  moveAndMakeSound(newLed);
}

void movePrev(){
  int newLed = selectedLed - 1;
  if(newLed < firstLed)
    newLed = lastLed;
  moveAndMakeSound(newLed);
}

void moveRandom(){
  int randomLed = getRandomLed();
  moveAndMakeSound(randomLed);
  randomizeDelay();
}

void move(int newLed){
  int oldLed = selectedLed;
  digitalWrite(ledPins[oldLed], LOW);
  digitalWrite(ledPins[newLed], HIGH);
  selectedLed = newLed;
}

void moveAndMakeSound(int newLed){
  move(newLed);
  makeSound(newLed);
}

void randomizeDelay(){
  delay(randomDelay);
}