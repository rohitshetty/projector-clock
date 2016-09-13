#define F_CPU 12000000UL
#include<avr/io.h>
#include<avr/interrupt.h>   
#include<util/delay.h>
#include<avr/pgmspace.h>
#include<avr/sleep.h>

#include "display/display.h"

/*
*	Hardware: 
*	7(rows)*5(columns) LED matrix. without driver. 

	A 	B 	C 	D 	E 	F 	G (PORTB)
1	*	*	*	*	*	*	*

2	*	*	*	*	*	*	*
	
3	*	*	*	*	*	*	*
	
4	*	*	*	*	*	*	*
	
5	*	*	*	*	*	*	*
(^ PORTA)

*	each * is a led with its row(A-G) being vcc and column(1-5) being grounds. 
*	That is to turn on say C3 LED, ground 3rd pin and vcc to C pin.

*	row (A-G) is connected to Port B
*	column (1-5) connected to Port A
*	That is any LED can be turned on or off by accessing PORTA and PORTB  
*	(ex. for C3 you would flip 3rd bit of portA low and 3rd bit of PORTB high).
*	display.c is used to render data on this matrix. and to give scroll effect
*	rtc.c helps to read, write and set ds1307 rtc.
*	Happy hacking :)
*/

void main(){
	char datetime[21];
	char hhmmss[9];
	DDRA=0xff;
	DDRB=0xff;
	init_timer0(); // used to update whatever is there in lowerlevel buffer.
	init_timer1(); // pushes from secondary buffer to lower buffer. for scrolling
	serial_init();
	RTCinit();
	// push button
	DDRD &=~(1<<PD2) ;
	PORTD |=(1<<PD2);
	sei(); //interrupt enable global
	render("PROJECTOR CLOCK LOADING ... ");

	// flag comes from display library. it is used to find number of scrolls occured. 

	while(flag<2);
	flag = 0;
	// GICR = (1<<INT0); // put into auto sleep to reduce power
	// set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	// RTCSet("22:03:20::3:13:09:16"); //hhmmssDddmmyy
	while(1){
	  
		// sleep_enable();
		// sleep_cpu();
		// sleep_disable();
	
		RTCRead(datetime);
		strcut(datetime,hhmmss,0,8);
		render(hhmmss);
		while(flag<1);
		flag = 0;
		// cleans the display (TODO write clear_screen() method)
		PORTA = 0x00; 
		PORTB = 0x00;
	}
}

