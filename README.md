# DRM_Watch_v3
ESP32-S2 based watch with SHARP Memory LCD



ESP32-S2 based watch with SHARP Memory LCD





## Tools used:
KiCad 7
Arduino IDE 2.2.1
esp32 by Espressif v2.0.11, ESP32-S2 (Select "Lolin S2 Mini" board to flash successfully)
LCD Assistant



## How to add own melodies:
To make melodies I used tool by ShivamJoker which I modified to generate my melody format.
Use tool in Software\MIDI-to-Arduino-main. 
Add melody array to MelodyPlayer.ini file and add melody to melodies array.



## Known libraries conflicts:
Temperature measurement is hanging system if after Wi-Fi usage it were not initialized.






