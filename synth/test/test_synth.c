#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <ncurses.h>

#include "wave.h"

#define NUM_OUT_BITS 16
#define NUM_ACC_BITS 20

#define FRAMES_PER_SEC 50
#define UPDATES_PER_SEC 5000

int main(void)
{
    float frequency_hz = 0.43;
    int frequency = (frequency_hz * (1 << NUM_ACC_BITS)) / UPDATES_PER_SEC;

    wave_t wave;
    wave_init(&wave, NUM_OUT_BITS, NUM_ACC_BITS);
    wave_set_freq(&wave, frequency);
    wave_set_type(&wave, WAVE_TYPE_TRI);
    wave_reset(&wave);

    int num_rows, num_cols;
    initscr();
    getmaxyx(stdscr, num_rows, num_cols);

    int wave_max = (1 << NUM_OUT_BITS) - 1;
    int clocks_per_frame = CLOCKS_PER_SEC / FRAMES_PER_SEC;
    int usec_per_frame = 1000000 / FRAMES_PER_SEC;
    int row_output_ratio = wave_max / num_rows;
    int col_output_ratio = wave_max / num_cols;

    clock_t current_time; 
    clock_t last_time = clock();

    int frame_count = 0;
 
    while (1)
    {
        current_time = clock();

        if (current_time - last_time > clocks_per_frame)
        {
            for (int i = 0; i < UPDATES_PER_SEC/FRAMES_PER_SEC; i++)
                wave_update(&wave);

            unsigned int wave_value = wave_get_value(&wave);
            int wave_col = wave_value / col_output_ratio;

            move(0, 0);
            clrtoeol();
            mvprintw(0, wave_col, "x");
            mvprintw(1, 0, "Value: %.8d / %.8d", wave_value, wave_max);
            mvprintw(2, 0, "Frequency: %f", frequency_hz);

            refresh();

            last_time += clocks_per_frame;
            frame_count++;
        }    

        //usleep(usec_per_frame);
    }

    endwin();
    
    return 0;
}