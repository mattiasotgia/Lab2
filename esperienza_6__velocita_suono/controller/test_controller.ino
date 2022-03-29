/**
 * @file test_controller.ino
 * @author Mattia Sotgia 
 * @brief testing 
 * @version 0.1
 * @date 2022-03-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

int led = LED_BUILTIN;
volatile byte ledStatus = LOW;

void setup(){
    Serial.begin(9600);
    pinMode(led, OUTPUT);
}

void loop(){
    digitalWrite(led, ledStatus);
    Serial.println("Hello!\n");
    delay(1000);
    ledStatus=!ledStatus;
}