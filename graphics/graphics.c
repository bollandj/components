
#include "graphics.h"
#include "nesfont8x8.h"

static disp_write_func_t _disp_write;

/**
 * \brief Converts bit pattern abcdefgh to aabbccddeeffgghh
 * \param x input byte to be stretched
 */
static uint16_t stretch(uint16_t x)
{
	x = (x & 0xF0) << 4 | (x & 0x0F);
	x = (x << 2 | x) & 0x3333;
	x = (x << 1 | x) & 0x5555;
	return x | x << 1;
}

/**
 * \brief Converts bit pattern abcdefgh to hgfedcba
 * \param x input byte to be reversed
 */
static uint8_t reverse(uint8_t x)
{
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	return x;
}

void disp_init(disp_write_func_t disp_write)
{
    _disp_write = disp_write;
}

void disp_plot_char(char c, uint8_t line, uint8_t col)
{
	uint8_t n = 0x00;
	_disp_write(&font[(c - FONT_OFFSET) * (CHAR_WIDTH - 1)], CHAR_WIDTH - 1, col, col + CHAR_WIDTH - 2, line, line);
	_disp_write(&n, 1, col + CHAR_WIDTH - 1, col + CHAR_WIDTH - 1, line, line);
}

void disp_plot_str(const char *str, uint8_t line, uint8_t col)
{
	char c;
	uint8_t column = col * CHAR_WIDTH;

	for (uint8_t i = 0; (c = *str++); i++)
	{
		disp_plot_char(c, line, column);
		column += CHAR_WIDTH;
	}
}

static const uint32_t factors[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, \
                                     10000000, 100000000, 1000000000};

void disp_plot_int(uint32_t num, uint8_t line, uint8_t col, uint8_t digits)
{
	uint8_t z = 0;

	uint8_t column = col * CHAR_WIDTH;

	for (int8_t i = digits - 1; i >= 0; i--)
	{
		uint8_t q = num / factors[i];

		if (q)
		{
			if (q > 9) q = 9;
			disp_plot_char('0' + q, line, column);
			num -= q * factors[i];
			z = 1;
		}
		else
		{
			if (z || !i) disp_plot_char('0', line, column);
			else disp_plot_char(' ', line, column);
		}

		column += CHAR_WIDTH;
	}
}

#if 0

void plot_int_dd(uint32_t num, uint8_t line, uint8_t col, uint8_t digits)
{
	/* Double Dabble implementation */
	uint32_t bcd = 0;
	/* An integer of this width can have at most 8 decimal digits */
	const uint8_t NUM_BITS = 26;
	for (int8_t i = 0; i < NUM_BITS; i++)
	{
		/* Add 3 to each nibble which is greater than 4 */
		for (int8_t j = 0; j < 32; j += 4)
			if (((bcd >> j) & 0x0F) > 4) bcd += (0x03 << j);
		/* Shift */
		bcd = (bcd << 1) | (num >> (NUM_BITS - 1));
		/* Prepare next bit of num */
		num <<= 1;
	}

	for (int8_t i = 28, c = col; i >= 0; i -= 4, c += CHAR_WIDTH)
	{
		uint8_t q = (bcd >> i) & 0x0F;
		disp_plot_char('0' + q, line, c);
	}
}

#endif

void disp_plot_int_signed(int32_t num, uint8_t line, uint8_t col, uint8_t digits)
{
	uint8_t z = 0;
	char sign;

	uint8_t column = col * CHAR_WIDTH;

	if (num < 0)
	{
		sign = '-';
		num = -num;
	}
	else
	{
		sign = ' ';
	}

	for (int8_t i = digits - 1; i >= 0; i--)
	{
		uint8_t q = num / factors[i];

		if (q)
		{
			if (q > 9) q = 9;

			if (!z)
			{
				disp_plot_char(sign, line, column);
				column += CHAR_WIDTH;
				z = 1;
			}

			disp_plot_char('0' + q, line, column);
			num -= q * factors[i];
		}
		else
		{
			if (z)
			{
				disp_plot_char('0', line, column);
			}
			else if (!i)
			{
				disp_plot_char(' ', line, column);
				column += CHAR_WIDTH;
				disp_plot_char('0', line, column);
			}
			else
			{
				disp_plot_char(' ', line, column);
			}
		}

		column += CHAR_WIDTH;
	}
}

void disp_plot_bin(uint32_t num, uint8_t line, uint8_t col, uint8_t bits)
{
	uint8_t column = col * CHAR_WIDTH;

	disp_plot_str("0b", line, column);
	column += CHAR_WIDTH * (bits + 2); // move to end and plot backwards

	uint8_t bit;
	for (uint8_t i = bits; i > 0; i--)
	{
		column -= CHAR_WIDTH;
		bit = num & 0x00000001;

		if (bit) disp_plot_char('1', line, column);
		else disp_plot_char('0', line, column);

		num >>= 1;
	}
}

void disp_plot_hex(uint32_t num, uint8_t line, uint8_t col, uint8_t nibbles)
{
	uint8_t column = col * CHAR_WIDTH;

	disp_plot_str("0x", line, column);
	column += CHAR_WIDTH * (nibbles + 2); // move to end and plot backwards

	uint8_t nibble;
	for (uint8_t i = nibbles; i > 0; i--)
	{
		column -= CHAR_WIDTH;
		nibble = num & 0x0000000F;

		if (nibble < 10) disp_plot_char(nibble + 0x30, line, column);
		else disp_plot_char(nibble + 0x37, line, column);

		num >>= 4;
	}
}