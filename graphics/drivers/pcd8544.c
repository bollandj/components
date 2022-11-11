
#include "pcd8544.h"
#include "gpio.h"
#include "spi.h"

void display_init()
{
    gpio_port_set_output(PCD8544_CE_PORT | PCD8544_DC_PORT | PCD8544_RST_PORT);
    gpio_port_set(PCD8544_CE_PORT | PCD8544_DC_PORT | PCD8544_RST_PORT);

    /* Reset sequence */
    gpio_port_clear(PCD8544_RST_PORT);
    for (uint32_t x = 0; x < 10000; x++) __asm("nop");
    gpio_port_set(PCD8544_RST_PORT);
    for (uint32_t x = 0; x < 10000; x++) __asm("nop");

    uint8_t init_seq[5] = {
        0x21, // extended instruction set (H = 1)
        0x13, // 1:48 bias
        0xC2, // Vop = 7V
        0x20, // normal instruction set (H = 0)
        //0x09, // all display segments on
        //0x08, // blank display
        0x0C  // normal mode
    };

    gpio_port_clear(PCD8544_CE_PORT);
    gpio_port_clear(PCD8544_DC_PORT); // command

    spi_write(init_seq, 5);

    gpio_port_set(PCD8544_CE_PORT);
}

void display_write(const uint8_t *buf,
                   uint32_t len,
                   uint8_t start_col,
                   uint8_t end_col,
                   uint8_t start_page,
                   uint8_t end_page)
{
    gpio_port_clear(PCD8544_CE_PORT);
    gpio_port_clear(PCD8544_DC_PORT); // command

    uint8_t pos[2] = {(0x80 | start_col), (0x40 | start_page)};
    spi_write(pos, 2);

    gpio_port_set(PCD8544_DC_PORT); // data

    spi_write(buf, len);

    gpio_port_set(PCD8544_CE_PORT);
}

void display_clear()
{
    gpio_port_clear(PCD8544_CE_PORT);
    gpio_port_clear(PCD8544_DC_PORT); // command

    uint8_t pos[2] = {0x80, 0x40};
    spi_write(pos, 2);

    gpio_port_set(PCD8544_DC_PORT); // data

    uint8_t n = 0x00;
    for (uint32_t i = 0; i < (84 * 6); i++) spi_write(&n, 1);

    gpio_port_set(PCD8544_CE_PORT);
}