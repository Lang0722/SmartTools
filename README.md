#  Release Version

"Smart Tools" is design to work as a combination of a clock, temperature sensor, light sensor and a simple smart home device. Smart Tools has three main modules. The Clock Modules, The Sensor Modules, The Network Modules. Those three modules will work together to provide a series of functions to users, such as "Calendar", "Remote Monitor", "Temperature Alarm" and so on.

## Clock Module

This module will display the default time to the LCD screen. User can change the time in the setting menu.

## Sensor Module

This module have two parts. The first one is based on the temperature sensor. It can provide the real time temperature and a graph or a list of recent temperature. It can also provide the difference between **the recent temperature** with the **standard temperature value** in a list form. User can change the interval between each detection and standard temperature value in the setting menu. The Alarm function will cause crush in this board due to the errors in the library files, but user can still set the alarm temperature and turn on the alarm function.

The second one is based on the Light Dependent Resistor (LDR). The functions of this part is basically the same as the first part but without the alarm function.

## Network Module

Because the application board does not have a WIFI module, the board will connect to a PC and let PC act as a server. Then PC will create a socket to provide the data to the connected clients in Graph form or a list form.