#include "ClockModule.h"

Clock::Clock(){
    hour = 10;
    min = 0;
    sec = 0;
}

void Clock::showTime(N5110 &lcd){
    lcd.clear();
    lcd.printString("10:0:0", 0, 3);
    lcd.refresh();
}

void Clock::setTime(){
    hour = 10;
    min = 0;
    sec = 0;
}