/* output.h -- basic output functions from inspired by ubreak in ICU licence GPL2+ {{{
  
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

#ifndef OLY_OUTPUT_H
#define OLY_OUTPUT_H 1

BEGIN_C_DECLS

void print_text_range( UChar* str, int32_t start, int32_t end );
void print_each_forward( UBreakIterator* boundary, UChar* str );
void print_each_backward( UBreakIterator* boundary, UChar* str);
void print_first(UBreakIterator* boundary, UChar* str);
void print_last(UBreakIterator* boundary, UChar* str);
void print_at(UBreakIterator* boundary, int32_t pos , UChar* str);

END_C_DECLS

#endif /* !OLY_OUTPUT_H */
