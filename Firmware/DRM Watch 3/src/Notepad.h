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
#define FAT_MS2B(s, ms) FAT_U8(((((s) & 0x1) * 1000) + (ms)) / 10)
#define FAT_HMS2B(h, m, s) FAT_U8(((s) >> 1) | (((m) & 0x7) << 5)), FAT_U8((((m) >> 3) & 0x7) | ((h) << 3))
#define FAT_YMD2B(y, m, d) FAT_U8(((d) & 0x1F) | (((m) & 0x7) << 5)), FAT_U8((((m) >> 3) & 0x1) | ((((y) - 1980) & 0x7F) << 1))
#define FAT_TBL2B(l, h) FAT_U8(l), FAT_U8(((l >> 8) & 0xF) | ((h << 4) & 0xF0)), FAT_U8(h >> 4)

#define README_CONTENTS "This is tinyusb's MassStorage Class demo.\r\n\r\nIf you find any bugs or get any questions, feel free to file an\r\nissue at github.com/hathach/tinyusb"
static const uint32_t DISK_SECTOR_COUNT = 2 * 500; // 2*8 8KB is the smallest size that windows allow to mount
static const uint16_t DISK_SECTOR_SIZE = 512;      // Should be 512   (same as LBA)
static const uint16_t DISC_SECTORS_PER_TABLE = 1;  // each table sector can fit 170KB (340 sectors)
bool showHelpText = true;

// https://github.com/espressif/esp-idf/blob/master/examples/storage/partition_api/partition_ops/main/main.c
//  Find the partition map in the partition table
const esp_partition_t *spifsPartition;
uint8_t pageBuffer[SPI_FLASH_SEC_SIZE]; // flsah can be written only by this blocks
esp_err_t res;                          // used many times where patition operations occur

static uint8_t msc_disk[4][DISK_SECTOR_SIZE] =
    {
        //------------- Block0: Boot Sector -------------//
        {
            // Header (62 bytes)
            0xEB, 0x3C, 0x90,                                      // jump_instruction
            'M', 'S', 'D', 'O', 'S', '5', '.', '0',                // oem_name
            FAT_U16(DISK_SECTOR_SIZE),                             // bytes_per_sector
            FAT_U8(1),                                             // sectors_per_cluster
            FAT_U16(1),                                            // reserved_sectors_count
            FAT_U8(1),                                             // file_alloc_tables_num
            FAT_U16(16),                                           // max_root_dir_entries
            FAT_U16(DISK_SECTOR_COUNT),                            // fat12_sector_num
            0xF8,                                                  // media_descriptor
            FAT_U16(DISC_SECTORS_PER_TABLE),                       // sectors_per_alloc_table;//FAT12 and FAT16
            FAT_U16(1),                                            // sectors_per_track;//A value of 0 may indicate LBA-only access
            FAT_U16(1),                                            // num_heads
            FAT_U32(0),                                            // hidden_sectors_count
            FAT_U32(0),                                            // total_sectors_32
            0x00,                                                  // physical_drive_number;0x00 for (first) removable media, 0x80 for (first) fixed disk
            0x00,                                                  // reserved
            0x29,                                                  // extended_boot_signature;//should be 0x29
            FAT_U32(0x1234),                                       // serial_number: 0x1234 => 1234
            'T', 'i', 'n', 'y', 'U', 'S', 'B', ' ', 'M', 'S', 'C', // volume_label padded with spaces (0x20)
            'F', 'A', 'T', '1', '2', ' ', ' ', ' ',                // file_system_type padded with spaces (0x20)

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

            // boot signature (2 bytes)
            0x55, 0xAA},

        //------------- Block1: FAT12 Table -------------//
        {
            FAT_TBL2B(0xFF8, 0xFFF), FAT_TBL2B(0xFFF, 0x000) // first 2 entries must be 0xFF8 0xFFF, third entry is cluster end of readme file
        },

        //------------- Block2: Root Directory -------------//
        {
            // first entry is volume label
            'D', 'R', 'M', ' ', 'W', 'a', 't', 'c',
            'h', ' ', '3',
            0x08, // FILE_ATTR_VOLUME_LABEL
            0x00,
            FAT_MS2B(0, 0),
            FAT_HMS2B(0, 0, 0),
            FAT_YMD2B(0, 0, 0),
            FAT_YMD2B(0, 0, 0),
            FAT_U16(0),
            FAT_HMS2B(13, 42, 30),  // last_modified_hms
            FAT_YMD2B(2018, 11, 5), // last_modified_ymd
            FAT_U16(0),
            FAT_U32(0),

            // second entry is readme file
            'R', 'E', 'A', 'D', 'M', 'E', ' ', ' ', // file_name[8]; padded with spaces (0x20)
            'T', 'X', 'T',                          // file_extension[3]; padded with spaces (0x20)
            0x20,                                   // file attributes: FILE_ATTR_ARCHIVE
            0x00,                                   // ignore
            FAT_MS2B(1, 980),                       // creation_time_10_ms (max 199x10 = 1s 990ms)
            FAT_HMS2B(13, 42, 36),                  // create_time_hms [5:6:5] => h:m:(s/2)
            FAT_YMD2B(2018, 11, 5),                 // create_time_ymd [7:4:5] => (y+1980):m:d
            FAT_YMD2B(2020, 11, 5),                 // last_access_ymd
            FAT_U16(0),                             // extended_attributes
            FAT_HMS2B(13, 44, 16),                  // last_modified_hms
            FAT_YMD2B(2019, 11, 5),                 // last_modified_ymd
            FAT_U16(2),                             // start of file in cluster
            FAT_U32(sizeof(README_CONTENTS) - 1)    // file size
        },

        //------------- Block3: Readme Content -------------//
        README_CONTENTS};

void writeFlash(uint32_t offset, uint8_t *buffer, uint32_t bufsize)
{
  int32_t pageOffset = offset % SPI_FLASH_SEC_SIZE;
  u8g2log.printf("  pageOffset: %d\n", pageOffset);
  //modeNotepadLoop();
  int32_t pageStart = offset - pageOffset;
  u8g2log.printf("  pageStart: %d\n", pageStart);
  //modeNotepadLoop();
  int32_t endOfBuf = offset + bufsize;
  u8g2log.printf("  endOfBuf: %d\n", endOfBuf);
  //modeNotepadLoop();
  int32_t endOfPage = pageStart + SPI_FLASH_SEC_SIZE;
  u8g2log.printf("  endOfPage: %d\n", endOfPage);
  //modeNotepadLoop();
  int32_t bufSz = min(endOfBuf, endOfPage) - offset;
  u8g2log.printf("  bufSz: %d\n", bufSz);
  //modeNotepadLoop();

  u8g2log.printf("  Read: %d-%d...", pageStart, endOfPage);
  res = esp_partition_read(spifsPartition, pageStart, pageBuffer, SPI_FLASH_SEC_SIZE);
  if (res == ESP_OK)
    u8g2log.printf("OK\n");
  else if (res == ESP_ERR_INVALID_ARG)
    u8g2log.printf("ESP_ERR_INVALID_ARG\n");
  else if (res == ESP_ERR_INVALID_SIZE)
    u8g2log.printf("ESP_ERR_INVALID_SIZE\n");
  else if (res == ESP_OK)
    u8g2log.printf("ERR %d\n", res);
  //modeNotepadLoop();

  u8g2log.printf("  Modify : %d-%d...", offset, offset + bufSz);
  memcpy(/*dst*/ pageBuffer, /*src*/ buffer, /*len*/ bufSz);
  u8g2log.printf("OK\n");
  //modeNotepadLoop();

  u8g2log.printf("  Erase: %d-%d...", pageStart, endOfPage);
  res = esp_partition_erase_range(spifsPartition, pageStart, SPI_FLASH_SEC_SIZE);
  if (res == ESP_OK)
    u8g2log.printf("OK\n");
  else if (res == ESP_ERR_INVALID_ARG)
    u8g2log.printf("ESP_ERR_INVALID_ARG\n");
  else if (res == ESP_ERR_INVALID_SIZE)
    u8g2log.printf("ESP_ERR_INVALID_SIZE\n");
  else if (res == ESP_OK)
    u8g2log.printf("ERR %d\n", res);
  //modeNotepadLoop();

  u8g2log.printf("  Write: %d-%d...", pageStart, endOfPage);
  res = esp_partition_write(spifsPartition, pageStart, pageBuffer, SPI_FLASH_SEC_SIZE); // sizeof(store_data)
  if (res == ESP_OK)
    u8g2log.printf("OK\n");
  else if (res == ESP_ERR_INVALID_ARG)
    u8g2log.printf("ESP_ERR_INVALID_ARG\n");
  else if (res == ESP_ERR_INVALID_SIZE)
    u8g2log.printf("ESP_ERR_INVALID_SIZE\n");
  else if (res == ESP_OK)
    u8g2log.printf("ERR %d\n", res);
  //modeNotepadLoop();

  if (endOfPage < endOfBuf) // write next page
    writeFlash(/*offset*/ offset + bufSz, /*buffer*/ buffer + bufSz, /*bufsize*/ bufsize - bufSz);
}

static int32_t onWrite(uint32_t lba, uint32_t lba_offset, uint8_t *buffer, uint32_t bufsize)
{
  showHelpText = false;
  u8g2log.printf("MSC WRT lba: %u, offs: %u, bufsz: %u:\n", lba, lba_offset, bufsize);
  //modeNotepadLoop();
  if (spifsPartition == NULL)
  {
    u8g2log.printf("\nERR! NO PARTITION!");
    return 0;
  }
  int32_t offset = lba * DISK_SECTOR_SIZE + lba_offset;
  u8g2log.printf("  offset: %d\n", offset);
  //modeNotepadLoop();
  writeFlash(offset, buffer, bufsize);
  return bufsize;
}

static int32_t onRead(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize)
{
  showHelpText = false;
  u8g2log.printf("MSC RD: lba: %u, offs: %u, bufsz: %u...", lba, offset, bufsize);
  if (spifsPartition == NULL)
  {
    u8g2log.printf("ERR! NO PARTITION!");
    modeNotepadLoop();
    return 0;
  }
  res = esp_partition_read(spifsPartition, lba * DISK_SECTOR_SIZE + offset, buffer, bufsize);
  if (res == ESP_OK)
    u8g2log.printf("OK\n");
  else if (res == ESP_ERR_INVALID_ARG)
    u8g2log.printf("ESP_ERR_INVALID_ARG\n");
  else if (res == ESP_ERR_INVALID_SIZE)
    u8g2log.printf("ESP_ERR_INVALID_SIZE\n");
  else if (res == ESP_OK)
    u8g2log.printf("ERR %d\n", res);
  // modeNotepadLoop();
  return bufsize;
}

static bool onStartStop(uint8_t power_condition, bool start, bool load_eject)
{
  showHelpText = false;
  u8g2log.printf("\nMSC START/STOP: power: %u, start: %u, eject: %u\n", power_condition, start, load_eject);
  return true;
}

void setmodeNotepad()
{
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

  MSC.vendorID("DrFailov");     // max 8 chars
  MSC.productID("DRM Watch 3"); // max 16 chars
  MSC.productRevision("3.0");   // max 4 chars
  MSC.onStartStop(onStartStop);
  MSC.onRead(onRead);
  MSC.onWrite(onWrite);
  MSC.mediaPresent(true);
  MSC.begin(DISK_SECTOR_COUNT, DISK_SECTOR_SIZE);

  spifsPartition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "spiffs");
  // spifsPartition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "app1");
}

void modeNotepadLoop()
{
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  lcd()->print("Завантажити файли");

  drawStatusbar(363, 1, true);

  if (showHelpText)
  {
    drawCentered("Підключіть до USB", 100);
    drawCentered("та закиньте текстові файли з компа", 140);
    if (spifsPartition != NULL)
    {
      drawCentered(spifsPartition->label, 200);
      itoa(spifsPartition->size, buffer, 10);
      drawCentered(buffer, 220);
      itoa(SPI_FLASH_SEC_SIZE, buffer, 10);
      drawCentered(buffer, 240);
    }
    else
      drawCentered("Розділ не знайдено", 220);
  }

  // DRAW LOG
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); // smalll
  lcd()->drawLog(2, 39, u8g2log);

  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260); // draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  draw_ic16_hashtag(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_warning(lx(), ly3(), black);

  lcd()->sendBuffer();
}

void showPartitions()
{
  showHelpText = false;
  u8g2log.printf("\n======== ESP32 Partition table: =======\n\n");
  modeNotepadLoop();

  u8g2log.printf("|Type|Sub| Offset |  Size  |  Label   |\n");
  modeNotepadLoop();
  u8g2log.printf("|----|---|--------|--------|----------|\n");
  modeNotepadLoop();

  esp_partition_iterator_t pi = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (pi != NULL)
  {
    do
    {
      const esp_partition_t *p = esp_partition_get(pi);
      u8g2log.printf("| %02x |%02x |0x%06X|0x%06X|%-10s|\r\n",
                     p->type, p->subtype, p->address, p->size, p->label);
      modeNotepadLoop();
    } while (pi = (esp_partition_next(pi)));
  }
  u8g2log.printf("|----|---|--------|--------|----------|\n");
  modeNotepadLoop();
}
void initPartition()
{
  showHelpText = false;
  if (spifsPartition == NULL)
  {
    u8g2log.printf("ERR! NO PARTITION!");
    modeNotepadLoop();
    return;
  }
  u8g2log.printf("Start\n");
  modeNotepadLoop();

  uint32_t imageSize = sizeof(msc_disk);
  u8g2log.printf("imageSize: %d\n", imageSize);
  modeNotepadLoop();

  u8g2log.printf("Clear buffer %d...", SPI_FLASH_SEC_SIZE);
  modeNotepadLoop();
  for (int i = 0; i < SPI_FLASH_SEC_SIZE; i++)
    pageBuffer[i] = 0;
  u8g2log.printf("OK\n");
  modeNotepadLoop();

  for(int offset = 0; (offset+SPI_FLASH_SEC_SIZE)<spifsPartition->size; offset += SPI_FLASH_SEC_SIZE){
    u8g2log.printf("Clear page %d...", offset);
    modeNotepadLoop();
    res = esp_partition_erase_range(spifsPartition, offset, SPI_FLASH_SEC_SIZE);
    if (res == ESP_OK)
      u8g2log.printf("OK,");
    else if (res == ESP_ERR_INVALID_ARG)
      u8g2log.printf("ESP_ERR_INVALID_ARG,");
    else if (res == ESP_ERR_INVALID_SIZE)
      u8g2log.printf("ESP_ERR_INVALID_SIZE,");
    else if (res == ESP_OK)
      u8g2log.printf("ERR %d,", res);
    modeNotepadLoop();

    res = esp_partition_write(spifsPartition, offset, pageBuffer, SPI_FLASH_SEC_SIZE);
    if (res == ESP_OK)
      u8g2log.printf("OK\n");
    else if (res == ESP_ERR_INVALID_ARG)
      u8g2log.printf("ESP_ERR_INVALID_ARG\n");
    else if (res == ESP_ERR_INVALID_SIZE)
      u8g2log.printf("ESP_ERR_INVALID_SIZE\n");
    else if (res == ESP_OK)
      u8g2log.printf("ERR %d\n", res);
    modeNotepadLoop();
  }
  

  u8g2log.printf("Prepare filesystem %d...", imageSize);
  modeNotepadLoop();
  memcpy(pageBuffer, msc_disk, imageSize);
  u8g2log.printf("OK\n");
  modeNotepadLoop();

  
  u8g2log.printf("Clear region %d-%d...", 0, SPI_FLASH_SEC_SIZE);
  modeNotepadLoop();
  res = esp_partition_erase_range(spifsPartition, 0, SPI_FLASH_SEC_SIZE);
  if (res == ESP_OK)
    u8g2log.printf("OK\n");
  else if (res == ESP_ERR_INVALID_ARG)
    u8g2log.printf("ESP_ERR_INVALID_ARG\n");
  else if (res == ESP_ERR_INVALID_SIZE)
    u8g2log.printf("ESP_ERR_INVALID_SIZE\n");
  else if (res == ESP_OK)
    u8g2log.printf("ERR %d\n", res);
  modeNotepadLoop();
    
  u8g2log.printf("Copy filesystem...\n");
  modeNotepadLoop();
  u8g2log.printf("Write %d bytes...", SPI_FLASH_SEC_SIZE);
  modeNotepadLoop();
  res = esp_partition_write(spifsPartition, 0, pageBuffer, SPI_FLASH_SEC_SIZE);
  if (res == ESP_OK)
    u8g2log.printf("OK\n");
  else if (res == ESP_ERR_INVALID_ARG)
    u8g2log.printf("ESP_ERR_INVALID_ARG\n");
  else if (res == ESP_ERR_INVALID_SIZE)
    u8g2log.printf("ESP_ERR_INVALID_SIZE\n");
  else if (res == ESP_OK)
    u8g2log.printf("ERR %d\n", res);
  modeNotepadLoop();
    
  {
    u8g2log.printf("Check filesystem...\n");
    modeNotepadLoop();
    for (uint32_t offset = 0; (offset + BUFFER_SIZE) < sizeof(msc_disk); offset += BUFFER_SIZE)
    {
      u8g2log.printf("Check offset %d...", offset);
      modeNotepadLoop();
      esp_err_t res = esp_partition_read(spifsPartition, offset, buffer, BUFFER_SIZE);
      if (res == ESP_OK)
      {
        if (memcmp(buffer, pageBuffer + offset, BUFFER_SIZE) != 0)
        {
          u8g2log.printf("FAIL!\n");
          modeNotepadLoop();
        }
        else
        {
          u8g2log.printf("OK!\n");
          modeNotepadLoop();
        }
      }
      else if (res == ESP_ERR_INVALID_ARG)
      {
        u8g2log.printf("ESP_ERR_INVALID_ARG\n");
        modeNotepadLoop();
      }
      else if (res == ESP_ERR_INVALID_SIZE)
      {
        u8g2log.printf("\nESP_ERR_INVALID_SIZE\n");
        modeNotepadLoop();
      }
      else
      {
        u8g2log.printf("\nERR %d", res);
        modeNotepadLoop();
      }
    }
    u8g2log.printf("== FINISHED. ==\n\n");
    modeNotepadLoop();
  }
}

void modeNotepadButtonUp()
{
  showPartitions();
}

void modeNotepadButtonDown()
{
  initPartition();
}

void modeNotepadButtonCenter()
{
  MSC.end();
  setModeAppsMenu(); // exit
}

#endif