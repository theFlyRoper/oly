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

BEGIN_C_DECLS

typedef struct res_disp_flag_t {
        unsigned char all;
        unsigned char tables;
        unsigned char arrays;
        unsigned char strings;
        unsigned char binaries;
        unsigned char aliases;
        unsigned char integers;
        unsigned char vectors;
    } res_disp_flag;

extern void list_icu_countries(void);
extern void list_icu_langs(void);
extern void list_package_locales(const char *package_name);
extern void list_bundle_resources(UResourceBundle *res, const res_disp_flag *flag, const int level);
extern void display_resource_type(UResourceBundle *res, const int level);
extern void ifelse_res_display(UResourceBundle *res, const res_disp_flag flag, const int level);
char *get_resource_type(UResourceBundle *res);

END_C_DECLS

#endif /* SRC_OLY_DEV_H */
