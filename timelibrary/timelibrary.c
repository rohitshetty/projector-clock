#include<avr/io.h>
#include "timelibrary.h"
// initialization of internal timers.
void init_timer0(void){
    TCCR0 |= (1<<CS01)|(1<<CS00);
    TIMSK |= (1<<TOIE0);
    TCNT0 = 0;
}

void init_timer1(void){
    TCCR1B |=(1<<CS11)|(1<<CS10)|(1<<WGM12);
    TCNT1=0;
    OCR1A = 20000;
    TIMSK|=(1<<OCIE1A);
}

void init_timer2(void){
    TCCR0 |= (1<<CS01)|(1<<CS00);
    TIMSK |= (1<<TOIE0);
    TCNT0 = 0;
}