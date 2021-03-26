#include "ClockModule.h"
#include <cstring>
#include <string>

using namespace std;

Clock::Clock(){
    hour = 10;
    min = 0;
    sec = 0;
    year = 2021;
    day = 10;
    month = 2;
}

void Clock::showTime(N5110 &lcd,DigitalIn &buttonC){

    string temp_hour;
    string temp_min;
    string temp_sec;
    string temp_date;
    
    while (1) {

        lcd.clear();
        processTime();
        temp_hour = to_string(hour);
        temp_min = to_string(min);
        temp_sec = to_string(sec);
        temp_date = to_string(day);
        
        temp_hour.append(":");
        temp_hour.append(temp_min);
        temp_hour.append(":");
        temp_hour.append(temp_sec);

        temp_date.append("/");
        temp_date.append(to_string(month));
        temp_date.append("/");
        temp_date.append(to_string(year));

        lcd.printString(temp_date.c_str(), 10, 1);
        lcd.printString(temp_hour.c_str(), 10, 2);
        lcd.printString("press C exit", 0, 4);
        lcd.refresh();
        thread_sleep_for(500);

        if (buttonC.read()) {
            return;
        }
        
    }
}

void Clock::setTime(){
    hour = 10;
    min = 0;
    sec = 0;
}

void Clock::processTime(){
    if (sec==60) {
        min++;
    }

    if (min == 60) {
        hour++;
    }

    if (hour==24) {
        hour = 0;
        day++;
    }

    sec++;
}