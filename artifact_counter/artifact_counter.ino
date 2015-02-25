const int MOMENTARY_PINS[] = {2, 3, 4, 5, 6, 7, 11, 12};
const int PIN_SERIAL = 8;
const int PIN_LATCH = 9;
const int PIN_CLOCK = 10;
const long BOUNCE_DELAY = 50;

// For debouncing the momentary-on buttons.
const int NUM_MOMENTARY_BUTTONS = sizeof(MOMENTARY_PINS)/sizeof(int);
int MOMENTARY_BUTTON_STATES[sizeof(MOMENTARY_PINS)];
int LAST_MOMENTARY_BUTTON_STATES[sizeof(MOMENTARY_PINS)];
long LAST_MOMENTARY_BUTTON_DEBOUNCE_TIMES[sizeof(MOMENTARY_PINS)];

// Integer values for the seven-segment LEDS for each digit (0-9). The ordinal position represents the binary value for that digit.
const int SEVEN_SEG_BINARY[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};

// The initial value of each seven-segment LED for each player.
int ledValues[4][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
int currentSet = 0;

// We can be in one of two modes: COUNTER or CALCULATOR.
// TODO: Use an enum for MODES
char* MODES[] = {"COUNTER", "CALCULATOR"};

// The default mode is COUNTER
char* currentMode = MODES[0];

void setup() {
  Serial.begin(9600);
  for (int i=0; i<NUM_MOMENTARY_BUTTONS; i++) {
    pinMode(MOMENTARY_PINS[i], INPUT_PULLUP);
    MOMENTARY_BUTTON_STATES[i] = HIGH;
    LAST_MOMENTARY_BUTTON_STATES[i] = HIGH;
    LAST_MOMENTARY_BUTTON_DEBOUNCE_TIMES[i] = 0;
  }
  pinMode(PIN_SERIAL, OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  
  render();
}

void loop() {
  int momentaryOnButtonStates[NUM_MOMENTARY_BUTTONS];
  readMomentaryOnButtons(momentaryOnButtonStates);
  
  for (int i=0; i<NUM_MOMENTARY_BUTTONS; i++) {
     if (momentaryOnButtonStates[i] == 1) {
       switch(i) {
         case 0:
           if (ledValues[currentSet][0] == 0) {
             ledValues[currentSet][0] = 9;
           } else {
             ledValues[currentSet][0]--;
           }
           break;
         case 1:
           if (ledValues[currentSet][0] == 9) {
             ledValues[currentSet][0] = 0;
           } else {
             ledValues[currentSet][0]++;
           }
           break;
         case 2:
           if (ledValues[currentSet][1] == 0) {
             ledValues[currentSet][1] = 9;
           } else {
             ledValues[currentSet][1]--;
           }
           break;
         case 3:
           if (ledValues[currentSet][1] == 9) {
             ledValues[currentSet][1] = 0;
           } else {
             ledValues[currentSet][1]++;
           }
           break;
         case 4:
           if (ledValues[currentSet][2] == 0) {
             ledValues[currentSet][2] = 9;
           } else {
             ledValues[currentSet][2]--;
           }
           break;
         case 5:
           if (ledValues[currentSet][2] == 9) {
             ledValues[currentSet][2] = 0;
           } else {
             ledValues[currentSet][2]++;
           }
           break;
         case 6:
           if (currentSet == 0) {
             currentSet = 3;
           } else {
             currentSet--;
           }
           break;
         case 7:
           if (currentSet == 3) {
             currentSet = 0;
           } else {
             currentSet++;
           }
           break;
       }
       render();
     }
  }
}

void render() {
  digitalWrite(PIN_LATCH, LOW);
  for (int i = 2; i >= 0; i--) {
    int shiftValue = SEVEN_SEG_BINARY[ledValues[currentSet][i]];
    shiftOut(PIN_SERIAL, PIN_CLOCK, MSBFIRST, shiftValue);
  }
  digitalWrite(PIN_LATCH, HIGH);
}

void readMomentaryOnButtons(int buttonStates[]) {
  for (int i=0; i<NUM_MOMENTARY_BUTTONS; i++) {
    buttonStates[i] = 0;
    
    int reading = digitalRead(MOMENTARY_PINS[i]);
    
    if (reading != LAST_MOMENTARY_BUTTON_STATES[i]) {
      LAST_MOMENTARY_BUTTON_DEBOUNCE_TIMES[i] = millis();
    } 
  
    if ((millis() - LAST_MOMENTARY_BUTTON_DEBOUNCE_TIMES[i]) > BOUNCE_DELAY) {
      if (reading != MOMENTARY_BUTTON_STATES[i]) {
        MOMENTARY_BUTTON_STATES[i] = reading;
  
        if (MOMENTARY_BUTTON_STATES[i] == LOW) {
          buttonStates[i] = 1;
        }
      }
    }
    
    LAST_MOMENTARY_BUTTON_STATES[i] = reading;
  }
}
