/*
 * TM1638-Teensy/TM1638-Teensy.h
 * Copyright (C) 2019
 * Shaun Landis <slandis.gmail.com>
 *
 */

 #if defined(ARDUINO) && ARDUINO >= 100
 	#include "Arduino.h"
 #else
 	#include "WProgram.h"
 #endif

//      Bits:                 Hex:
//        -- 0 --               -- 01 --
//       |       |             |        |
//       5       1            20        02
//       |       |             |        |
//        -- 6 --               -- 40 --
//       |       |             |        |
//       4       2            10        04
//       |       |             |        |
//        -- 3 --  .7           -- 08 --   .80

#define SEG_A   0x01
#define SEG_B   0x02
#define SEG_C   0x04
#define SEG_D   0x08
#define SEG_E   0x10
#define SEG_F   0x20
#define SEG_G   0x40
#define SEG_DOT 0x80

#define BLANK   0x00

#define NBR_0   (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define NBR_1   (SEG_B | SEG_C)
#define NBR_2   (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G)
#define NBR_3   (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G)
#define NBR_4   (SEG_B | SEG_C | SEG_F | SEG_G)
#define NBR_5   (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G)
#define NBR_6   (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define NBR_7   (SEG_A | SEG_B | SEG_C)
#define NBR_8   (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define NBR_9   (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)

#define LTR_A   (SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G)
#define LTR_B   (SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define LTR_C   (SEG_A | SEG_D | SEG_E | SEG_F)
#define LTR_D   (SEG_B | SEG_C | SEG_D | SEG_E | SEG_G)
#define LTR_E   (SEG_A | SEG_D | SEG_E | SEG_F | SEG_G)
#define LTR_F   (SEG_A | SEG_E | SEG_F | SEG_G)
#define LTR_G   (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)
#define LTR_H   (SEG_B | SEG_C | SEG_E | SEG_F | SEG_G)
#define LTR_I   (SEG_B | SEG_C)
#define LTR_J   (SEG_B | SEG_C | SEG_D | SEG_E)
#define LTR_K   (SEG_G | SEG_DOT)
#define LTR_L   (SEG_D | SEG_E | SEG_F)
#define LTR_M   (SEG_G | SEG_DOT)
#define LTR_N   (SEG_C | SEG_E | SEG_G)
#define LTR_O   (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define LTR_P   (SEG_A | SEG_B | SEG_E | SEG_F | SEG_G)
#define LTR_Q   (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_DOT)
#define LTR_R   (SEG_E | SEG_G)
#define LTR_S   (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G)
#define LTR_T   (SEG_D | SEG_E | SEG_F | SEG_G)
#define LTR_U   (SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define LTR_V   (SEG_B | SEG_C | SEG_D | SEG_F)
#define LTR_W   (SEG_G | SEG_DOT)
#define LTR_X   (SEG_B | SEG_C | SEG_E | SEG_F | SEG_G)
#define LTR_Y   (SEG_B | SEG_C | SEG_F | SEG_G)
#define LTR_Z   (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G)

#define DISP_MODE   0x80

#define WRITE_AUTO  0x40
#define WRITE_FIXED 0x44

#define CS_RISE_US      5
#define READ_DELAY_US   1

class TM1638_Teensy {
public:
  TM1638_Teensy(uint8_t data, uint8_t clock, uint8_t strobe, boolean active, uint8_t brightness);

  void setBrightness(uint8_t brightness);
  void setDisplay(boolean active);

  void setDisplayAt(uint8_t position, uint8_t value);
  void setDisplay(uint8_t message[]);

  void setLEDAt(uint8_t position, boolean active);
  void setLEDs(uint8_t mask);

  void clearDisplay();
  void clearLEDs();
  void clearAll();

  uint8_t readKeys();

private:
  void sendByte(uint8_t value);
  void sendCommand(uint8_t value);
  uint8_t receiveByte();

  uint8_t data;
  uint8_t clock;
  uint8_t strobe;
  uint8_t brightness;
  boolean active;
};
