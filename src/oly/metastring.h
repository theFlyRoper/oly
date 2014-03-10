/* metastring.h -- a simple way to store internal and display versions of object metadata. License GPL2+ {{{
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

#ifndef OLY_METASTRING_H
#define OLY_METASTRING_H 1

/* We will need many metastring access functions and these builder macros 
 * simplify that, to a degree. */

#define METANAME(prefix, suffix) get_##prefix##_##suffix

#define METASTRING_FACTORY(object, thing) \
    extern OlyMetastring * METANAME(object,thing)(const object *input)

#define METASTRING_FUNCTION(object, thing) \
    OlyMetastring * METANAME(object,thing)(const object *input) \
        { return input->thing ; }

#define INNER_META(object, thing, me) ( \
    (METANAME(object,thing)(me))->internal )

#define DISP_META(object, thing, me) ( \
    (METANAME(object,thing)(me))->display )

/* metastrings simplify logging and reporting. 
 * the struct is transparent because we use
 * them everywhere. */
struct OlyMetastring_struct {
    char    *internal;
    OChar   *display;
};

typedef struct OlyMetastring_struct OlyMetastring;

OlyMetastring   *new_metastring(const char *input);
void             close_metastring(OlyMetastring *close_me);

#endif /* !OLY_METASTRING_H */

