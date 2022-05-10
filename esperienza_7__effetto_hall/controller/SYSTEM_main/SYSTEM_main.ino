/**
 * @file SYSTEM_main.ino
 * @author Mattia Sotgia (s4942225@studenti.unige.it)
 * @brief 
 * @version 0.1
 * @date 2022-05-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define A0def   A0  // analog pin to read data form op-amp/strum V_H
#define D5V_pin 2   // digital pin: 5V write if masure w/ i, =V otherwise
#define A1def   A1  // analog pin to send V_x (reference for converter)

void serial3_sendmmsg_delay(const char* fmt){
    Serial3.println(fmt);
    delay(100);
    return;
}

int main(void)
{
    init();

    Serial3.begin(9600);
    Serial.begin(9600);

    PinMode(D5V_pin, OUTPUT);
    PinMode(A0def, INPUT);
    PinMode(A1def, OUTPUT);

    serial3_sendmmsg_delay("");



    return 0;
}