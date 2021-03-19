#ifndef CLOCKMODULE_H
#define CLOCKMODULE_H

#include "N5110.h"

class Clock{
    public:
        Clock();
        void showTime(N5110 &lcd);
        void setTime();
    private:
        int hour;
        int min;
        int sec;

};


#endif