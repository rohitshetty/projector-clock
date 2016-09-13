#ifndef DISPLAY_H
#define DISPLAY_H

extern volatile int flag;

void strcut(char *,char *,uint8_t,uint8_t);
void push_LDB(uint8_t);
void render(char *);
void init_timer0(void);
void init_timer1(void);



#endif