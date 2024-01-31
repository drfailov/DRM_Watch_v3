
<!-- For new line: \ -->
<!-- Picture: <img src="Photos/" width="300"/>  -->

# About project DRM Watch v3
ESP32-S2 based watch with SHARP Memory LCD


<p align="center"><img src="Photos/DRM_zb2.png" width="300"/></p>


ESP32-S2 based watch with SHARP Memory LCD.

## Features
- Meow function, can be accessed from menu;
- Stopwatch, can be accessed from menu;

- Low power consumption, while screen is always showing time;
- Charging over USB, battery status indication;
- Easy to get components needed to assemble;
- Case fits to default 20mm watch bands;
- No need to disassemble device for firmwate update;
- Device easily can be used as ESP32 playground: battery powered, equipped with 3 buttons, display, buzzer, 2 lights;
- Water-resistance up to 0 meters :)
- 3-button controlled graphic menu allow access to all functions;
- Monophonic melodies, can be played from menu;
- Stopwatch, can be accessed from menu;
- Saved Wi-Fi networks list;
- Standalone Wi-Fi time update;
- If needed, you can reboot watch from settings or press hidden reset button;
- Screen "About" shows credits and firmware version.


## Modules used as components source
<p align="center">
<img src="Photos/5a768_electronic_circuits_gI_0_MemoryLCDPress.jpg" width="200"/>
<img src="Photos/1333_1.png" width="200"/>
<img src="Photos/large_AOC777-1.jpg" width="200"/>
</p>




## Tools used:
- KiCad 7
- Arduino IDE 2.2.1
- esp32 by Espressif v2.0.11, ESP32-S2 (Select "Lolin S2 Mini" board to flash successfully)
- LCD Assistant



## How to add own melodies:
To make melodies I used tool by ShivamJoker which I modified to generate my melody format.
Use tool in Software\MIDI-to-Arduino-main. 
Add melody array to MelodyPlayer.ini file and add melody to melodies array.



## Known libraries conflicts:
Temperature measurement is hanging system if after Wi-Fi usage it were not initialized.






