#include<avr/io.h>
#include<avr/interrupt.h>
#include "fonts.h"
#include "display.h"

#define MIRROR 1

volatile unsigned char low_display_buffer[7];
volatile uint8_t global_counter = 0;
volatile unsigned char push_buffer[150];
volatile uint8_t counter_PB=0;
volatile int flag = 0;

/*
*   Display is a LED matrix of 7(row)X5(column) size. Hence we use one byte to represent one row.
*   Data is updated row by row. Data for one frame would be an array of size 7 with each member of 1 byte.
*   Each array member is one row.
*   To display we use the PoV technique. Each of the row is turned on and off for small amount of time. 
*   (Here controlled by timer 0).
*   Data for this frame is maintained in a lower buffer. `lower_display_buffer`. Whatever is written
*   in this buffer is directly rendered.
*   Scrolling effect is given by storing characters in secondary buffer, `push_buffer`.
*   timer 1 periodically updates/ copies data from secondary buffer to lower buffer, using push_LDB().
*   This is cyclic in nature. This also provides a flag variable. This is used to let the main code know
*   the number of scroll.
*   Any character is rendered using render() function. currently supports only upper case.
*   font size is 3(row)*5(column)
*/

void strcut(char * source, char *target,uint8_t from,uint8_t to){
    // This is replica of standard string library cut function.
    uint8_t counter;
    for(counter = from;counter<to;counter++){
        target[counter] = source[counter];
    }
    target[counter++]=' ';
    target[counter] = '\0';
}


void push_LDB(uint8_t data){
    //this function is used to push the new data into display buffer. this is the reason for scrolling
    uint8_t counter;
    for(counter=0;counter<6;counter++){
        low_display_buffer[counter] = low_display_buffer[counter+1];
    }
    low_display_buffer[counter] = data;
}

void render(char * string){
    //This is used to render characters to 5X7. It is mapped using characterMap at fonts.h
    char local_loop, character_loop;
    for(local_loop=0;string[local_loop]!='\0';local_loop++){
        if(string[local_loop]==' '){
            //space
            for(character_loop=0;character_loop<=2;character_loop++){
                push_buffer[local_loop*4 + character_loop] = 0x00;
            }
            push_buffer[local_loop*4 + character_loop] = 0x00;
        }else{
            for(character_loop=0;character_loop<=2;character_loop++){
                // read the font from Flash for particular ascii character
                push_buffer[local_loop*4 + character_loop] = pgm_read_byte(&characterMap[string[local_loop]-48][character_loop]);
            }
            push_buffer[local_loop*4 + character_loop] = 0x00;
        }

    }
    push_buffer[local_loop*4] = 0xff;
}

ISR(TIMER1_COMPA_vect){
    //This regularly pushes data from secondary buffer to low level buffer
    if(push_buffer[counter_PB]==0xff){
        counter_PB = 0;
        flag ++;
    }

    push_LDB(push_buffer[counter_PB]);
    counter_PB++;
}


ISR(TIMER0_OVF_vect){
    //This is the lowlest level buffer function. This regularly scans and updates whatever present in low_display_buffer
	if (global_counter>6)
		global_counter = 0;

    #if defined MIRROR && MIRROR == 1
    // This is used for projector clock, where it is needed to invert and flip the rendering.
        PORTB = 1<<(global_counter);
    #else
        PORTB = 1 << (6-global_counter);
    #endif

	PORTA = ~(low_display_buffer[global_counter]);
	global_counter++;
}