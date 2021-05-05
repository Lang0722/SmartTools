#ifndef CLOCKMODULE_H
#define CLOCKMODULE_H

#include "Joystick.h"
#include "N5110.h"
#include "mbed.h"


using namespace std;

class Clock {
public:
    Clock();
    void showTime(N5110 &lcd, DigitalIn &buttonC);
    void setTime(DigitalIn &buttonD, DigitalIn &buttonA, DigitalIn &buttonC,
               DigitalIn &buttonB, Joystick &joystick, N5110 &lcd);
    void processTime();

    int hour;
    int min;
    int sec;
    int day;
    int year;
    int month;
};

#endif