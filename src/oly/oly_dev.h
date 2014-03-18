/* oly_dev.h -- Debug functions {{{
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
   }}} */

#ifndef SRC_OLY_DEV_H
#define SRC_OLY_DEV_H 1
#include "oly/olytypes.h"

BEGIN_C_DECLS

typedef struct res_disp_flag_t {
        unsigned int all;
        unsigned int help;
        unsigned int version;
        unsigned int only_locale;
        unsigned int badopt;
        unsigned int tables;
        unsigned int arrays;
        unsigned int strings;
        unsigned int binaries;
        unsigned int aliases;
        unsigned int integers;
        unsigned int vectors;
    } res_disp_flag;

extern void list_icu_countries(void);
extern void list_icu_langs(void);
extern void list_package_locales(const char *package_name);
extern void list_table_resources(UResourceBundle *res, const res_disp_flag *flag, const int level);
extern void list_array_resources(UResourceBundle *res, const res_disp_flag *flag, const int level);
extern void flag_res_display(UResourceBundle *res, const res_disp_flag *flag, const int level);
extern char *level_indent(char *indent, size_t size, const int level);
extern void init_res_disp_flag(res_disp_flag *flag);
extern const char *get_resource_type(UResourceBundle *res);
extern UResType flag_check(UResourceBundle *res, const res_disp_flag *flag);

extern void print_config( OlyStatus *status );

typedef enum bash_pretty_attrs_enum {
    RESET = 0,
    BRIGHT = 1,
    DIM = 2,
    UNDERLINE = 3,
    BLINK = 4,
    REVERSE = 7,
    HIDDEN = 8
} BashAttr;

typedef enum bash_pretty_colors_enum {
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    CYAN = 6,
    WHITE = 7
} BashColors;

extern void print_stdout_char_color(BashColors fg, BashColors bg, BashAttr attr, const char *text);
extern void print_stderr_char_color(BashColors fg, BashColors bg, BashAttr attr, const char *text);
extern void print_stdout_OChar_color(BashColors fg, BashColors bg, BashAttr attr, const OChar *text);
extern void print_stderr_OChar_color(BashColors fg, BashColors bg, BashAttr attr, const OChar *text);

END_C_DECLS

#endif /* SRC_OLY_DEV_H */
