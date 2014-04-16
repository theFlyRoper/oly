/* olytypes.h -- typedef for OlyStatus enum License GPL2+ {{{
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

#ifndef OLY_OLYTYPES_H
#define OLY_OLYTYPES_H 1

#define DEFAULT_BUFFER_SIZE BUFSIZ
#define MAX_NODE_DEPTH 16
#define MAX_KEY_LENGTH (DEFAULT_BUFFER_SIZE/(MAX_NODE_DEPTH))

#ifdef OLYDEV
    #define OLYLOG(fmt, args) printf("%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__,args)
#else
    #define OLYLOG(fmt, args)
#endif

/* error handling macros */
#define HANDLE_OLY_STATUS(status, action) \
    if ( status != OLY_OKAY ) { u_fprintf(u_stderr, "%s:%d:%s(): %S\n", \
            __FILE__, __LINE__, __func__, get_errmsg( status )); action ; }

#define HANDLE_STATUS_AND_RETURN(status) \
    HANDLE_OLY_STATUS(status, return status)
#define HANDLE_STATUS_AND_DIE(status) \
    HANDLE_OLY_STATUS(status, exit(EXIT_FAILURE))
#define HANDLE_STATUS_AND_CONTINUE(status) \
    HANDLE_OLY_STATUS(status, status = OLY_OKAY )

BEGIN_C_DECLS


/* main oly structure. details in src/pvt_core.h */
struct  oly_struct; 
typedef struct oly_struct Oly;

/* OlyStatus type:
 * Defines states for Oly.
 * 
 * Negative numbers and zero are non-error states
 * or warning states. Positive numbers are error 
 * states. Starts at lowest negative number and goes 
 * up. 
 *
 * Standard return state is OLY_OKAY, which you should
 * test for after calling most functions. Use others as
 * appropriate. OLY_OKAY is equal to zero.
 */

typedef enum OlyStatus_enum {
    /* OLY_STATUS_MIN helps map these internal codes into the 
     * external i18n resource files.     */
    OLY_WARN_NODE_CONSUMED=-16,
    OLY_STATUS_MIN=-16,
    OLY_STATUS_OFFSET=16,
    OLY_WARN_NODE_PRODUCED=-15,
    OLY_WARN_BOUNDARY_RESET=-14,
    OLY_WARN_LONG_STRING=-13,
    OLY_WARN_BUFFER_WRITE_LOCK=-12,
    OLY_WARN_BUFFER_EMPTY=-11,
    OLY_WARN_TOP_LEVEL_NODE=-10,
    OLY_WARN_NODE_HAS_NO_KEY=-9,
    OLY_WARN_END_OF_ARGS=-8,
    OLY_WARN_DS_BUFFER_DEFAULT=-7,
    OLY_WARN_SHOW_VERSION=-6,
    OLY_WARN_SHOW_HELP=-5,
    OLY_WARN_DSOPT_NOT_USED=-4,
    OLY_WARN_REINIT=-3,
    OLY_WARN_LIB=-2,
    OLY_WARN_EXIT=-1,
    OLY_OKAY=0,
    OLY_ERR_UNKNOWN=1,
    OLY_ERR_SYS=2,
    OLY_ERR_LIB=3,
    OLY_ERR_INIT=4,
    OLY_ERR_NOMEM=5,
    OLY_ERR_NOPWD=6,
    OLY_ERR_NOUSER=7,
    OLY_ERR_FILEIO=8,
    OLY_ERR_READHEX=9,
    OLY_ERR_HASH=10,
    OLY_ERR_BADARG=11,
    OLY_ERR_BUFFER_OVERFLOW=12,
    OLY_ERR_FILE_NOT_FOUND=13,
    OLY_ERR_CONFIG_FILE_NOT_FOUND=14,
    OLY_ERR_LIBYAML_INIT=15,
    OLY_ERR_NODES_TOO_DEEP=16,
    OLY_ERR_NODES_TOO_SHALLOW=17,
    OLY_ERR_UNKNOWN_FUNCTION_TYPE=18,
    OLY_ERR_DS_OPTION_CONFLICT=19,
    OLY_ERR_LTDL_UNKNOWN=20,
    OLY_ERR_LTDL_ERR=21,
    OLY_ERR_YAML_PARSE=22,
    OLY_ERR_SQLITE_INIT=23,
    OLY_ERR_SQLITE=24,
    OLY_ERR_KEY_STR_TOO_LONG=25,
    OLY_ERR_NO_KEY_BUFFER=26,
    OLY_ERR_NODE_MUST_HAVE_VALUE=27,
    OLY_ERR_NODE_MUST_NOT_HAVE_VALUE=28,
    OLY_ERR_ILLEGAL_TAG=29,
    OLY_ERR_NO_OBJECT=30,
    OLY_ERR_NO_RESERVATION=31,
    OLY_ERR_STRING_BUFFER_STATE=32,
    OLY_ERR_NODE_QUEUE_FULL=33,
    OLY_ERR_NODE_QUEUE_EMPTY=34,
    OLY_ERR_DS_EOF=35,
    OLY_ERR_CONFIG_ITEM_NOT_FOUND=36,
    OLY_ERR_CONFIG_UNKNOWN=37,
    OLY_STATUS_MAX=37
} OlyStatus;

/* OFILE IO */
extern OFILE *u_stderr;
extern OFILE *u_stdout;
extern OFILE *u_stdin;
extern char **environ; 
extern Oly *oly;

extern OChar *get_errmsg( OlyStatus status );
END_C_DECLS

#endif /* !OLY_OLYTYPES_H */

