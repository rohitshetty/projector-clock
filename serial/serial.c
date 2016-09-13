#include<avr/io.h>
#include "serial.h"

#define F_CPU 12000000UL
#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
/*
*	This is UART driver.
*/
struct stack {
	char stack_array[20];
	char pointer;
};


void serial_init(){
	UBRRH = (BAUDRATE>>8);
	UBRRL = (BAUDRATE);
	UCSRB =(1<<RXCIE)|(1<<RXEN)|(1<<TXEN);
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}

void serial_send_string(char array[]){
	int i;
	for(i=0;array[i]!='\0';i++){
		while(!(UCSRA & (1<<UDRE)));
		UDR = array[i];
	}
	return;
}

void send_float(float num){
	uint16_t mant,deci;
	mant = (int) num;
	deci = (num-(mant*1.0))*1000000;
	int_to_ascii(mant);
	serial_send('.');
	int_to_ascii(deci);
	serial_send('\n');
	return;
}

void send_int(uint8_t num){
	int_to_ascii(num);
	serial_send('\n');
	return;
}
void int_to_ascii(uint8_t num){
	struct stack LIFO;
	uint8_t rem;
	LIFO.pointer=0;
	for(LIFO.pointer=0;num!=0;LIFO.pointer++){
		rem = num%10;
		num = num/10;
		LIFO.stack_array[LIFO.pointer] = rem+48;
	}
	for(LIFO.pointer--;LIFO.pointer>=0;LIFO.pointer--){
		while(!(UCSRA & (1<<UDRE)));
		UDR = LIFO.stack_array[LIFO.pointer];
	}
	return;
}

void serial_send(char data){
	while(!(UCSRA & (1<<UDRE)));
	UDR = data;
	return;
}
