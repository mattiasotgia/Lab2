

void setup(){
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(2), start, RISING);
    attachInterrupt(digitalPinToInterrupt(3), stop, RISING);
}

double strtime = 0;

void loop(){
}

void start(){
    Serial.println("** RUN data result **");
    strtime = millis();
}

void stop(){
    Serial.print("time from reset: ");
    Serial.println(millis()-strtime);
}