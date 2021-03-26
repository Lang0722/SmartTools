#include "ClockModule.h"
#include "N5110.h"
#include "mbed.h"
#include "platform/mbed_thread.h"
#include <cstdio>

N5110 lcd(p14, p8, p9, p10, p11, p13, p21);
DigitalIn buttonA(p29);
DigitalIn buttonB(p28);
DigitalIn buttonC(p27);
Clock myClock;

void init();
void welcomePage();
void selectPage();
void clockModule();
void sensorModule();
void networkModule();

// main() runs in its own thread in the OS
int main()
{
	init();
	welcomePage();
    Loop:
	selectPage();
	thread_sleep_for(100);
	while (1)
	{

		if (buttonA.read())
		{
			clockModule();
            goto Loop;
		}
		else if (buttonB.read())
		{
			sensorModule();
            goto Loop;
		}
		else if (buttonC.read())
		{
			networkModule();
		}

		thread_sleep_for(100);
	}
}

void init()
{
	lcd.init();
	lcd.setContrast(0.5);
    myClock = Clock();
}

void welcomePage()
{
	lcd.clear();
	lcd.printString(" Smart Tools! ", 0, 1);
	lcd.printString("    Press A   ", 0, 2);
	lcd.refresh();

	while (buttonA.read() == 0)
	{
	}
}

void selectPage()
{
	lcd.clear();
	lcd.printString("A: Clock  ", 0, 1);
	lcd.printString("B: Sensor  ", 0, 3);
	lcd.printString("C: Network  ", 0, 5);
	lcd.refresh();
}

void clockModule()
{
	lcd.clear();
	lcd.printString("A: Time", 0, 1);
	lcd.printString("B: Setting", 0, 3);
	lcd.printString("C: Quit", 0, 5);
	lcd.refresh();
    thread_sleep_for(100);

	while (1)
	{
		if (buttonA.read())
		{
			myClock.showTime(lcd,buttonC);
            return;
		}
		else if (buttonB.read())
		{
			myClock.setTime();
            return;
		}
        else if (buttonC.read()){
            return;
        }
	}
}

void sensorModule()
{
	lcd.clear();
	lcd.printString("this is sensor", 0, 1);
	lcd.refresh();
	while (1)
	{
	}
}

void networkModule()
{
	lcd.clear();
	lcd.printString("this is network", 0, 1);
	lcd.refresh();
	while (1)
	{
	}
}
