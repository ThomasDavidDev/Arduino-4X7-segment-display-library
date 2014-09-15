**Arduino-4X7-segment-display-library**
===================================


Dear arduino-user,

This is our first library for arduino. **Is this just yet another 4x7-segment library? NO.** Multiple timers, number formating, update the screen without flickering,.. And the best of all: it is all handled by the library! There is even multi-dispay support!

The number formating: This version of the library makes it possible to print in an easy way an integer to a 4X7 segment display. Positive numers higher than 9999 will be print as XXEX and negative numbers lower than -999 will be print as -XEX.

Timer: You can use any timer you want. (for Arduino mega users, uncomment some rules to use the 3th,4th and 5th timer.)

**Howto use?**
see included example

**functions**
-constructor: Display(short segmentPins[8], short digitPins[4], short timer);
-setNumber(long value)-setSpecificNumber(short firstDigit, short secondDigit, short thirdDigit, short ffourthDigit)
-stop()
-start()

I hope that the library is free of bugs. Feel free to comment and/ or adjust bugs or suggest extra functions And remember, respect the GPL2 license.
