// IRremote - Version: Latest 
#include <IRremote.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

const int ledCount = 9;
const int inputCount = 6;
const int buttonNext = 2;
const int buttonPrev = 4;
const int buttonRandom = 3;
const int remoteButtonNext = 14;
const int remoteButtonPrev = 15;
const int remoteButtonRandom = 16;
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
int inputPins[inputCount] = {buttonNext, buttonPrev, buttonRandom, remoteButtonNext, remoteButtonPrev, remoteButtonRandom};
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
#define IR_UP 16601263
#define IR_DOWN 16584943
#define IR_SELECT 16617583

IRrecv irrecv(remoteIR);
decode_results results;

void setup() {
  initIR();
  initInputs();
  initLeds();
  startSequence();
}

void loop() {
  if(isRandomizing()){
    continueRandomize();
  } else {
    readInputs();
    handleButtonPress();
  }
}

void startSequence(){
  int i;
  for(i = ledCount - 1; i >= 0; i--){
    delay(200);
    moveAndMakeSound(i);
  }
  delay(500);
  moveAndMakeSound(getRandom(false));
}

void readInputs(){
  int i;
  int irReading = readIR();
  if(irReading != 0) {
    buttonState[irReading] = true;
  } else {
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
}

int readIR(){
  int irBtnPressed = 0;
  if (irrecv.decode(&results))
  {
    if (results.value == IR_UP)
      irBtnPressed = buttonPrevIndex;
    else if (results.value == IR_DOWN)
      irBtnPressed = buttonNextIndex;
    else if (results.value == IR_SELECT)
      irBtnPressed = buttonRandomIndex;
    
    delay(200);

    irrecv.resume();
  }
  
  return irBtnPressed;
}

bool isRandomizing(){
  return randomCountdown > 0;
}

void startRandomize(){
  int countdownSpread = randomizeMaxStepCount - randomizeMinStepCount;
  int rnd = random(0, countdownSpread);
  randomCountdown = randomizeMinStepCount + rnd;
  moveRandom();
}

void continueRandomize(){
  randomCountdown -= 1;
  moveRandom();
  if(randomCountdown == 0){
    stopSound();
  }
}

void makeSound(int i){
  //tone(buzzer, notes[i], tonePause);
}

void stopSound(){
  //noTone(buzzer);
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

void initIR(){
  Serial.begin(9600);
  irrecv.enableIRIn();
}

int getRandom(bool forceNew){
  int reading = analogRead(A4);
  int rnd = reading % 8;
  return forceNew && rnd == selectedLed ? getRandom(forceNew) : rnd;
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
  int rnd = getRandom(true);
  moveAndMakeSound(rnd);
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
