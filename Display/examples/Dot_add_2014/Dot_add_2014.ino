
/*
 4 Segment Display
 
 This sketch let show a number on your segment display.
 
 This example is written and tested on a Arduino Mega 2560. 
 Please report compatibility problems.
 
 created May 2014
 by Thomas Devoogdt 
 && David Devoogdt
 */

#include <Display.h>

int i;

//Set Pins + Choose timer
short segment[8] = {0,1,2,3,4,5,6,7};
short digit[4] = {8,9,10,11};
//Segment pins, Digit pins, Used timer
Display mydisplay = Display(segment,digit,2);
//Segment pins, Digit pins, Used timer = 2
//Display mydisplay = Display(segment,digit);

void setup() { 

}

void loop(){
 mydisplay.setSpecificNumber(2,0,1,4);
 mydisplay.setSpecificDot(1,0,0,0);
 delay(150);
 mydisplay.setSpecificDot(0,1,0,0);
 delay(150);
 mydisplay.setSpecificDot(0,0,1,0);
 delay(150);
 mydisplay.setSpecificDot(0,0,0,1);
 delay(150);
 


}
