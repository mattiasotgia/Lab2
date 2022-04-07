/**
 * @file DATAread.ino
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


const int        fNdataread = 50;       ///> number of points in single run

const int           swPin = 22;

volatile bool       _fallingedge = false;
volatile bool       _risingedge = false;
volatile bool       isRISING = false;
volatile bool       isENABLE = false;

volatile unsigned long  strttime = 0;


///////////////////////////////////////////////////////////////////////////////

volatile int            countOnRise = 0;
volatile int    countOnFall = 0;
volatile unsigned long  timeOnRise[2][fNdataread]; ///>[fNdatareads] time readings for rising edge: 0 = falling edge, 1 = rising edge

void resetCLK_TM_OnRise(){
    /* Time reset
       t0 = micros()
    */

   strttime = micros();

    _fallingedge = true;
    _risingedge = true;

    isRISING = true;

    countOnRise++;

}

///////////////////////////////////////////////////////////////////////////////

volatile unsigned long   timeOnFall[2][fNdataread]; ///>[fNdatareads] time readings for falling edge: 0 = falling edge, 1 = rising edge

void resetCLK_TM_OnFall(){
    /* Time reset
       t0 = micros()
    */

   strttime = micros();

    _fallingedge = true;
    _risingedge = true;

    countOnFall++;

}

///////////////////////////////////////////////////////////////////////////////

void stopCLK_TM_falling(){
    if(_fallingedge){
        if(isRISING){
            timeOnRise[0][countOnRise-1] = micros()-strttime;
            _fallingedge = false;
        } else {
            timeOnFall[0][countOnFall-1] = micros()-strttime;
            _fallingedge = false;
        }
    }
}

void stopCLK_TM_rising(){
    if(_risingedge){
        if(isRISING){
            timeOnRise[1][countOnRise-1] = micros()-strttime;
            _risingedge = false;
        }else{
            timeOnFall[1][countOnFall-1] = micros()-strttime;
            _risingedge = false;
        }
    }

    isRISING = false;
}

///////////////////////////////////////////////////////////////////////////////

void writeTimeSData(volatile unsigned long tRead[2][fNdataread], int size = fNdataread){
    for(int i=0; i<2; i++){
        for(int j=0; j<size; j++){
            Serial.println(tRead[i][j]);
        }
    }
    return;
}

void enable(){
    isENABLE = !isENABLE;
}

void setup(){

    attachInterrupt(digitalPinToInterrupt(swPin), enable, CHANGE);

    Serial.begin(9600);

    // initialize array
    for(int i=0; i<2; i++){
        for(int j=0; j<fNdataread; j++){
            timeOnRise[i][j] = 0;
            timeOnFall[i][j] = 0;
        }
    }
    // verify initial setup for array
    writeTimeSData(timeOnRise);
    writeTimeSData(timeOnFall);


}

void loop(){

    if(isENABLE && countOnFall<50){
        Serial.println(countOnFall);

        attachInterrupt(digitalPinToInterrupt(2), resetCLK_TM_OnRise, RISING);
        attachInterrupt(digitalPinToInterrupt(3), resetCLK_TM_OnFall, FALLING);
        attachInterrupt(digitalPinToInterrupt(4), stopCLK_TM_falling, FALLING);
        attachInterrupt(digitalPinToInterrupt(5), stopCLK_TM_rising, RISING);
    } else if (!isENABLE) {
        detachInterrupt(digitalPinToInterrupt(2));
        detachInterrupt(digitalPinToInterrupt(3));
        detachInterrupt(digitalPinToInterrupt(4));
        detachInterrupt(digitalPinToInterrupt(5));
        countOnFall = 0;
    }

    if(countOnFall == fNdataread){
        detachInterrupt(digitalPinToInterrupt(2));
        detachInterrupt(digitalPinToInterrupt(3));
        detachInterrupt(digitalPinToInterrupt(4));
        detachInterrupt(digitalPinToInterrupt(5));
        Serial.println("** RUN data result **");
        writeTimeSData(timeOnFall);
        writeTimeSData(timeOnRise);
        countOnFall++;
    }

}
