
#ifndef PCD8544_CFG_H_
#define PCD8544_CFG_H_

#include "sam.h"

#define DISPLAY_WIDTH 84
#define DISPLAY_HEIGHT 48

#define PCD8544_DC_PIN  PIN_PA03
#define PCD8544_DC_PORT  (1 << PCD8544_DC_PIN)

#define PCD8544_CE_PIN  PIN_PA02
#define PCD8544_CE_PORT  (1 << PCD8544_CE_PIN)

#define PCD8544_RST_PIN  PIN_PA04
#define PCD8544_RST_PORT  (1 << PCD8544_RST_PIN)

#endif //PCD8544_CFG_H_
