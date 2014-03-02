dnl Oly configure.ac License (GPL 2+) {{{
dnl (C) Oly Project
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2, or (at your option)
dnl any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
dnl MA 02110-1301, USA.

dnl SECTION 1: Initialization & Setup
dnl SECTION 2: Architecture, target, and host checks
dnl SECTION 3: Command line arguments for the configure script.
dnl SECTION 4: Check for programs we need and that they are the right version
dnl SECTION 5: Check for libraries
dnl SECTION 6: Check for header files
dnl SECTION 7: Check for types and structures
dnl SECTION 8: Check for specific functions needed
dnl SECTION 9: Additional checks, variables, etc.
dnl SECTION 10: Specify the output files and generate it
dnl

dnl }}}

dnl establish the necessary version for autoconf.
AC_PREREQ(2.60)

AC_INIT([Oly],[1.0.1],[bugs@fakesite.com],[oly],[http://fakesite.com])

AC_SUBST(OLY_COPYRIGHT,["Copyright (c) 2014 The Oly Project."])
AC_COPYRIGHT([Copyright (c) 2014 The Oly Project.])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([ac-aux])
AC_CONFIG_MACRO_DIR([ac-aux/m4])
AC_CONFIG_TESTDIR([tests])
AC_CONFIG_LIBOBJ_DIR([lib])

AM_INIT_AUTOMAKE([1.8 subdir-objects \
                  dist-bzip2 no-dist-gzip readme-alpha -Wno-portability])

AC_CONFIG_HEADERS([src/olyconf.h:src/olyconf-h.in])
dnl questions address
AC_SUBST(FAQ_ADDR,[maintainer@fakesite.com])

dnl Architecture, target and host checks 
dnl must run early, otherwise, we won't know our host system.

AC_CANONICAL_HOST

dnl Determine the platform type and cache its value.
dnl Oly includes the source code for SHA-3, optimized 
dnl for 32 bit or 64 bit systems, and hence, checks the 
dnl host value.

dnl by all means add more.  This is just a preemptive catch.
dnl case $host in
dnl  x86_64-*)
dnl    AC_CONFIG_LINKS([src/oly/cpu.h:src/oly/cpu/x86_64.h]) ;;
dnl    ;;
dnl  *)
dnl    AC_CONFIG_LINKS([src/oly/cpu.h:src/oly/cpu/ix86.h]) ;;
dnl    ;;
dnl esac


dnl command line args 
dnl these args have messages at the bottom, after AC_OUTPUT, because
dnl that ensures they are very obvious to maintainers and users.

dnl simple configuration level debug mode, define in makefile.am

AC_ARG_ENABLE(debug,
    [AS_HELP_STRING([--enable-debug],
                    [enables debug defines (default is no).])],
                    [enable_debug=yes],
                    [enable_debug=no])

AS_IF([test "x$enable_debug" != xyes],,
      [AC_SUBST(DEV_DEF,["-DOLYDEV=1"])
       AC_SUBST(DEV_CFLAGS,["-ggdb3"])])

AS_IF([test "x$enable_debug" != xyes],,
      [AC_SUBST(DEV_LIB,["liboly_dev.a"])])

CFG_LOCALEDIR="${CFG_LOCALEDIR-${datadir}/locale/oly}"


dnl Checks for programs.
AC_PROG_CPP
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MKDIR_P

AC_PATH_PROG([RMV],[rm],[])
AC_PATH_PROG([GENRB],[genrb],[])
AC_PATH_PROG([PKGDATA],[pkgdata],[])

AM_PROG_CC_C_O

LT_INIT

dnl checks for libraries 

dnl adapted to oly. finds two extra variables;
dnl ICUIO_LIBS and ICULAYOUT_LIBS

OLY_CHECK_ICU([4.8],[],[])

dnl checks for header files 
AC_INCLUDES_DEFAULT
AC_HEADER_SYS_WAIT

dnl checks for ICU headers at the end; check as needed.
dnl                  sqlite3.h \
AC_CHECK_HEADERS([locale.h paths.h \
                  unicode/ustdio.h unicode/umsg.h unicode/ures.h \
                  unicode/ubrk.h unicode/ucnv.h \
                  unicode/ustring.h unicode/usprep.h])

AC_TYPE_PID_T

AC_FUNC_MALLOC
AM_WITH_DMALLOC

dnl ACX_PTHREAD

AC_CHECK_FUNCS([bzero \
                memset], \
                [break])

AC_CHECK_FUNCS([calloc \
                strchr \
                strrchr \
                setlocale])

AC_REPLACE_FUNCS([basename \
                  strsignal \
                  strtok_r \
                  waitpid])

dnl verify how big the system is.
AC_CHECK_SIZEOF([size_t])

AS_IF([test "x$ac_cv_sizeof_size_t" = x],
      [AC_SUBST(ac_cv_sizeof_size_t,[4])],
      [])

AM_CONDITIONAL(SHA3_64BIT,
               [test "x$ac_cv_sizeof_size_t" = x8])

AC_C_CONST
AC_C_VOLATILE
AC_C_INLINE

dnl add later; openmp helps with multicore resource sharing.
dnl more details on use here:
dnl http://www.gnu.org/software/autoconf/manual/autoconf.html#Generic-Compiler-Characteristics
dnl (at the bottom)
dnl AC_OPENMP

dnl I agree with flameeyes; majority of systems I care about are posix 99
dnl compat these days, hence, save a buncha checks.
dnl reference
dnl https://www.flameeyes.eu/autotools-mythbuster/index.html
AC_DEFINE([_POSIX_C_SOURCE],[200112L],
                     [Force full POSIX 1999 compliance])
AC_DEFINE([_XOPEN_SOURCE],[600],
          [Second part of open standards compliance.])

AC_DEFINE([_FORTIFY_SOURCE],[2],
          [set _FORTIFY_SOURCE=2, which enables the GCC stack smashing detector
           even for small buffers.])

dnl ICU defines.  This is a C application so here they are.
AC_DEFINE([U_USING_ICU_NAMESPACE],[0],
          [Disable ICU C++ namespace.])

AC_DEFINE([U_HIDE_DRAFT_API],[1],
          [ICU API 1 of 3: Parts of API not set in stone.])

AC_DEFINE([U_HIDE_INTERNAL_API],[1],
          [ICU API 2 of 3: Parts of API not intended for 
           application use.])

AC_DEFINE([U_HIDE_SYSTEM_API],[1],
          [ICU API 3 of 3: Parts of API intended only 
           for use at a system level.]) 

AC_DEFINE([UNISTR_FROM_STRING_EXPLICIT],[explicit],
          [Recommended setting from ICU documentation; stipulates 
           all conversion from char * to UNISTR must be explicit])

dnl Substitutions and output 

AC_DISABLE_FAST_INSTALL
dnl AC_DISABLE_STATIC

AC_CONFIG_FILES([doc/Makefile \
                 lib/Makefile \
                 Makefile])

AC_SUBST(ac_aux_dir)
AC_SUBST([OLY_RESOURCE],[oly_lang])
AC_DEFINE([OLY_RESOURCE],["oly_lang"], [The primary resource file for Oly.])

AC_OUTPUT

dnl this is a graceful way to ensure your messages end up appearing after all
dnl the configure tests and output.

AS_IF([test "x$enable_debug" != xyes],
      [],
      [AC_MSG_NOTICE([Developer debug mode enabled.])])


