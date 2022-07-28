/*const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {A10, A0, A1, A2, A3};*/
const int NUM_SLIDERS = 4;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3};
const int NUM_BUTTONS = 6;
const int buttonInputs[NUM_BUTTONS] = {9,8,7,6,5,4};

unsigned long previousMillis = 0;        // will store last time Data was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 100;           // interval at which to check sliders (milliseconds)

int sliderValues[NUM_SLIDERS];
int oldSliderValues[NUM_SLIDERS];

int buttonValues[NUM_BUTTONS];
int oldButtonValues[NUM_BUTTONS];

void setup() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
    analogRead(analogInputs[i]);
  }

  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonInputs[i], INPUT_PULLUP);
  }

  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  
  copy(buttonValues, oldButtonValues, NUM_BUTTONS);
  updateButtonValues();
  
  for (int i = 0;  i < NUM_BUTTONS; i++){
    if(oldButtonValues[i] != buttonValues[i]){
      if (buttonValues[i] == LOW){
        sendValues(); // Actually send data
        break;
      }
    }
  }

  if(currentMillis - previousMillis > interval) {
    // save the last time you changed slider Data 
    previousMillis = currentMillis;

    copy(sliderValues, oldSliderValues, NUM_SLIDERS);
    updateSliderValues();
    
    for (int i = 0;  i < NUM_SLIDERS; i++){
      if(oldSliderValues[i] != sliderValues[i]) 
      {
        sendValues(); // Actually send data
        break;
      }
    }
  }
}

void updateButtonValues() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttonValues[i] = digitalRead(buttonInputs[i]);
  }
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    //because of A/D acquisition time and inconsistencies delay and read twice
    sliderValues[i] = analogRead(analogInputs[i]);
    delay(10);
    sliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += "s";
    builtString += String((int)sliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }

  if(NUM_BUTTONS > 0){
    builtString += String("|");
  }

  for (int i = 0; i < NUM_BUTTONS; i++) {
    builtString += "b";
    builtString += String((int)buttonValues[i]);

    if (i < NUM_BUTTONS - 1) {
      builtString += String("|");
    }
  }

  Serial.println(builtString);
}

// Function to copy 'len' elements from 'src' to 'dst'
void copy(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}
