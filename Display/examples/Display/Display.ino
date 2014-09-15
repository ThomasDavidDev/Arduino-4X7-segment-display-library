
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
  //delay(100);
  //analogWrite(12,100);    //To create a brithness pin value between 0 and 1023
  i++;
  mydisplay.setNumber(i);   //Print number i on your display.
  if (i==10000){
    i=0; 
    mydisplay.stop();       //Stop Display function + crl number.
    delay(1000);
    mydisplay.start();      //Restart Display.
    }
}