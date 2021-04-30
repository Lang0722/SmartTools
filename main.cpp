#include "ClockModule.h"
#include "Joystick.h"
#include "N5110.h"
#include "SensorModule.h"
#include "mbed.h"
#include "platform/mbed_thread.h"
#include <cstdio>

N5110 lcd(p14, p8, p9, p10, p11, p13, p21);
AnalogIn tmp36(p16);
AnalogIn ldr(p15);
Joystick joystick(p20, p19);
Tone dac(p18);
DigitalIn buttonA(p29);
DigitalIn buttonB(p28);
DigitalIn buttonC(p27);
DigitalIn buttonD(p26);
Clock myClock;
Sensor mySensor;

// for the clock
Ticker ticker;

void init();
void welcomePage();
void selectPage();
void clockModule();
void sensorModule();
void networkModule();
void timer_isr();

// main() runs in its own thread in the OS
int main() {
    init();
    welcomePage();
    
Loop:
    selectPage();
    thread_sleep_for(500);
    while (1) {

        if (buttonA.read()) {
            clockModule();
            goto Loop;
        } else if (buttonB.read()) {
            sensorModule();
            goto Loop;
        } else if (buttonC.read()) {
            networkModule();
        }

        thread_sleep_for(50);
    }
}

void init() {
    lcd.init();
    dac.init();
    lcd.setContrast(0.5);
    joystick.init();
    myClock = Clock();
    mySensor = Sensor();
}

void welcomePage() {
    lcd.clear();
    lcd.printString(" Smart Tools! ", 0, 1);
    lcd.printString("    Press A   ", 0, 2);
    lcd.refresh();

    while (buttonA.read() == 0) {
    }
}

void selectPage() {
    lcd.clear();
    lcd.printString("A: Clock  ", 0, 1);
    lcd.printString("B: Sensor  ", 0, 3);
    lcd.printString("C: Network  ", 0, 5);
    lcd.refresh();
}

void clockModule() {
    lcd.clear();
    lcd.printString("A: Time", 0, 1);
    lcd.printString("B: Setting", 0, 3);
    lcd.printString("C: Quit", 0, 5);
    lcd.refresh();
    thread_sleep_for(1000);

    while (1) {
        if (buttonA.read()) {
            ticker.attach(&timer_isr, 1);
            myClock.showTime(lcd, buttonC);
            return;
        } else if (buttonB.read()) {
            ticker.detach(); // detach the timer_isr() for setting time
            myClock.setTime(buttonD, buttonA, buttonC, buttonB, joystick, lcd);
            ticker.attach(&timer_isr, 1000); // resume the ticking
            return;
        } else if (buttonC.read()) {
            return;
        }
    }
}

void sensorModule() {
    lcd.clear();
    lcd.printString("A: Temperature", 0, 1);
    lcd.printString("B: Light", 0, 3);
    lcd.printString("C: Quit", 0, 5);
    lcd.refresh();
    thread_sleep_for(1000);

    Vector2D coord = joystick.get_mapped_coord(); // for change pages

    while (1) {
        if (buttonA.read()) {
            thread_sleep_for(300);

            lcd.clear();
            lcd.printString("A: Data", 0, 0);
            lcd.printString("B: Graph", 0, 1);
            lcd.printString("C: Setting", 0, 2);
            lcd.printString("D: Quit", 0, 4);
            lcd.refresh();

            while (1) {
                if (buttonA.read()) {
                    mySensor.showTempData(lcd, buttonC, tmp36, dac);
                    return;
                } else if (buttonB.read()) {
                    mySensor.showTempGraph(lcd, buttonC, buttonB, tmp36);
                    return;
                } else if (buttonC.read()) {

                    thread_sleep_for(300);
                    Page1:
                    lcd.clear();
                    lcd.printString("A: Standard T", 0, 2);
                    lcd.printString("B: Interval", 0, 3);
                    lcd.printString("C: Alarm T", 0, 4);
                    lcd.printString("D: Quit", 0, 5);
                    lcd.refresh();
                    thread_sleep_for(100);

                    while (1) {
                        coord = joystick.get_mapped_coord();

                        if (buttonA.read()) {
                            mySensor.setStandardTemp(lcd, buttonA, buttonB, buttonC, joystick);
                            return;
                        }else if (buttonB.read()) {
                            mySensor.modflag_interval = 0;
                            mySensor.setInterval(lcd, buttonA, buttonB, buttonC, joystick);
                            return;
                        }else if (buttonC.read()) {
                            mySensor.modflag_interval = 2;
                            mySensor.setInterval(lcd, buttonA, buttonB, buttonC, joystick);
                            return;
                        }else if (buttonD.read()){
                            return;
                        }else if (coord.y < -0.5) {
                            goto Page2;
                        }
                    }

                    Page2: 
                    lcd.clear();
                    lcd.printString("A: Alarm", 0, 2);
                    lcd.printString("B: Quit", 0, 3);
                    lcd.refresh();
                    thread_sleep_for(200);

                    while (1) {
                        coord = joystick.get_mapped_coord();

                        if (buttonA.read()) {
                            mySensor.modflag_interval = 3;
                            mySensor.setInterval(lcd, buttonA, buttonB, buttonC, joystick);
                            return;
                        }else if (buttonB.read()) {
                            return;
                        }else if (coord.y > 0.5) {
                            goto Page1;
                        }

                    }

                    return;
                } else if (buttonD.read()) {
                    break;
                }
            }

            return;
        } else if (buttonB.read()) {
            thread_sleep_for(500);

            lcd.clear();
            lcd.printString("A: Data", 0, 0);
            lcd.printString("B: Graph", 0, 1);
            lcd.printString("C: Setting", 0, 2);
            lcd.printString("D: Quit", 0, 4);
            lcd.refresh();

            while (1) {
                if (buttonA.read()) {
                    mySensor.showLDRData(lcd, buttonC, ldr);
                    return;
                } else if (buttonB.read()) {
                    mySensor.showLDRGraph(lcd, buttonC, buttonB, ldr);
                    return;
                } else if (buttonC.read()) {
                    mySensor.modflag_interval = 1;
                    mySensor.setInterval(lcd, buttonA, buttonB, buttonC, joystick);
                    return;
                } else if (buttonD.read()) {
                    break;
                }
            }

            return;
        } else if (buttonC.read()) {
            return;
        }
    }
}

void networkModule() {
    lcd.clear();
    lcd.printString("this is network", 0, 1);
    lcd.refresh();
    while (1) {
    }
}

void timer_isr() { 
    myClock.processTime(); 
}

