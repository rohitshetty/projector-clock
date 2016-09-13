#ifndef FONTS_H
#define FONTS_H

#include<avr/pgmspace.h>

const unsigned char characterMap[][3] PROGMEM = {
    {0XF8,0X88,0XF8}, //0
    {0X48,0XF8,0X08}, //1
    {0XB8,0XA8,0XE8}, //2
    {0XA8,0XA8,0XF8}, //3
    {0XE0,0X20,0X38}, //4
    {0XE8,0XA8,0XB8}, //5
    {0XF8,0XA8,0XB8}, //6
    {0X80,0XA0,0XF8}, //7
    {0XF8,0XA8,0XF8}, //8
    {0XE0,0XA0,0XF8}, //9
    {0X00,0X50,0X00}, //:
    {},
    {},
    {},
    {},
    {},
    {},
    {0X78,0XA0,0X78}, //A
    {0XF8,0XA8,0X70}, //B
    {0XF8,0X88,0X88}, //C
    {0XF8,0X88,0X70}, //D
    {0XF8,0XA8,0XA8}, //E
    {0XF8,0XA0,0XA0}, //F
    {0XF8,0X88,0XB8}, //G
    {0XF8,0X20,0XF8}, //H
    {0X00,0XF8,0X00}, //I
    {0X98,0X88,0XF8}, //J
    {0XF8,0X20,0XD8}, //K
    {0XF8,0X08,0X08}, //L
    {0XF8,0X60,0XF8}, //M
    {0XF8,0X80,0XF8}, //N
    {0X70,0X88,0X70}, //O
    {0XF8,0XA0,0XE0}, //P
    {0XF0,0X98,0XF0}, //Q
    {0XF8,0XA0,0XD8}, //R
    {0XC8,0XA8,0X98}, //S
    {0X80,0XF8,0X80}, //T
    {0XF8,0X08,0XF8}, //U
    {0XF0,0X08,0XF0}, //V
    {0XF8,0X30,0XF8}, //W
    {0XD8,0X20,0XD8}, //X
    {0XC0,0X38,0XC0}, //Y
    {0X98,0XA8,0XC8}, //Z
    {0X88,0X88,0X70}, //]
    {0X00,0X00,0X00} //
};

#endif