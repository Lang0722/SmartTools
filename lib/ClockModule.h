#ifndef CLOCKMODULE_H
#define CLOCKMODULE_H

#include "N5110.h"
#include "mbed.h"

using namespace std;


class Clock{
    public:
        Clock();
        void showTime(N5110 &lcd,DigitalIn &buttonC);
        void setTime();
        void processTime();
    private:
        int hour;
        int min;
        int sec;
        int day;
        int year;
        int month;
};


#endif