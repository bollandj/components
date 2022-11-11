
#ifndef SSD1306_CFG_H_
#define SSD1306_CFG_H_

/* Right-aligned 7-bit I2C address (0x3C or 0x3D) */
#define SSD1306_ADDRESS 0x3C

#define SSD1306_128x64

#ifdef SSD1306_128x64
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define COL_OFFSET 0    // 28 for 72x40, 0 otherwise
#endif

#endif //SSD1306_CFG_H_
