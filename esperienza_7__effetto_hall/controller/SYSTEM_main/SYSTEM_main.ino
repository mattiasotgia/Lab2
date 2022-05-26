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

#define A0def   A10  // analog pin to read data form op-amp/strum V_H
#define D5V_pin 2   // digital pin: 5V write if masure w/ i, =V otherwise
#define A3def   A13  // analog pin to send V_x (reference for converter)

void Serial2_sendmmsg_delay(const char* fmt){
    Serial2.println(fmt);
    delay(1000);
    return;
}

void serial_writeboth(double V_gen, double V_out){
    Serial.print(V_out, 20);
    Serial.print("\n");
    Serial.print(V_gen, 20);
    Serial.print("\n");
};

void data_routine(const char* step) 
{

    int M = 12;
    int N = 100;

    double V_gen, V_out;

    for ( int i=0; i<M; i++ ) {

        Serial.print("*CYCLESTART\n");
        Serial.print("I:");
        Serial.print((float)i/10);
        Serial.print("\n");

        for ( int j=0; j<N; j++ ) {
            digitalWrite(D5V_pin, HIGH);
            delay(50);
            // eseguire la misura di A0
            V_out = (double)((double)analogRead(A0def)*5.0/(double)1023);
            // eseguire la misura di A3
            V_gen = (double)((double)analogRead(A3def)*5.0/(double)1023);
            // scrivere tutto sul seriale
            serial_writeboth(V_out, V_gen);

            delay(50);
            digitalWrite(D5V_pin, LOW);
            delay(50);
            // eseguire la misura di A0
            V_out = (double)((double)analogRead(A0def)*5.0/(double)1023);
            // eseguire la misura di A3
            V_gen = (double)((double)analogRead(A3def)*5.0/(double)1023);
            // scrivere tutto sul seriale
            serial_writeboth(V_out, V_gen);

            delay(50);
        }
        Serial2_sendmmsg_delay(step);
        Serial.print("CYCLEEND*\n");
        delay(100);
    }
    return;
}


int main(void)
{
    init();

    Serial2.begin(9600);
    Serial.begin(9600);

    pinMode(D5V_pin, OUTPUT);
    pinMode(A0def, INPUT);
    pinMode(A3def, INPUT);

    Serial2_sendmmsg_delay("V1 30");
    Serial2_sendmmsg_delay("I1 1.3");
    Serial2_sendmmsg_delay("OP1 1");
    Serial2_sendmmsg_delay("DELTAI1 1e-1");
    // analogReadResolution(12);

    // while ( true ) {

    delay(5000);
    Serial.print("*RUN\n");
    Serial.print("*BPOS\n");
    delay(100);
    
    data_routine("DECI1");
    Serial.print("Change polarization in B and in OPAMP input! (45s left)\n");
    delay(45000);
    // wait to change polarization for B
    

    Serial.print("*BPOS\n");
    delay(100);
    data_routine("INCI1");
            
    Serial2_sendmmsg_delay("V1 0");
    Serial2_sendmmsg_delay("I1 0");
    Serial2_sendmmsg_delay("OP1 0");
    // Serial.print("*STP\n");
        // Serial.
        // break;
    // }

    return 0;
}
