
#ifndef SSD1306_H_
#define SSD1306_H_

#include <stdint.h>

#include "ssd1306_cfg.h"

#define SSD1306_DATA 0x40
#define SSD1306_SINGLE 0x80
#define SSD1306_COMMAND 0x00

#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF

#define SSD1306_DISPLAY_NORMAL 0xA6
#define SSD1306_DISPLAY_INVERT 0xA7

#define SSD1306_SET_CONTRAST 0x81

#define SSD1306_SET_MUX_RATIO 0xA8
#define SSD1306_MUX_RATIO(ratio) ((ratio) & 0x3F)

#define SSD1306_SCROLL_OFF 0x2E
#define SSD1306_SCROLL_ON 0x2F

#define SSD1306_SET_START_LINE(line) ((0x40 + (line)) & 0x7F)

#define SSD1306_SEG_MAP_NORMAL 0xA0
#define SSD1306_SEG_MAP_REVERSE 0xA1

#define SSD1306_COM_SCAN_DIR_NORMAL 0xC0
#define SSD1306_COM_SCAN_DIR_REVERSE 0xC8

#define SSD1306_SET_DISPLAY_OFFSET 0xD3

#define SSD1306_SET_CHARGE_PUMP 0x8D

/* 0x00 - Sequential COM pin configuration, disable L/R remap  */
/* 0x01 - Alternative COM pin configuration, disable L/R remap */
/* 0x02 - Sequential COM pin configuration, enable L/R remap   */
/* 0x03 - Alternative COM pin configuration, enable L/R remap  */
#define SSD1306_SET_COM_PINS_CONFIG 0xDA
#define SSD1306_COM_PINS_CONFIG(value) ((value & 0x03) << 4 | 0x02)

#define SSD1306_SET_PRECHARGE_PERIOD 0xD9
#define SSD1306_PRECHARGE_PERIOD(period_1, period_2) (((period_1) & 0x0F) | ((period_2) & 0x0F) << 4)

#define SSD1306_SET_CLOCK_RATIO_FREQ 0xD5
#define SSD1306_CLOCK_RATIO_FREQ(ratio, freq) (((ratio) & 0x0F) | ((freq) & 0x0F) << 4)

/* 0x00 - 0.65Vcc */
/* 0x20 - 0.77Vcc */
/* 0x30 - 0.83Vcc */
#define SSD1306_SET_VCOMH_DESEL_LEVEL 0xDB
#define SSD1306_VCOMH_DESEL_LEVEL(level) (level)

#define SSD1306_SET_MEM_ADDR_MODE 0x20

#define SSD1306_SET_COL_ADDR 0x21
#define SSD1306_SET_PAGE_ADDR 0x22

#define SSD1306_SET_START_COL_L(col) ((col) & 0x0F)
#define SSD1306_SET_START_COL_H(col) (((col) & 0x0F) | 0x10)

#define SSD1306_NOP 0xE3

typedef struct _ssd1306_i2c_drv_t
{
    int (*start)(uint8_t);
    int (*write)(const uint8_t *, unsigned int);
    void (*stop)(void);
} ssd1306_i2c_drv_t;

void ssd1306_init(ssd1306_i2c_drv_t *i2c_drv);
void ssd1306_write(const uint8_t *buf, unsigned int len, uint8_t start_col,
                    uint8_t end_col, uint8_t start_page, uint8_t end_page);
void ssd1306_clear();

void ssd1306_set_start_line(uint8_t line);
void ssd1306_scroll();


#endif /* SSD1306_H_ */