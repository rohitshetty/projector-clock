#include<avr/io.h>
#include<avr/interrupt.h>



ISR(INT0_vect)
{
return;
}



ISR(USART_RXC_vect){
	//This is to set RTC time.

}
