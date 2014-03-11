/* token_str_to_array - turn a string into a set of pointers to strings.  License GPL2+ {{{
 * Copyright (C) 2014 Oly Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * }}} */

/*  the two counters may be null but status, string and delims are required.
 */
char **
token_str_to_array(char *s, char *delims, unsigned int *count_chars, 
        unsigned int *count_tokens, OlyStatus *status) 
{
    int32_t             num_chars = 0, num_tokens = 0;
    /* we need arr_size char pointers and arr_size (for the EOLs) + num_chars chars. */
    char          **result, *c = xstrdup(s), *r, *rest;
    
    assert(s != NULL && delims != NULL && status != NULL);
    num_chars = 0;
    r = strtok_r(c, delims, &rest);
    while((r) = strtok_r(NULL, delims, &rest)) {
        (num_tokens)++;
    }
    result = (char **)xmalloc((num_tokens)*sizeof(char *));
    OFREE(c);
    num_tokens = 0;
    c = xstrdup(s);
    r = strtok_r(c, delims, &rest);
    result[(num_tokens)++] = r;
    (num_chars) += strlen(r);
    while(r = strtok_r(NULL, delims, &rest))
    {
        result[(num_tokens)++] = r;
        (num_chars)+=strlen(r);
    }
    
    if (count_tokens != NULL) {
        *count_tokens = num_tokens;
    }
    if (count_chars != NULL) {
        *count_chars = num_chars;
    }
    return result;
}

