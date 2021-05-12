#include "ClockModule.h"
#include <cstdio>
#include <cstring>
#include <string>

using namespace std;


Clock::Clock(){
    hour = 10;
    min = 0;
    sec = 0;
    year = 2021;
    day = 1;
    month = 5;
}

void Clock::showTime(N5110 &lcd,DigitalIn &buttonC){

    string temp_hour;
    string temp_min;
    string temp_sec;
    string temp_date;
    
    while (1) {

        lcd.clear();   // format the data
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

        if (buttonC.read()) {
            return;
        }
        
    }
}

void Clock::setTime(DigitalIn &buttonD, DigitalIn &buttonA, DigitalIn &buttonC, DigitalIn &buttonB,Joystick &joystick, N5110 &lcd){
    int posflag = 0;
    int addflag = 0;
    int temp_x = 0;
    int pos;
    int showflag = 0; // 0 for time, 1 for date
    

    string temp_hour;
    string temp_min;
    string temp_sec;
    string temp_date;  

    Vector2D coord = joystick.get_mapped_coord();

    // for instruction
    while (!buttonA.read()) {
        lcd.clear();
        lcd.printString("Tips!", 30, 1);
        lcd.printString("A move left", 0, 2);
        lcd.printString("C move right", 0, 3);
        lcd.printString("Use Joy stick", 0, 4);
        lcd.printString("To change", 0, 5);
        lcd.refresh();
    }

    while (!buttonD.read()) {
        addflag = 0;
        lcd.clear();
        coord = joystick.get_mapped_coord();

        // check the button and joystick's movement
        if (buttonA.read()) {
            posflag++;
        }else if (buttonC.read()) {
            posflag--;
        }else if (coord.y > 0.5) {
            addflag = 1 ;
        }else if(coord.y < -0.5){
            addflag = -1;
        }else if (buttonB.read()) {
            if (showflag) {
                showflag = 0;
            }else {
                showflag = 1;
            }
        }

        //format the data
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

        // show the data
        if (!showflag) {
            lcd.printString(temp_hour.c_str(), 10, 2);
        }else {
            lcd.printString(temp_date.c_str(), 10, 2);
        }

        lcd.printString("D to exit", 0, 4);
        lcd.printString("B next page", 0, 5);
        
        // set the indication flag
        pos = posflag % 3;

        // set the pos sign and change the value
        switch (pos) {
            case 0 :{
                lcd.setPixel(10, 1);
                lcd.setPixel(10, 0);
                lcd.setPixel(11, 0);
                lcd.setPixel(9, 0);
                if (addflag!=0 && !showflag) {
                    if (addflag == 1) {
                        hour++;
                        if (hour == 24) {
                            hour = 0;
                        }
                    }else {
                        hour--;
                        if (hour == -1) {
                            hour = 23;
                        }
                    }
                }else if (addflag!=0 && showflag) {
                    if (addflag == 1) {
                        day++;
                        if (day == 32) {
                            day = 0;
                        }
                    }else {
                        day--;
                        if (hour == -1) {
                            hour = 31;
                        }
                    }
                }
                break;
            }
            case 1:{
                lcd.setPixel(30, 1);
                lcd.setPixel(30, 0);
                lcd.setPixel(31, 0);
                lcd.setPixel(29, 0);
                if (addflag!=0 && !showflag) {
                    if (addflag == 1) {
                        min++;
                        if (min == 60) {
                            min = 0;
                        }
                    }else {
                        min--;
                        if (min == -1) {
                            min = 59;
                        }
                    }
                }else if (addflag!=0 && showflag) {
                    if (addflag == 1) {
                        month++;
                        if (month == 13) {
                            month = 0;
                        }
                    }else {
                        month--;
                        if (month == -1) {
                            month = 12;
                        }
                    }
                }
                break;
            }
            case 2:{
                lcd.setPixel(45, 1);
                lcd.setPixel(45, 0);
                lcd.setPixel(44, 0);
                lcd.setPixel(46, 0);
                if (addflag!=0 && !showflag) {
                    if (addflag == 1) {
                        sec++;
                        if (sec == 60) {
                            sec = 0;
                        }
                    }else {
                        sec--;
                        if (sec == -1) {
                            sec = 59;
                        }
                    }
                }else if (addflag!=0 && showflag) {
                    if (addflag == 1) {
                        year++;
                    }else {
                        year--;
                    }
                }
                break;
            }
        }
        lcd.refresh();

        thread_sleep_for(200);

    }

    lcd.refresh();
}

void Clock::processTime(){
    sec++;

    if (sec==60) {
        min++;
        sec = 0;
    }

    if (min == 60) {
        hour++;
        min = 0;
    }

    if (hour==24) {
        hour = 0;
        day++;
    }
}
