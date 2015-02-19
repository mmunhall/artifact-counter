
// These values are configurable and definee the pins used on the Arduino.
const int PIN_SERIAL = 8;
const int PIN_LATCH = 9;
const int PIN_CLOCK = 10;

// Integer values for the seven-segment LEDS for each digit (0-9). The ordinal position represents the binary value for that digit.
const int SEVEN_SEG_BINARY[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};

// The initial value of each seven-segment LED.
int LED_VALUES[] = {0, 1, 2};

const int NUM_DIGITS = sizeof(LED_VALUES)/sizeof(int);

void setup() {
  Serial.begin(9600);
  pinMode(PIN_SERIAL, OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
}

void loop() {
  render();
  delay(10000);
}

void render() {
  digitalWrite(PIN_LATCH, LOW);
  for (int i = NUM_DIGITS - 1; i >= 0; i--) {
    int shiftValue = SEVEN_SEG_BINARY[LED_VALUES[i]];
    shiftOut(PIN_SERIAL, PIN_CLOCK, MSBFIRST, shiftValue);
  }
  digitalWrite(PIN_LATCH, HIGH);
}
