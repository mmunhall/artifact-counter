const int MOMENTARY_PINS[] = {2, 3, 4, 5, 6, 7};
const int PIN_SERIAL = 8;
const int PIN_LATCH = 9;
const int PIN_CLOCK = 10;
const long BOUNCE_DELAY = 50;

// For debouncing the momentary-on buttons.
int BUTTON_STATES[sizeof(MOMENTARY_PINS)];
int LAST_BUTTON_STATES[sizeof(MOMENTARY_PINS)];
long LAST_BUTTON_DEBOUNCE_TIMES[sizeof(MOMENTARY_PINS)];

// Integer values for the seven-segment LEDS for each digit (0-9). The ordinal position represents the binary value for that digit.
const int SEVEN_SEG_BINARY[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};

// The initial value of each seven-segment LED.
int ledValues[] = {0, 0, 0};

// We can be in one of two modes: COUNTER or CALCULATOR.
// TODO: Use an enum for MODES
char* MODES[] = {"COUNTER", "CALCULATOR"};

// The default mode is COUNTER
char* currentMode = MODES[0];

void setup() {
  Serial.begin(9600);
  for (int i=0; i<sizeof(MOMENTARY_PINS); i++) {
    pinMode(MOMENTARY_PINS[i], INPUT_PULLUP);
    BUTTON_STATES[i] = HIGH;
    LAST_BUTTON_STATES[i] = HIGH;
    LAST_BUTTON_DEBOUNCE_TIMES[i] = 0;
  }
  pinMode(PIN_SERIAL, OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  
  render();
}

void loop() {
  Serial.println(sizeof(MOMENTARY_PINS));
  int momentaryOnButtonStates[sizeof(MOMENTARY_PINS)];
  readMomentaryOnButtons(momentaryOnButtonStates);
  
  for (int i=0; i<sizeof(momentaryOnButtonStates); i++) {
     if (momentaryOnButtonStates[i] == 1) {
       Serial.print(i);
       Serial.println(" was pressed!"); 
     }
  }
}

void render() {
  digitalWrite(PIN_LATCH, LOW);
  for (int i = 2; i >= 0; i--) {
    int shiftValue = SEVEN_SEG_BINARY[ledValues[i]];
    shiftOut(PIN_SERIAL, PIN_CLOCK, MSBFIRST, shiftValue);
  }
  digitalWrite(PIN_LATCH, HIGH);
}

void readMomentaryOnButtons(int buttonStates[]) {
  for (int i=0; i<sizeof(MOMENTARY_PINS); i++) {
    buttonStates[i] = 0;
    
    int reading = digitalRead(MOMENTARY_PINS[i]);
    
    if (reading != LAST_BUTTON_STATES[i]) {
      LAST_BUTTON_DEBOUNCE_TIMES[i] = millis();
    } 
  
    if ((millis() - LAST_BUTTON_DEBOUNCE_TIMES[i]) > BOUNCE_DELAY) {
      if (reading != BUTTON_STATES[i]) {
        BUTTON_STATES[i] = reading;
  
        if (BUTTON_STATES[i] == LOW) {
          buttonStates[i] = 1;
        }
      }
    }
    
    LAST_BUTTON_STATES[i] = reading;
  }
}
