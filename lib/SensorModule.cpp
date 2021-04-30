#include "SensorModule.h"
#include <cstdio>
#include <cstring>
#include <string>


using namespace std;

Sensor::Sensor()
{
    tempRead = 0;
    ldrRead = 0;
    standardTemp = 38.00;
    standardTemp = 41.00;
    intervalTemp = 500;
    intervalLdr = 500;
    alarmTemp = 41.00; 
    alarmflag = 0;
}

void Sensor::showTempData(N5110 &lcd, DigitalIn &buttonC, AnalogIn &tmp36, Tone &dac)
{

    float voltage; // transfer the ADC value to voltage; 3.3* tmp's value

    while (!buttonC.read())
    {
        lcd.clear();

        voltage = 3.3f * tmp36.read();
        tempRead = 100.0f * voltage - 50.0f;

        checkTemp(dac);

        char buffer[14];
        sprintf(buffer, "T=%.2f C", tempRead);

        lcd.printString("Temperature:", 0, 2);
        lcd.printString(buffer, 0, 3);
        lcd.printString("C to exit", 0, 4);

        lcd.refresh();

        thread_sleep_for(intervalTemp);
    }

    return;
}

void Sensor::showLDRData(N5110 &lcd, DigitalIn &buttonC, AnalogIn &ldr)
{

    while (!buttonC.read())
    {
        ldrRead = ldr.read();

        lcd.clear();

        char buffer[14];
        sprintf(buffer, "T=%.2f %%", ldrRead * 100);

        lcd.drawCircle(40, 18, 18, FILL_TRANSPARENT);
        int percent = int(ldrRead * 18.0f);
        lcd.drawCircle(40, 18, percent, FILL_BLACK);

        lcd.printString(buffer, 10, 5);

        lcd.refresh();

        thread_sleep_for(intervalLdr);
    }
}

void Sensor::showTempGraph(N5110 &lcd, DigitalIn &buttonC, DigitalIn &buttonB,
                           AnalogIn &tmp36)
{
    int modflag = 0;
    int pos = 0;

    thread_sleep_for(100);

    while (!buttonC.read())
    {

        for (int i = 0; i < 6; i++)
        {
            tempData[i] = (tmp36.read() * 3.3f) * 100.0f - 50.0f;
            thread_sleep_for(intervalTemp);
        }

        if (buttonB.read())
        {
            modflag++;
            modflag = modflag % 3;
        }

        lcd.clear();

        lcd.drawLine(2, 0, 2, 47, 1);
        lcd.setPixel(1, 46);
        lcd.setPixel(3, 46);
        lcd.setPixel(0, 45);
        lcd.setPixel(4, 45);

        if (modflag == 0)
        {
            lcd.drawRect(12, 0, 60, 5, FILL_TRANSPARENT);
            lcd.drawRect(12, 8, 60, 5, FILL_TRANSPARENT);
            lcd.drawRect(12, 16, 60, 5, FILL_TRANSPARENT);
            lcd.drawRect(12, 24, 60, 5, FILL_TRANSPARENT);
            lcd.drawRect(12, 32, 60, 5, FILL_TRANSPARENT);
            lcd.drawRect(12, 40, 60, 5, FILL_TRANSPARENT);

            lcd.printString("50C", 73, 0);

            int percent[6];
            for (int i = 0; i < 6; i++)
            {
                percent[i] = int(tempData[i] / 50.0f * 60.0f);
            }

            for (int i = 0; i < 6; i++)
            {
                lcd.drawRect(12, 8 * i, percent[i], 5, FILL_BLACK);
            }

            lcd.refresh();
        }
        else if (modflag == 1)
        {

            for (int i = 0; i < 6; i++)
            {
                char buffer[14];
                sprintf(buffer, "T=%.2f C", tempData[i]);
                lcd.printString(buffer, 10, i);
            }

            lcd.refresh();
        }
        else if (modflag == 2)
        {

            for (int i = 0; i < 6; i++)
            {
                //   lcd.drawRect(37, 8 * i, percent[i], 5, FILL_BLACK);
                char buffer[14];
                sprintf(buffer, "T-S=%.2f C", tempData[i] - standardTemp);
                lcd.printString(buffer, 10, i);
            }

            lcd.refresh();
        }
    }
}


void Sensor::setStandardTemp(N5110 &lcd, DigitalIn &buttonA, DigitalIn &buttonB,
                             DigitalIn &buttonC, Joystick &joystick)
{
    int posflag = 0;
    int addflag = 0;
    int pos;
    int value[4];
    Vector2D coord = joystick.get_mapped_coord();

    while (!buttonB.read())
    {
        addflag = 0;
        coord = joystick.get_mapped_coord();

        value[3] = int(standardTemp) / 10;
        value[2] = int(standardTemp) % 10;
        value[1] = int(standardTemp*10) % 10;
        value[0] = int(standardTemp*100) % 10;

        if (buttonA.read())
        {
            posflag++;
        }
        else if (buttonC.read())
        {
            posflag--;
        }
        else if (coord.y > 0.5)
        {
            addflag = 1;
        }
        else if (coord.y < -0.5)
        {
            addflag = -1;
        }

        pos = posflag % 4;

        
        lcd.clear();
        switch (pos) {
        case 0:{
            lcd.setPixel(25, 15);
            lcd.setPixel(25, 14);
            lcd.setPixel(25, 13);
            lcd.setPixel(24, 14);
            lcd.setPixel(26, 14);
            lcd.setPixel(23, 13);
            lcd.setPixel(27, 13);
            if (addflag != 0) {
                if (addflag == 1) {
                    value[3]++;
                    if (value[3] == 10) {
                        value[3] = 0;
                    }
                }else {
                    value[3]--;
                    if (value[3] == -1) {
                        value[3] = 9;
                    }
                }
            }
            break;
        }
        case 1:{
            lcd.setPixel(31, 15);
            lcd.setPixel(31, 14);
            lcd.setPixel(31, 13);
            lcd.setPixel(30, 14);
            lcd.setPixel(32, 14);
            lcd.setPixel(29, 13);
            lcd.setPixel(33, 13);
            if (addflag != 0) {
                if (addflag == 1) {
                    value[2]++;
                    if (value[2] == 10) {
                        value[2] = 0;
                    }
                }else {
                    value[2]--;
                    if (value[2] == -1) {
                        value[2] = 9;
                    }
                }
            }
            break;
        }
        case 2:{
            lcd.setPixel(43, 15);
            lcd.setPixel(43, 14);
            lcd.setPixel(43, 13);
            lcd.setPixel(42, 14);
            lcd.setPixel(44, 14);
            lcd.setPixel(41, 13);
            lcd.setPixel(45, 13);
            if (addflag != 0) {
                if (addflag == 1) {
                    value[1]++;
                    if (value[1] == 10) {
                        value[1] = 0;
                    }
                }else {
                    value[1]--;
                    if (value[1] == -1) {
                        value[1] = 9;
                    }
                }
            }
            break;
        }
        case 3:{
            lcd.setPixel(49, 15);
            lcd.setPixel(49, 14);
            lcd.setPixel(49, 13);
            lcd.setPixel(48, 14);
            lcd.setPixel(50, 14);
            lcd.setPixel(47, 13);
            lcd.setPixel(51, 13);
            if (addflag != 0) {
                if (addflag == 1) {
                    value[0]++;
                    if (value[0] == 10) {
                        value[0] = 0;
                    }
                }else {
                    value[0]--;
                    if (value[0] == -1) {
                        value[0] = 9;
                    }
                }
            }
            break;
        }

        }
        
        if (addflag != 0) {
            standardTemp = float(value[3])*10.0 + float(value[2])*1.0 + float(value[1]) * 0.1 + float(value[0]) * 0.01;
        }
        // standardTemp = value[3]*10.0 + value[2]*1.0 + value[1] * 0.1 + value[0] * 0.01;
    

        char buffer[14];
        sprintf(buffer, "T=%.2f C", standardTemp);
        lcd.printString(buffer,12,3);
        lcd.printString("B to exit", 12, 4);
        
        lcd.refresh();


        thread_sleep_for(200);


    }
}

void Sensor::showLDRGraph(N5110 &lcd, DigitalIn &buttonC, DigitalIn &buttonB, AnalogIn &ldr){
    int modflag = 0;

    thread_sleep_for(100);
    while (!buttonC.read()) {

        for (int i = 0; i < 6; i++)
        {
            ldrData[i] = ldr.read()*100;
            thread_sleep_for(intervalLdr);
        }

        if (buttonB.read())
        {
            modflag++;
            modflag = modflag % 2;
        }

        lcd.clear();

        lcd.drawLine(2, 0, 2, 47, 1);
        lcd.setPixel(1, 46);
        lcd.setPixel(3, 46);
        lcd.setPixel(0, 45);
        lcd.setPixel(4, 45);

        if (modflag == 0)
        {
            lcd.drawRect(12, 0, 60, 5, FILL_TRANSPARENT);
            lcd.drawRect(12, 8, 60, 5, FILL_TRANSPARENT);
            lcd.drawRect(12, 16, 60, 5, FILL_TRANSPARENT);
            lcd.drawRect(12, 24, 60, 5, FILL_TRANSPARENT);
            lcd.drawRect(12, 32, 60, 5, FILL_TRANSPARENT);
            lcd.drawRect(12, 40, 60, 5, FILL_TRANSPARENT);

            int percent[6];
            for (int i = 0; i < 6; i++)
            {
                percent[i] = int(ldrData[i]/100.0 * 60.0f);
            }

            for (int i = 0; i < 6; i++)
            {
                lcd.drawRect(12, 8 * i, percent[i], 5, FILL_BLACK);
            }

            lcd.refresh();
        }else if (modflag == 1) {

            for (int i = 0; i < 6; i++)
            {
                char buffer[14];
                sprintf(buffer, "light=%.2f%%", 100 - ldrData[i]);
                lcd.printString(buffer, 10, i);
            }

            lcd.refresh();
            
        }


    }
    
}

void Sensor::setInterval(N5110 &lcd,DigitalIn &buttonA, DigitalIn &buttonB, DigitalIn &buttonC, Joystick &joystick){
    int addflag = 0;
    Vector2D coord = joystick.get_mapped_coord();

    lcd.clear();
    lcd.printString("C to exit", 10, 2);
    lcd.printString("A to skip", 10, 4);
    lcd.refresh();
    thread_sleep_for(200);

    while (!buttonA.read()) {
    }

    thread_sleep_for(500);

    while (!buttonC.read()) {
        char buffer[14];
        addflag = 0;
        coord = joystick.get_mapped_coord();
        float showdata;  // for display

        if (coord.y > 0.5) {
            addflag = 1;
        }else if (coord.y < -0.5) {
            addflag = -1;
        }

        lcd.clear();
        if (modflag_interval == 0) {
            showdata = intervalTemp * 0.001f;
            sprintf(buffer, "Temp t=%.2f s", showdata);
            lcd.setPixel(44, 1);
            lcd.setPixel(44, 0);
            lcd.setPixel(43, 0);
            lcd.setPixel(45, 0);
            lcd.printString(buffer, 0, 2);

            if (addflag != 0) {
                if (addflag == 1) {
                    showdata = showdata + 0.1;
                }else {
                    showdata = showdata - 0.1;
                }

                intervalTemp = int(showdata * 1000);
            }
        }else if (modflag_interval == 1) {
            showdata = intervalLdr * 0.001f;
            sprintf(buffer, "LDR t=%.2f s", showdata);
            lcd.setPixel(38, 1);
            lcd.setPixel(38, 0);
            lcd.setPixel(37, 0);
            lcd.setPixel(39, 0);
            lcd.printString(buffer, 0, 2);

            if (addflag != 0) {
                if (addflag == 1) {
                    showdata = showdata + 0.1;
                }else {
                    showdata = showdata - 0.1;
                }

                intervalLdr = int(showdata * 1000);
            }
        }else if (modflag_interval == 2) {
            sprintf(buffer, "Alarm T=%.2fC", alarmTemp);
            lcd.printString(buffer, 0, 2);
            if (addflag != 0) {
                if (addflag == 1) {
                    alarmTemp = alarmTemp + 0.1;
                }else {
                    alarmTemp = alarmTemp - 0.1;
                }
            }
        }else if (modflag_interval == 3){
            lcd.printString("Status:", 0, 3);
            if (alarmflag == 0) {
                lcd.printString("N", 44, 3);
            }else if (alarmflag == 1) {
                lcd.printString("Y", 44, 3);
            }

            if (addflag != 0) {
                if (addflag == 1) {
                    alarmflag = 1;
                }else {
                    alarmflag = 0;
                }
            }
            
        }

        lcd.refresh();

        thread_sleep_for(200);
    
    }
}

void Sensor::checkTemp(Tone &dac){
    if (alarmflag) {
        if (alarmTemp <= tempRead) {
            dac.play(200, 1);
        }
    }
}
