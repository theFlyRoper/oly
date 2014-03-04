/* get_message.c - pull an indexed user message out of a message array. {{{
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

oly_status
get_message(ochar **message, int32_t *message_len,
        const oly_status oly_errno)
{
    int32_t result_len = 0;
    oly_status return_status = OLY_OKAY;
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    ochar *err_text = NULL;
    oly_errors = init_errhandler();
    assert(message_len != NULL);
    result_len = *message_len;

    printf("Just pre getstringbyindex\n");
    err_text = ures_getStringByIndex(
            oly_errors, oly_errno, &result_len, &u_status);
    u_fprintf(u_stderr, "Err text: %s\n", err_text);

    if (U_FAILURE(u_status)) {
#ifdef OLYDEV
        fprintf(stderr, "oly error number %i not found. status: %s.\n", 
                oly_errno, u_errorName(u_status));
#endif /* OLYDEV */
        err_text = NULL;
        return_status = OLY_WARN_ERROR_NOT_FOUND;
    } else {
        *message = err_text;
    }
#endif
    printf("Just after getstringbyindex\n");
    return return_status;
}
