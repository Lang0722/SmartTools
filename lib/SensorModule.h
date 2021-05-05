#ifndef SENSORMODULE_H
#define SENSORMODULE_H

#include "Joystick.h"
#include "N5110.h"
#include "mbed.h"
#include "Tone.h"


using namespace std;

class Sensor {
public:
  Sensor();
  void showTempData(N5110 &lcd, DigitalIn &buttonC, AnalogIn &tmp36, Tone &dac);
  void showTempGraph(N5110 &lcd, DigitalIn &buttonC, DigitalIn &buttonB,
                     AnalogIn &tmp36);
  void showLDRData(N5110 &lcd, DigitalIn &buttonC, AnalogIn &ldr);
  void showLDRGraph(N5110 &lcd, DigitalIn &buttonC, DigitalIn &buttonB,
                     AnalogIn &ldr);
  void setStandardTemp(N5110 &lcd, DigitalIn &buttonA, DigitalIn &buttonB,
                       DigitalIn &buttonC, Joystick &joystick);
  void setInterval(N5110 &lcd, DigitalIn &buttonA, DigitalIn &buttonB, DigitalIn &buttonC, Joystick &joystick);
  void checkTemp(Tone &dac);
  int reportTemp(AnalogIn &tmp36);
  int reportLdr(AnalogIn &ldr);

  int modflag_interval;
  int alarmflag;
  

private:
  float tempRead;
  float ldrRead;

  float tempData[6] = {0};
  float ldrData[6] = {0};

  float standardTemp;
  float alarmTemp;
  int intervalTemp;
  int intervalLdr;

  
};

#endif