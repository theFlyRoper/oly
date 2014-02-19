/* builtin.h -- installing and removing oly builtin commands {{{
  
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

#ifndef OLY_BUILTIN_H
#define OLY_BUILTIN_H 1

BEGIN_C_DECLS

/** @start 1 */
typedef int (*builtin_handler) (Oly *oly,
                                int argc, 
                                char *const argv[]);

typedef struct {
  const char *name;
  builtin_handler func;
  int min, max;
} Builtin;

typedef struct builtintab BuiltinTab;

extern Builtin *builtin_find (Oly *oly, const char *name);
extern int builtin_install   (Oly *oly, Builtin *table);
extern int builtin_remove    (Oly *oly, Builtin *table);
/** @end 1 */

END_C_DECLS

#endif /* !OLY_BUILTIN_H */
