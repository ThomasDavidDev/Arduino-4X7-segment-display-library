/********************************************************************************
Display.h - Library for flashing Disp code.
Created by David D. & Thomas D. , 2014.
Released into the public domain.
********************************************************************************/

#ifndef Display_h

#define Display_h
#include "Arduino.h"

/********************************************************************************
Class
********************************************************************************/

class Display
{
public:
	Display(short segmentPins[8], short digitPins[4], short timer); //constructor
	Display(short segmentPins[8], short digitPins[4]); //constructor
	bool active;
	void setNumber(long value); //number to show on display
	void setSpecificNumber(short aNumber, short bNumber, short cNumber, short dNumber);
	void setSpecificDot(bool aDot, bool bDot, bool cDot, bool dDot);
	void stop();  //stop display
	void start(); //restart display
	void update(); //called by interrupts

private:
	short specific[4];
	short buffer;
	long valueBuffer;
	void makeReference(); //copy current instance to static instance
	short _segmentPins[8]; //= {23,25,27,29,31,33,35,37};
	short _digitPins[4]; //= {22,24,26,28};
	short dispArray[10]; // 10: maximum lenght long: 2,147,483,647
	short Timer;
	short pos;
	short activeStateFlag;
};

/********************************************************************************
Static
********************************************************************************/

//Please, dont use in your arduino sketch, this is static.
//copy of instances (needed for interrupts)
static Display(*d0[99]); //Don't modify directly
static Display(*d1[99]);
static Display(*d2[99]);
static Display(*d3[99]);
static Display(*d4[99]);
static Display(*d5[99]);

static short timerUse[5]={0, 0, 0, 0, 0};

static byte combs[15] = {
	0b11000000, //0
	0b11111001, //1
	0b10100100, //2
	0b10110000, //3 
	0b10011001, //4 
	0b10010010, //5 
	0b10000010, //6
	0b11111000, //7
	0b10000000, //8
	0b10010000, //9 
	0b01111111, //.
	0b11111111, //Empty
	0b10000110, //E
	0b11001110,	//r
	0b10111111,	//-
};

#endif
