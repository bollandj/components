
#include "ssd1306.h"

#ifdef SSD1306_128x64
static const uint8_t init_seq[] =
{
    SSD1306_COMMAND,                // Command
    SSD1306_DISPLAY_OFF,            // Set Display OFF
    SSD1306_SET_CLOCK_RATIO_FREQ,   // Set display clock
    0x80,                           // Recommended value
    SSD1306_SET_MUX_RATIO,          // Set Multiplex Ratio (height - 1)
    0x3F,
    SSD1306_SET_DISPLAY_OFFSET,     // Set Display Offset
    0x00,                           // No vertical shift of COM
    SSD1306_SET_START_LINE(0),      // Set Display Start Line (0x40 - 0x7F)
    SSD1306_SET_CHARGE_PUMP,        // Charge pump
    0x14,
    SSD1306_SET_MEM_ADDR_MODE,      // Set Memory Addressing Mode
    0x01,                           // Vertical addressing mode
    SSD1306_SEG_MAP_REVERSE,         // Set Segment Re-map (0xA0/0xA1 horiz. flip)
    SSD1306_COM_SCAN_DIR_REVERSE,    // Set COM Output Scan Direction (0xC0/0xC8 vert. flip)
    SSD1306_SET_COM_PINS_CONFIG,    // Set COM Pins Hardware Configuration
    0x12,                           // 128x32/128x64/72x40  0x02/0x12/0x12
    SSD1306_SET_CONTRAST,           // Set Contrast
    0xBF,                           // 0x00 - 0xFF
    SSD1306_SET_PRECHARGE_PERIOD,   // Set Pre-charge Period
    0xF1,                           // 241x DCLK
    SSD1306_SET_VCOMH_DESEL_LEVEL,  //  Set VCOMH Deselect Level
    0x40,
    SSD1306_DISPLAY_NORMAL,         // 0xA6/0xA7 Normal/Inverse
    SSD1306_DISPLAY_ON              // Set Display ON
};
#endif

static ssd1306_i2c_drv_t *_i2c_drv=0; 

/**
 * \brief Initialise display by sending initialisation command sequence (init_seq)
 * \param i2c_drv Pointer to struct containing I2C start, stop and write functions
 */
void ssd1306_init(ssd1306_i2c_drv_t *i2c_drv)
{
    /* Register I2C driver functions */
    _i2c_drv = i2c_drv;

    /* Send initialisation sequence */
    _i2c_drv->start(SSD1306_ADDRESS);
    _i2c_drv->write(init_seq, sizeof(init_seq));
    _i2c_drv->stop();
}

static void ssd1306_set_write_range(uint8_t start_col, uint8_t end_col, uint8_t start_page, uint8_t end_page)
{
    uint8_t set_write_range_seq[] = 
    {
        SSD1306_COMMAND,
        SSD1306_SET_COL_ADDR, // Set column address range
        start_col,
        end_col,
        SSD1306_SET_PAGE_ADDR, // Set page address range
        start_page,
        end_page
    };

    _i2c_drv->start(SSD1306_ADDRESS);
    _i2c_drv->write(set_write_range_seq, sizeof(set_write_range_seq));
    _i2c_drv->stop();
}

/**
 * \brief Write a block of bytes to the display
 * \param buf pointer to the first byte in the array to be written
 * \param len total number of bytes to be written
 * \param start_col start column
 * \param start_page start page
 * \param end_col end column
 * \param end_page end page
 */
void ssd1306_write(const uint8_t *buf, unsigned int len, uint8_t start_col, 
                    uint8_t end_col, uint8_t start_page, uint8_t end_page)
{
    const uint8_t ssd1306_data_byte = SSD1306_DATA;

    ssd1306_set_write_range(start_col, end_col, start_page, end_page);

    /* Write in 32-byte blocks */
    uint8_t *buf_p = (uint8_t *)buf;

    while (len > 32)
    {   
        _i2c_drv->start(SSD1306_ADDRESS);
        _i2c_drv->write(&ssd1306_data_byte, 1);
        _i2c_drv->write(buf_p, 32);
        _i2c_drv->stop();

        buf_p += 32;
        len -= 32;
    }

    _i2c_drv->start(SSD1306_ADDRESS);
    _i2c_drv->write(&ssd1306_data_byte, 1);
    _i2c_drv->write(buf_p, len);
    _i2c_drv->stop();
}

/**
 * \brief Blank all display pixels
 */
void ssd1306_clear()
{
    const uint8_t ssd1306_data_byte = SSD1306_DATA;
    const uint8_t zero_byte = 0x00;

    ssd1306_set_write_range(0, 127, 0, 7);

    for (int i = 0; i < 32; i++)
    {
        _i2c_drv->start(SSD1306_ADDRESS);
        _i2c_drv->write(&ssd1306_data_byte, 1);
        for (int j = 0; j < 32; j++)
            _i2c_drv->write(&zero_byte, 1);
        _i2c_drv->stop();
    }
}

/**
 * \brief Set the line of the display RAM that corresponds to the first line of the display
 * \param line start line
 */
void ssd1306_set_start_line(uint8_t line)
{
    uint8_t set_start_line_seq[] = 
    {
        SSD1306_COMMAND,
        SSD1306_SET_START_LINE(line)
    };

    _i2c_drv->start(SSD1306_ADDRESS);
    _i2c_drv->write(set_start_line_seq, sizeof(set_start_line_seq));
    _i2c_drv->stop();
}

void ssd1306_scroll()
{
    uint8_t scroll_seq[] = 
    {
        SSD1306_COMMAND,
        0x26,
        0x00,
        0x04,
        0x07,
        0x05,
        0x00,
        0xFF,

        0x2F
    };

    _i2c_drv->start(SSD1306_ADDRESS);
    _i2c_drv->write(scroll_seq, sizeof(scroll_seq));
    _i2c_drv->stop();

// 	_i2c_drv->start(OLED_ADDRESS);
// 	_i2c_drv->write_single(SSD1306_COMMAND);
// 	_i2c_drv->write_single(0x2F);
// 	_i2c_drv->stop();	
}

