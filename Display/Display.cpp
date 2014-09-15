/********************************************************************************
Display.h - Library for flashing Disp code.
Created by David D. & Thomas D. , 2014.
Released into the public domain.
********************************************************************************/

#include "Arduino.h"
#include "Display.h"
#include "avr/interrupt.h"
#include <avr/io.h>
#include <util/atomic.h>

/********************************************************************************
Setup
********************************************************************************/

//constructor with user defined inputs
Display::Display(short segmentPins[8], short digitPins[4], short timer)
{
	for (short i = 0; i < 4; i++)
	{
		_digitPins[i] = digitPins[i];
		pinMode(_digitPins[i], OUTPUT);
		digitalWrite(_digitPins[i], LOW);
	}

	for (short i = 0; i < 8; i++){
		_segmentPins[i] = segmentPins[i];
		pinMode(_segmentPins[i], OUTPUT);
		digitalWrite(_segmentPins[i], HIGH);
	}

	Timer = timer;
	pos = 0;
	makeReference();
	start();
}

//constructor with default timer and user defined inputs
Display::Display(short segmentPins[8], short digitPins[4])
{
	for (short i = 0; i < 4; i++)
	{
		_digitPins[i] = digitPins[i];
		pinMode(_digitPins[i], OUTPUT);
		digitalWrite(_digitPins[i], LOW);
	}
	
	for (short i = 0; i < 8; i++){
		_segmentPins[i] = segmentPins[i];
		pinMode(_segmentPins[i], OUTPUT);
		digitalWrite(_segmentPins[i], HIGH);
	}

	Timer = 1;
	pos = 0;
	makeReference();
	start();
}

//copy current instance to static instance  (accessible by interrupts)
void Display::makeReference()
{
	switch (Timer){
	case 0:
		d0[timerUse[0]] = this;
		timerUse[0]++;
	case 1:
		d1[timerUse[1]] = this;
		timerUse[1]++;
	case 2:
		d2[timerUse[2]] = this;
		timerUse[2]++;
	case 3:
		d3[timerUse[3]] = this;
		timerUse[3]++;
	case 4:
		d4[timerUse[4]] = this;
		timerUse[4]++;
	case 5:
		d5[timerUse[5]] = this;
		timerUse[5]++;
		/*case 5:
		d5 = this;*/
	}
}

/********************************************************************************
Timer setup
********************************************************************************/

//setup timers to overflow after 4 millis
void Display::start()
{	
	active = true;
	switch (Timer){
	case 0: //Used for delay(), millis(), micros(), ...
		break;
	case 1:
		noInterrupts();
		TCCR1A = 0;
		TCCR1B = 0;
		TCCR1B |= (1 << CS10);
		//TCCR1B |= (0 << CS11);   
		//TCCR1B |= (0 << CS12);    
		TIMSK1 |= (1 << TOIE1);
		interrupts();
		break;
	case 2: //8bit   
		noInterrupts();
		TCCR2A = 0;
		TCCR2B = 0;
		//TCCR2B |= (0 << CS20); 
		//TCCR2B |= (0 << CS21);   //256 prescaler
		TCCR2B |= (1 << CS22);
		TIMSK2 |= (1 << TOIE2);
		interrupts();
		break;
/*vvvvvvvvvvvvvvvvv*Only for arduino mega*vvvvvvvvvvvvvvv*/
	/*case 3:
		noInterrupts();
		TCCR3A = 0;
		TCCR3B = 0;
		TCCR3B |= (1 << CS30);
		//TCCR3B |= (0 << CS31);   
		//TCCR3B |= (0 << CS32);    
		TIMSK3 |= (1 << TOIE3);
		interrupts();
		break;
	case 4:
		noInterrupts();
		TCCR4A = 0;
		TCCR4B = 0;
		TCCR4B |= (1 << CS40);
		//TCCR4B |= (0 << CS41);   
		//TCCR4B |= (0 << CS42);    
		TIMSK4 |= (1 << TOIE4);
		interrupts();
		break;
	case 5:
		noInterrupts();
		TCCR5A = 0;
		TCCR5B = 0;
		TCCR5B |= (1 << CS50);
		//TCCR5B |= (0 << CS51);   
		//TCCR5B |= (0 << CS52);    
		TIMSK5 |= (1 << TOIE5);
		interrupts();
		break;*/
/*^^^^^^^^^^^^^^^^^*Only for arduino mega*^^^^^^^^^^^^^^^*/
	default:
		break;
	}
}

//stop timers and update();
void Display::stop()
{	
	active = false;
	for (short i = 1; i < 5; i++){
		dispArray[i] = 11;
	}

	for (short i = 0; i < 4; i++)
	{
		digitalWrite(_digitPins[i], HIGH);
	}

	switch (Timer){
	case 0: //Used for delay(), millis(), micros(), ...
		break;
	case 1:
		noInterrupts();
		TCCR1A = 0;
		TCCR1B = 0;
		TIMSK1 |= (0 << TOIE1);
		interrupts();
		break;
	case 2: //8bit   
		noInterrupts();
		TCCR2A = 0;
		TCCR2B = 0;
		TIMSK2 |= (0 << TOIE2);
		interrupts();
		break;
/*vvvvvvvvvvvvvvvvv*Only for arduino mega*vvvvvvvvvvvvvvv*/
	/*case 3:
		noInterrupts();
		TCCR3A = 0;
		TCCR3B = 0;
		TIMSK3 |= (0 << TOIE3);
		interrupts();
		break;
	case 4:
		noInterrupts();
		TCCR4A = 0;
		TCCR4B = 0;
		TIMSK4 |= (0 << TOIE4);
		interrupts();
		break;
	case 5:
		noInterrupts();
		TCCR5A = 0;
		TCCR5B = 0;
		TIMSK5 |= (0 << TOIE5);
		interrupts();
		break;*/
/*^^^^^^^^^^^^^^^^^*Only for arduino mega*^^^^^^^^^^^^^^^*/
	default:
		break;
	}
}

/********************************************************************************
Sketch callable functions
********************************************************************************/

//set pins 4x7 seg display
void Display::update(){
	digitalWrite(_digitPins[pos], 1);
	(pos == 3) ? pos = 0 : pos++;
	buffer = combs[dispArray[4 - pos]];
	for (short i = 0; i < 8; i++){
		digitalWrite(_segmentPins[i], (buffer>>i)&1 );
	}
	digitalWrite(_digitPins[pos], 0);
}

void Display::setSpecificNumber(short a, short b, short c, short d)
{	
	specific[4] = a;
	specific[3] = b;
	specific[2] = c;
	specific[1] = d;
	for (short i = 0; i < 5; i++){
		if (specific[i]>9){//Err
			dispArray[1] = 13;
			dispArray[2] = 13;
			dispArray[3] = 12;
			dispArray[4] = 11;
			break;
		}
		else {
			dispArray[i] = specific[i];
		}
	}
}

//Formatting input to displayable number
void Display::setNumber(long value)
{
	noInterrupts();
	
	//forms: ___x  ; __xx ; _xxx ; xxxx ; xxex ;
	//setting every number to seperated array + get lenght of array
	if ((valueBuffer == 0) && (value >= 0)){
		valueBuffer = value;
		short lenghtOfArray = 1;
		while (true) {
			dispArray[lenghtOfArray] = valueBuffer % 10; // % = modulo : e.g. 7 % 5 = 2 -- 7/5 = 1 rest 2 , 27%13=2 rest=1
			valueBuffer = (valueBuffer - dispArray[lenghtOfArray]) / 10;
			if (valueBuffer == 0){ break; }
			lenghtOfArray++;
		}

		switch (lenghtOfArray){
		case 1: // ___x
			dispArray[4] = 11;
			dispArray[3] = 11;
			dispArray[2] = 11;
			break;
		case 2: // __xx
			dispArray[4] = 11;
			dispArray[3] = 11;
			break;
		case 3: // _xxx
			dispArray[4] = 11;
			break;
		case 4:  // xxxx
			break;
		default: //groter of gelijk aan 5 // xxex
			dispArray[3] = dispArray[lenghtOfArray - 1];
			dispArray[4] = dispArray[lenghtOfArray];
			dispArray[2] = 12; //letter e
			dispArray[1] = lenghtOfArray - 2; //macht van 10
			break;
		}
	}
	else if ((valueBuffer == 0) && (value < 0)){
		valueBuffer = abs(value);
		short lenghtOfArray = 1;
		while (true) {
			dispArray[lenghtOfArray] = valueBuffer % 10; // % = modulo : e.g. 7 % 5 = 2 -- 7/5 = 1 rest 2 , 27%13=2 rest=1
			valueBuffer = (valueBuffer - dispArray[lenghtOfArray]) / 10;
			if (valueBuffer == 0){ break; }
			lenghtOfArray++;
		}
		
		switch (lenghtOfArray){
		case 1: // ___x
			dispArray[4] = 11;
			dispArray[3] = 11;
			dispArray[2] = 14;
			break;
		case 2: // __xx
			dispArray[4] = 11;
			dispArray[3] = 14;
			break;
		case 3: // _xxx
			dispArray[4] = 14;
			break;
		default: //groter of gelijk aan 5 // xxex
			dispArray[3] = dispArray[lenghtOfArray];
			dispArray[4] = 14;
			dispArray[2] = 12; //letter e
			dispArray[1] = lenghtOfArray - 2; //macht van 10
			break;
		}
	}
	interrupts();
}

/********************************************************************************
Interrupts
********************************************************************************/

//timers: overflow interrupt
ISR(TIMER1_OVF_vect)
{
	for (short i = 0; i < timerUse[1]; i++){
		(*d1[i]).update();
	}
}
ISR(TIMER2_OVF_vect)
{
	for (short i = 0; i < timerUse[2]; i++){
		(*d2[i]).update();
	}
}
ISR(TIMER3_OVF_vect)
{
	for (short i = 0; i < timerUse[3]; i++){
		(*d3[i]).update();
	}
}
ISR(TIMER4_OVF_vect)
{
	for (short i = 0; i < timerUse[4]; i++){
		(*d4[i]).update();
	}
}
ISR(TIMER5_OVF_vect)
{
	for (short i = 0; i < timerUse[5]; i++){
		(*d5[i]).update();
	}
}
