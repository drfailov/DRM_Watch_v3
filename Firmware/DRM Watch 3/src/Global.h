/*
CTRL+K CTRL+M  - fold
CTRL+K CTRL+J  - unfold
*/

#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>

struct Point{
    public:
      int x;
      int y;
};

/*Do not use pins!
 D17 - Stays ON when firmware update
 D31 - Used for flash communication
 D32 - Used for flash communication
*/
//#define HW_REV_1
//#define HW_REV_2
//#define HW_REV_3
#define HW_REV_4

#ifdef HW_REV_1
#define LCD_CLK 7
#define LCD_SDA 11
#define LCD_CS 5
#define LCD_EN GPIO_NUM_6
#define BUT_UP GPIO_NUM_0 //Active LOW
#define BUT_CENTER GPIO_NUM_1 //Active LOW
#define BUT_DOWN GPIO_NUM_2 //Active LOW
#define SENS_USB_PIN 3
#define SENS_BATERY_PIN 4
#define BUZZER_PIN 12
#define LED_TOP_PIN GPIO_NUM_33      //OLD
#define LED_BOTTOM_PIN GPIO_NUM_34   //OLD
#define LED_STATUS_PIN 35            //OLD
#define TOUCH1_PIN 8
#define TOUCH2_PIN 9
#define TOUCH3_PIN 10
#define CHARGER_EN_PIN 37  //OLD
#define BACKLIGHT_EN GPIO_NUM_8  //NOT USED
#endif

#ifdef HW_REV_2
#define LCD_CLK 7
#define LCD_SDA 11
#define LCD_CS 5
#define LCD_EN GPIO_NUM_6
#define BUT_UP GPIO_NUM_0 //Active LOW
#define BUT_CENTER GPIO_NUM_1 //Active LOW
#define BUT_DOWN GPIO_NUM_2 //Active LOW
#define SENS_USB_PIN 3
#define SENS_BATERY_PIN 4
#define BUZZER_PIN 12
#define LED_TOP_PIN GPIO_NUM_36     
#define LED_BOTTOM_PIN GPIO_NUM_37  
#define LED_STATUS_PIN GPIO_NUM_38  
#define TOUCH1_PIN 8
#define TOUCH2_PIN 9
#define TOUCH3_PIN 10
#define RTC_SDA_PIN 33    
#define RTC_SCL_PIN 35    
#endif

#ifdef HW_REV_3
#define LCD_CLK 7
#define LCD_SDA 11
#define LCD_CS 5
#define LCD_EN GPIO_NUM_6
#define BACKLIGHT_EN GPIO_NUM_8
#define BUT_UP GPIO_NUM_0 //Active LOW
#define BUT_CENTER GPIO_NUM_1 //Active LOW
#define BUT_DOWN GPIO_NUM_2 //Active LOW
#define SENS_USB_PIN 3
#define SENS_BATERY_PIN 4
#define BUZZER_PIN 12
#define LED_TOP_PIN GPIO_NUM_36     
#define LED_BOTTOM_PIN GPIO_NUM_37  
#define LED_STATUS_PIN GPIO_NUM_38  
#define RTC_SDA_PIN 33    
#define RTC_SCL_PIN 35    
#endif

#ifdef HW_REV_4
#define LCD_CLK 7
#define LCD_SDA 11
#define LCD_CS 5
#define LCD_EN GPIO_NUM_6
#define BACKLIGHT_EN GPIO_NUM_8
#define BUT_UP GPIO_NUM_0 //Active LOW
#define BUT_CENTER GPIO_NUM_1 //Active LOW
#define BUT_DOWN GPIO_NUM_2 //Active LOW
#define SENS_USB_PIN 3
#define SENS_BATERY_PIN 4
#define BUZZER_PIN 12
#define LED_TOP_PIN GPIO_NUM_36     
#define LED_BOTTOM_PIN GPIO_NUM_37  
#define LED_STATUS_PIN GPIO_NUM_38  
#define RTC_SDA_PIN 33    
#define RTC_SCL_PIN 35    
#endif

#define W 400
#define H 240
const int BUFF_SCALE = 2;  //screenBuffer  //2 is 3KB out of 8KB RTC Memory
const int BUFF_W = W/BUFF_SCALE;   //screenBuffer
const int BUFF_H = H/BUFF_SCALE;   //screenBuffer
  
String version = "FW:v3.15";          //================================== <<<<< VERSION
bool black = 1;
bool white = 0;

bool firstDraw = false;  //global for menus, used to animate first frame of any mode. Resets on clearScreenAnimation.
int items = 0;    //global for menus
int selected = 0; //global for menus
int roundness = 4; //global for menus
int frame = 2; //global for menus

typedef void (*Runnable)();
typedef void (*WatchfaceDrawable)(bool firstDraw);
typedef void (*Drawable)(int x,int y, bool color);
typedef const char* (*Textable)(int index); //replacing arrays
RTC_DATA_ATTR Runnable modeSetup;
Runnable modeLoop = 0;
Runnable modeExit = 0;  //end processes. Set it to 0 every time this function is called. Call modeExit every time when any new mode is set.
Runnable modeButtonUp = 0;
Runnable modeButtonCenter = 0;
Runnable modeButtonDown = 0;
Runnable modeButtonUpLong = 0;
Runnable modeButtonCenterLong = 0;
Runnable modeButtonDownLong = 0;
#define BUFFER_SIZE 40  //размер текстового буфера. Чем меньше тем экономнее.
char buffer[BUFFER_SIZE];   //общий на всю программу текстовый буфер чтобы не объявлять каждый раз локальную.

void doNothing(){} //to use as reference in modes for buttons
bool strendswith(const char* str, const char* suffix)
{
    int len = strlen(str);
    int suffixlen = strlen(suffix);
    if(suffixlen > len)
        return false;
    str += (len - suffixlen);
    return strcmp(str, suffix) == 0;
}

const char* getFileNameFromPath(const char* _buffer)
{
    char c;
    int  i;
    for (i = 0; ;++i) {
        c = *((char*)_buffer+i);
        if (c == '\\' || c == '/')
            return getFileNameFromPath((char*)_buffer + i + 1);
        if (c == '\0')
            return _buffer;
    }
    return "";
}

#endif