
#ifndef PCD8544_H_
#define PCD8544_H_

#include "sam.h"

#include "pcd8544_cfg.h"

void display_init();
void display_write(const uint8_t *buf, uint32_t len, uint8_t start_col,
                    uint8_t end_col, uint8_t start_page, uint8_t end_page);
void display_clear();

#endif //PCD8544_H_
