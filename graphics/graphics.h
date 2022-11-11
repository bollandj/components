
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdint.h>

typedef void (*disp_write_func_t)(const uint8_t *buf, unsigned int len, uint8_t start_col,
                                    uint8_t end_col, uint8_t start_page, uint8_t end_page);

/**
 * \brief Initialise graphics driver
 * \param disp_write_func pointer to display write function
 */
void disp_init(disp_write_func_t disp_write);

/**
 * \brief Plot a single character onscreen (NORMAL scale)
 * \param c character to be plotted
 * \param line line on which character is to be plotted
 * \param col start column for character (0 - DISPLAY_WIDTH-1)
 */
void disp_plot_char(char c, uint8_t line, uint8_t col);

/**
 * \brief Plot an ASCII string onscreen
 * \param str string to be plotted
 * \param line line on which string is to be plotted
 * \param col start column (0 - floor(DISPLAY_WIDTH/CHAR_WIDTH)-1)
 */
void disp_plot_str(const char *str, uint8_t line, uint8_t col);

/**
 * \brief Plot an unsigned integer
 * \param num number to be plotted
 * \param line line on which number is to be plotted
 * \param col start column (0 - floor(DISPLAY_WIDTH/CHAR_WIDTH)-1)
 * \param digits maximum number of digits to expect (this number of characters' worth of
          space will be occupied on the display)
 */
void disp_plot_int(uint32_t num, uint8_t line, uint8_t col, uint8_t digits);

/**
 * \brief Plot a signed integer
 * \param num number to be plotted
 * \param line line on which number is to be plotted
 * \param col start column (0 - floor(DISPLAY_WIDTH/CHAR_WIDTH)-1)
 * \param digits maximum number of digits to expect (this number of characters' worth of
          space, plus one more for '-' sign, will be occupied on the display)
 */
void disp_plot_int_signed(int32_t num, uint8_t line, uint8_t col, uint8_t digits);

/**
 * \brief Plot an integer as binary
 * \param num number to be plotted
 * \param line line on which number is to be plotted
 * \param col start column (0 - floor(DISPLAY_WIDTH/CHAR_WIDTH)-1)
 * \param digits maximum number of bits to expect (this number of characters' worth of
          space will be occupied on the display)
 */
void disp_plot_bin(uint32_t num, uint8_t line, uint8_t col, uint8_t bits);

/**
 * \brief Plot an integer as hex
 * \param num number to be plotted
 * \param line line on which number is to be plotted
 * \param col start column (0 - floor(DISPLAY_WIDTH/CHAR_WIDTH)-1)
 * \param digits maximum number of nibbles to expect (this number of characters' worth of
          space will be occupied on the display)
 */
void disp_plot_hex(uint32_t num, uint8_t line, uint8_t col, uint8_t nibbles);

#endif /* GRAPHICS_H_ */
