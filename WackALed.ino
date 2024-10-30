byte minLed = 7;
byte maxLed = 13;
byte buttonPin = 4;

void setup() {
    Serial.begin(9600);

    for (byte led = minLed; led <= maxLed; led++) pinMode(led, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
}