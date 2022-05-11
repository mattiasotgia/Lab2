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
    Serial3.print(fmt);
    delay(100);
    return;
}

void data_routine() 
{

    int M = 13;
    int N = 100;

    for ( int i=0; i<M; i++ ) {

        for ( int j=0; j<N; j++ ) {
            digitalWrite(D5V_pin, HIGH);
            delay(100);
            // eseguire la misura di A0
            // eseguire la misura di A3
            // scrivere tutto sul seriale
            delay(100);
            digitalWrite(D5V_pin, LOW);
            delay(100);
            // eseguire la misura di A0
            // eseguire la misura di A3
            // scrivere tutto sul seriale
            delay(100);
        }
        serial3_sendmmsg_delay("INCI1");
    }
    return;
}


int main(void)
{
    init();

    Serial3.begin(9600);
    Serial.begin(9600);

    pinMode(D5V_pin, OUTPUT);
    pinMode(A0def, INPUT);
    pinMode(A1def, OUTPUT);

    serial3_sendmmsg_delay("V1 30\n");
    serial3_sendmmsg_delay("I1 0\n");
    serial3_sendmmsg_delay("OP1 1\n");
    serial3_sendmmsg_delay("DELTAI1 1e-1\n");

    // char buffer[40];

    while ( true ) {
        if ( Serial.available() > 0 ){
            if ( strcmp((Serial.readString()).c_str(), "?STR") == 0 ) {

                delay(100);
                Serial.print("*RUN\n");
                delay(100);
                
                data_routine();
                
                serial3_sendmmsg_delay("V1 0\n");
                serial3_sendmmsg_delay("I1 0\n");
                serial3_sendmmsg_delay("OP1 0\n");
                Serial.print("*STP\n");
            }
        }
    }

    return 0;
}
