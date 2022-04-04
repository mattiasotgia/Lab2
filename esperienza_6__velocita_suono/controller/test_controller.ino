

void setup(){
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(2), start, RISING);
    attachInterrupt(digitalPinToInterrupt(3), stop, RISING);
}

double strtime = 0;

void loop(){
    Serial.println("Running!");
    delay(1000);
}

void start(){
    Serial.print("pin.2 (start) at time (ms): ");
    Serial.println(millis());
    strtime = millis();
}

void stop(){
    Serial.print("pin.3 (stop)  at time (ms): ");
    Serial.println(millis()-strtime);
}