
int led = LED_BUILTIN;
volatile byte ledStatus = LOW;

int ai = 2;

void setup(){
    Serial.begin(9600);
    pinMode(led, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(ai), handler, LOW);
}

void loop(){
    Serial.println("Running!");
    delay(1000);
}

void handler(){
    digitalWrite(led, ledStatus);
    Serial.println("LED!");
    delay(1000);
    ledStatus=!ledStatus;
}