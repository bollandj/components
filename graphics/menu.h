
#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>
#include <stddef.h>

#include "graphics.h"

typedef enum {NONE, UINT, INT, HEX, BIN, STR} value_type_t;

typedef struct menu_t_ menu_t;
typedef struct menu_list_t_ menu_list_t;
typedef struct menu_item_t_ menu_item_t;

struct menu_t_
{     
    menu_list_t *current_list;
    unsigned int item_idx; 
    unsigned int frame_idx; 
};

struct menu_list_t_
{
    const char *name;     /**< Display name */
    unsigned int length;  /**< Number of items in this list */
    menu_item_t *items;   /**< Array of items in this list */
};

struct menu_item_t_
{
    const char *name;           /**< Display name */
    uint8_t name_len;           /**< Length of display name */
    void *value;                /**< Display value (if applicable) */
    uint8_t value_len;          /**< Length of display value */
    value_type_t value_type;    /**< Type of display value */
    void (*sel_cb)(menu_t *);   /**< Function to be called when menu option clicked */
};


#define MENU_ITEM_TEXT(_name)           \
{                                       \
    .name = _name,                      \
    .name_len = (sizeof(_name) - 1),    \
    .value = NULL,                      \
    .value_type = NONE,                 \
    .sel_cb = NULL                      \
}

#define MENU_ITEM_UINT(_name, _value, _value_digits)    \
{                                                       \
    .name = _name,                                      \
    .name_len = (sizeof(_name) - 1),                    \
    .value = _value,                                    \
    .value_len = _value_digits,                         \
    .value_type = UINT,                                 \
    .sel_cb = NULL                                      \
}

#define MENU_LIST(_name, _items)                    \
{                                                   \
    .name = _name,                                  \
    .length = (sizeof(_items)/sizeof(menu_item_t)), \
    .items = _items                                 \
}

void menu_init(menu_t *menu, menu_list_t *init_menu_list);

void menu_goto(menu_t *menu, menu_list_t *new_menu_list);

void menu_item_register_sel_cb(menu_item_t *menu_item, void (*_sel_cb)(menu_t *));

void menu_draw(menu_t *menu);

void menu_next(menu_t *menu);
void menu_prev(menu_t *menu);
void menu_sel(menu_t *menu);

#endif /* MENU_H_ */

