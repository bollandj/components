
#include "menu.h"
#include "graphics.h"

void menu_init(menu_t *menu, menu_list_t *init_menu_list)
{
    menu->current_list = init_menu_list;
    menu->item_idx = 0;    
    menu->frame_idx = 0;    
}

void menu_goto(menu_t *menu, menu_list_t *new_menu_list)
{
    menu->current_list = new_menu_list;
    menu->item_idx = 0;    
    menu->frame_idx = 0;    
}

void menu_item_register_sel_cb(menu_item_t *menu_item, void (*_sel_cb)(menu_t *))
{
    menu_item->sel_cb = _sel_cb;   
}

#define NUM_LINES 4
#define CHARS_PER_LINE 128/8

void menu_draw(menu_t *menu)
{
    int menu_list_len = menu->current_list->length; 

    disp_plot_int(menu_list_len, 6, 0, 2);

    for (int idx = 0; idx < NUM_LINES; idx++)
    {
        menu_item_t item = menu->current_list->items[menu->frame_idx + idx]; 
        disp_plot_str("       ", idx, 0);
        disp_plot_str(item.name, idx, 1);
    }

    disp_plot_str(">", menu->item_idx - menu->frame_idx, 0);
}

void menu_next(menu_t *menu)
{
    int menu_list_len = menu->current_list->length; 

    if (menu->item_idx == menu_list_len - 1)
    {
        menu->item_idx = 0;
        menu->frame_idx = 0;
    }
    else if (menu->item_idx >= menu->frame_idx + NUM_LINES - 1) {
        menu->item_idx++;
        menu->frame_idx++;
    } 
    else menu->item_idx++;
}

void menu_prev(menu_t *menu)
{
    int menu_list_len = menu->current_list->length; 

    if (menu->item_idx == 0)
    {
        menu->item_idx = menu_list_len - 1;
        menu->frame_idx = menu_list_len - NUM_LINES;
    }
    else if (menu->item_idx <= menu->frame_idx) {
        menu->item_idx--;
        menu->frame_idx--;
    } 
    else menu->item_idx--;   
}

void menu_sel(menu_t *menu)
{
    menu_item_t *menu_item = &menu->current_list->items[menu->item_idx];
    if (menu_item->sel_cb) menu_item->sel_cb(menu);
}
