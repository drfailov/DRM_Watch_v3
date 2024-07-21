#ifndef modeNotepad_H
#define modeNotepad_H

/*PROTOTYPES*/
void modeNotepadLoop();
void setmodeNotepad();
void modeNotepadButtonUp();
void modeNotepadButtonCenter();
void modeNotepadButtonDown();
int timerStep();
bool timerAlert();
void resetTimer();
bool timerAlertCheckButtons();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeSerialLog.h"
#include <Arduino.h>
#include "USB.h"
#include "USBMSC.h"


USBMSC MSC;

#define FAT_U8(v) ((v) & 0xFF)
#define FAT_U16(v) FAT_U8(v), FAT_U8((v) >> 8)
#define FAT_U32(v) FAT_U8(v), FAT_U8((v) >> 8), FAT_U8((v) >> 16), FAT_U8((v) >> 24)
#define FAT_MS2B(s,ms)    FAT_U8(((((s) & 0x1) * 1000) + (ms)) / 10)
#define FAT_HMS2B(h,m,s)  FAT_U8(((s) >> 1)|(((m) & 0x7) << 5)),      FAT_U8((((m) >> 3) & 0x7)|((h) << 3))
#define FAT_YMD2B(y,m,d)  FAT_U8(((d) & 0x1F)|(((m) & 0x7) << 5)),    FAT_U8((((m) >> 3) & 0x1)|((((y) - 1980) & 0x7F) << 1))
#define FAT_TBL2B(l,h)    FAT_U8(l), FAT_U8(((l >> 8) & 0xF) | ((h << 4) & 0xF0)), FAT_U8(h >> 4)

#define README_CONTENTS "This is tinyusb's MassStorage Class demo.\r\n\r\nIf you find any bugs or get any questions, feel free to file an\r\nissue at github.com/hathach/tinyusb"
static const uint32_t DISK_SECTOR_COUNT = 2 * 500; //2*8 8KB is the smallest size that windows allow to mount
static const uint16_t DISK_SECTOR_SIZE = 512;    // Should be 512   (same as LBA)
static const uint16_t DISC_SECTORS_PER_TABLE = 1; //each table sector can fit 170KB (340 sectors)
bool showHelpText = true;
uint32_t beginning = 1000;

//https://github.com/espressif/esp-idf/blob/master/examples/storage/partition_api/partition_ops/main/main.c
// Find the partition map in the partition table
const esp_partition_t *spifsPartition;

static uint8_t msc_disk[4][DISK_SECTOR_SIZE] =
{
  //------------- Block0: Boot Sector -------------//
  {
    // Header (62 bytes)
    0xEB, 0x3C, 0x90, //jump_instruction
    'M' , 'S' , 'D' , 'O' , 'S' , '5' , '.' , '0' , //oem_name
    FAT_U16(DISK_SECTOR_SIZE), //bytes_per_sector
    FAT_U8(1),    //sectors_per_cluster
    FAT_U16(1),   //reserved_sectors_count
    FAT_U8(1),    //file_alloc_tables_num
    FAT_U16(16),  //max_root_dir_entries
    FAT_U16(DISK_SECTOR_COUNT), //fat12_sector_num
    0xF8,         //media_descriptor
    FAT_U16(DISC_SECTORS_PER_TABLE),   //sectors_per_alloc_table;//FAT12 and FAT16
    FAT_U16(1),   //sectors_per_track;//A value of 0 may indicate LBA-only access
    FAT_U16(1),   //num_heads
    FAT_U32(0),   //hidden_sectors_count
    FAT_U32(0),   //total_sectors_32
    0x00,         //physical_drive_number;0x00 for (first) removable media, 0x80 for (first) fixed disk
    0x00,         //reserved
    0x29,         //extended_boot_signature;//should be 0x29
    FAT_U32(0x1234), //serial_number: 0x1234 => 1234
    'T' , 'i' , 'n' , 'y' , 'U' , 'S' , 'B' , ' ' , 'M' , 'S' , 'C' , //volume_label padded with spaces (0x20)
    'F' , 'A' , 'T' , '1' , '2' , ' ' , ' ' , ' ' ,  //file_system_type padded with spaces (0x20)

    // Zero up to 2 last bytes of FAT magic code (448 bytes)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

     //boot signature (2 bytes)
    0x55, 0xAA
  },

  //------------- Block1: FAT12 Table -------------//
  {
    FAT_TBL2B(0xFF8, 0xFFF), FAT_TBL2B(0xFFF, 0x000) // first 2 entries must be 0xFF8 0xFFF, third entry is cluster end of readme file
  },

  //------------- Block2: Root Directory -------------//
  {
    // first entry is volume label
    'D' , 'R' , 'M' , ' ' , 'W' , 'a' , 't' , 'c' , 
    'h' , ' ' , '3' , 
    0x08, //FILE_ATTR_VOLUME_LABEL
    0x00, 
    FAT_MS2B(0,0), 
    FAT_HMS2B(0,0,0),
    FAT_YMD2B(0,0,0), 
    FAT_YMD2B(0,0,0), 
    FAT_U16(0), 
    FAT_HMS2B(13,42,30),  //last_modified_hms
    FAT_YMD2B(2018,11,5), //last_modified_ymd
    FAT_U16(0), 
    FAT_U32(0),
    
    // second entry is readme file
    'R' , 'E' , 'A' , 'D' , 'M' , 'E' , ' ' , ' ' ,//file_name[8]; padded with spaces (0x20)
    'T' , 'X' , 'T' ,     //file_extension[3]; padded with spaces (0x20)
    0x20,                 //file attributes: FILE_ATTR_ARCHIVE
    0x00,                 //ignore
    FAT_MS2B(1,980),      //creation_time_10_ms (max 199x10 = 1s 990ms)
    FAT_HMS2B(13,42,36),  //create_time_hms [5:6:5] => h:m:(s/2)
    FAT_YMD2B(2018,11,5), //create_time_ymd [7:4:5] => (y+1980):m:d
    FAT_YMD2B(2020,11,5), //last_access_ymd
    FAT_U16(0),           //extended_attributes
    FAT_HMS2B(13,44,16),  //last_modified_hms
    FAT_YMD2B(2019,11,5), //last_modified_ymd
    FAT_U16(2),           //start of file in cluster
    FAT_U32(sizeof(README_CONTENTS) - 1) //file size
  },

  //------------- Block3: Readme Content -------------//
  README_CONTENTS
};

static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize){
  showHelpText = false;
  u8g2log.printf("\nMSC WR: lba: %u, offs: %u, bufsz: %u...", lba, offset, bufsize);
  //modeNotepadLoop();
  if(spifsPartition == NULL ){
    u8g2log.printf("\nERR! NO PARTITION!");
    return 0;
  }
  esp_err_t res = esp_partition_write(spifsPartition, beginning+lba*DISK_SECTOR_SIZE+offset, buffer, bufsize); //sizeof(store_data)
  switch(res){
    case ESP_OK:
      u8g2log.printf("OK");
      break;
    case ESP_ERR_INVALID_ARG:
      u8g2log.printf("ESP_ERR_INVALID_ARG");
      break;
    case ESP_ERR_INVALID_SIZE:
      u8g2log.printf("ESP_ERR_INVALID_SIZE");
      break;
    default:
      u8g2log.printf("ERR %d", res);
      break;
  }
  //modeNotepadLoop();
  //memcpy(msc_disk[lba] + offset, buffer, bufsize);
  return bufsize;
}

static int32_t onRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize){
  showHelpText = false;
  u8g2log.printf("\nMSC RD: lba: %u, offs: %u, bufsz: %u...", lba, offset, bufsize);
  //modeNotepadLoop();
  if(spifsPartition == NULL ){
    u8g2log.printf("ERR! NO PARTITION!");
    return 0;
  }
  esp_err_t res = esp_partition_read(spifsPartition, beginning+lba*DISK_SECTOR_SIZE+offset, buffer, bufsize);
  switch(res){
    case ESP_OK:
      u8g2log.printf("OK");
      break;
    case ESP_ERR_INVALID_ARG:
      u8g2log.printf("ESP_ERR_INVALID_ARG");
      break;
    case ESP_ERR_INVALID_SIZE:
      u8g2log.printf("ESP_ERR_INVALID_SIZE");
      break;
    default:
      u8g2log.printf("ERR %d", res);
      break;
  }
  //modeNotepadLoop();
  //memcpy(buffer, msc_disk[lba] + offset, bufsize);
  return bufsize;
}

static bool onStartStop(uint8_t power_condition, bool start, bool load_eject){
  showHelpText = false;
  u8g2log.printf("\nMSC START/STOP: power: %u, start: %u, eject: %u\n", power_condition, start, load_eject);
  return true;
}


void setmodeNotepad(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Notepad"));
  modeSetup = setmodeNotepad;
  modeLoop = modeNotepadLoop;
  modeButtonUp = modeNotepadButtonUp;
  modeButtonCenter = modeNotepadButtonCenter;
  modeButtonDown = modeNotepadButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  showHelpText = true;

  u8g2log.begin(LOG_W, LOG_H, u8log_buffer);
  u8g2log.setLineHeightOffset(0);

  MSC.vendorID("DrFailov");//max 8 chars
  MSC.productID("DRM Watch 3");//max 16 chars
  MSC.productRevision("3.0");//max 4 chars
  MSC.onStartStop(onStartStop);
  MSC.onRead(onRead);
  MSC.onWrite(onWrite);
  MSC.mediaPresent(true);
  MSC.begin(DISK_SECTOR_COUNT, DISK_SECTOR_SIZE);

  spifsPartition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "spiffs");
  //esp_partition_erase_range(partition, 0, partition->size)
}

void modeNotepadLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Нотатки");

  drawStatusbar(363, 1, true);

  if(showHelpText){
    drawCentered("Підключіть до USB", 100);
    drawCentered("та закиньте текстові файли з компа", 140);
    if(spifsPartition != NULL)
      drawCentered(spifsPartition->label, 220);
    else
      drawCentered("Розділ не знайдено", 220);
  }

  //DRAW LOG
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->drawLog(2, 39, u8g2log);

  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260);  //draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  draw_ic16_back(lx(), ly2(), black);

  lcd()->sendBuffer();
}

void initPartition()
{
  showHelpText = false;
  if(spifsPartition == NULL ){
    u8g2log.printf("ERR! NO PARTITION!");
    return;
  }
  {
    u8g2log.printf("\n\nClear all partition...");
    modeNotepadLoop();
    esp_err_t res = esp_partition_erase_range(spifsPartition, 0, spifsPartition->size);
    switch(res){
      case ESP_OK:
        u8g2log.printf("OK");
        break;
      case ESP_ERR_INVALID_ARG:
        u8g2log.printf("ESP_ERR_INVALID_ARG");
        break;
      case ESP_ERR_INVALID_SIZE:
        u8g2log.printf("ESP_ERR_INVALID_SIZE");
        break;
      default:
        u8g2log.printf("ERR %d", res);
        break;
    }
    modeNotepadLoop();
  }
  {
    u8g2log.printf("\n\nInit filesystem...");
    modeNotepadLoop();
    esp_err_t res = esp_partition_write(spifsPartition, beginning, msc_disk, sizeof(msc_disk)); 
    switch(res){
      case ESP_OK:
        u8g2log.printf("OK");
        break;
      case ESP_ERR_INVALID_ARG:
        u8g2log.printf("ESP_ERR_INVALID_ARG");
        break;
      case ESP_ERR_INVALID_SIZE:
        u8g2log.printf("ESP_ERR_INVALID_SIZE");
        break;
      default:
        u8g2log.printf("ERR %d", res);
        break;
    }
    modeNotepadLoop();
    u8g2log.printf("\nWritten %d bytes", sizeof(msc_disk));    
    modeNotepadLoop();
  }
  {
    u8g2log.printf("\n\nCheck filesystem...");
    modeNotepadLoop();
    uint32_t bufsize = 32;
    uint8_t tmp[bufsize];
    for(uint32_t i=0; (i+bufsize)<sizeof(msc_disk); i+=bufsize){
      esp_err_t res = esp_partition_read(spifsPartition, beginning+i, tmp, bufsize);
      if(res == ESP_OK) {/*ok*/}
      else if(res == ESP_ERR_INVALID_ARG){ u8g2log.printf("\nESP_ERR_INVALID_ARG"); break;}
      else if(res == ESP_ERR_INVALID_SIZE){ u8g2log.printf("\nESP_ERR_INVALID_SIZE"); break;}
      else { u8g2log.printf("\nERR %d", res);  break;}
      
      if(memcmp(tmp, msc_disk+i, bufsize) != 0){
        u8g2log.printf("\nERR on: %d byte", i);
        modeNotepadLoop();
      }
    }
    u8g2log.printf("\nEND.");
    modeNotepadLoop();
  }
  
}

void modeNotepadButtonUp(){
  
}

void modeNotepadButtonDown(){
  initPartition();
}

void modeNotepadButtonCenter(){
  MSC.end();
  setModeAppsMenu(); //exit
}


#endif