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

dnl ************ INITIALIZATION ******** {{{

dnl establish the necessary version for autoconf.
AC_PREREQ(2.60)

dnl initialize and provide the website for oly.
AC_INIT([Oly],[0.0.0],[bugs@fakesite.com],[oly],[http://fakesite.com])

dnl Provide a copyright substitution and ensure the copyright notice is included
dnl in the output of --version option of the generated configure script.
AC_SUBST(OLY_COPYRIGHT,["Copyright (c) 2012 The Oly Project."])
AC_COPYRIGHT([Copyright (c) 2012 The Oly Project.])

dnl Verify that the source directory is valid. This makes sure that we are
dnl configuring Oly and not some other package (it validates --srcdir argument)
AC_CONFIG_SRCDIR([src/main.c])

dnl Place all of the extra autoconf files into the config subdirectory. Tell
dnl various tools where the m4 autoconf macros are.
AC_CONFIG_AUX_DIR([ac-aux])
AC_CONFIG_MACRO_DIR([ac-aux/m4])

AC_CONFIG_TESTDIR([tests])
AC_CONFIG_LIBOBJ_DIR([replace])

AM_INIT_AUTOMAKE([1.8 subdir-objects \
                  dist-bzip2 no-dist-gzip readme-alpha])

AC_CONFIG_HEADERS([src/config.h:src/config-h.in])

dnl *****************
dnl a few AC_SUBST values to hold useful information.

dnl questions address
AC_SUBST(FAQ_ADDR,[maintainer@fakesite.com])

dnl }}}

dnl Architecture, target and host checks {{{

dnl must run early, otherwise, we won't know our host system.

AC_CANONICAL_HOST

dnl Platform check {{{
dnl Determine the platform type and cache its value.
dnl Oly includes the source code for SHA-3, optimized 
dnl for 32 bit or 64 bit systems, and hence, checks the 
dnl host value.
AC_CACHE_CHECK([our host operating system],
               [oly_cv_os_type],
[case $host in
  *-*-aix*)
    oly_cv_os_type="AIX"
    oly_cv_platform_type="Unix" ;;
  *-*-irix*)
    oly_cv_os_type="IRIX"
    oly_cv_platform_type="Unix" ;;
  *-*-cygwin*)
    oly_cv_os_type="Cygwin"
    oly_cv_platform_type="Unix" ;;
  *-*-darwin*)
    oly_cv_os_type="Darwin"
    oly_cv_platform_type="Unix" ;;
  *-*-minix*)
    oly_cv_os_type="Minix"
    oly_cv_platform_type="Unix" ;;
  *-*-freebsd* | *-*-kfreebsd-gnu)
    oly_cv_os_type="FreeBSD"
    oly_cv_platform_type="Unix" ;;
  *-*-openbsd*)
    oly_cv_os_type="OpenBSD"
    oly_cv_platform_type="Unix" ;;
  *-*-netbsd*)
    oly_cv_os_type="NetBSD"
    oly_cv_platform_type="Unix" ;;
  *-*-dragonfly*)
    oly_cv_os_type="DragonFly"
    oly_cv_platform_type="Unix" ;;
  *-*-hpux*)
    oly_cv_os_type="HP-UX"
    oly_cv_platform_type="Unix" ;;
  *-*-interix*)
    oly_cv_os_type="Interix"
    oly_cv_platform_type="Unix" ;;
  *-*-linux*)
    oly_cv_os_type="Linux"
    oly_cv_platform_type="Unix" ;;
  *-*-gnu*)
    oly_cv_os_type="GNU"
    oly_cv_platform_type="Unix" ;;
  *-*-solaris*)
    oly_cv_os_type="SunOS"
    oly_cv_platform_type="Unix" ;;
  *-*-auroraux*)
    oly_cv_os_type="AuroraUX"
    oly_cv_platform_type="Unix" ;;
  *-*-win32*)
    oly_cv_os_type="Win32"
    oly_cv_platform_type="Win32" ;;
  *-*-mingw*)
    oly_cv_os_type="MingW"
    oly_cv_platform_type="Win32" ;;
  *-*-haiku*)
    oly_cv_os_type="Haiku"
    oly_cv_platform_type="Unix" ;;
  *-unknown-eabi*)
    oly_cv_os_type="Freestanding"
    oly_cv_platform_type="Unix" ;;
  *-unknown-elf*)
    oly_cv_os_type="Freestanding"
    oly_cv_platform_type="Unix" ;;
  *)
    oly_cv_os_type="Unknown"
    oly_cv_platform_type="Unknown" ;;
esac])

dnl Make sure we aren't attempting to configure for an unknown system
if test "$oly_cv_os_type" = "Unknown" ; then
  AC_MSG_ERROR([Operating system is unknown, configure cannot continue])
fi
dnl }}}

dnl Set the "OS" Makefile variable based on the platform type so the
dnl makefile can configure itself to specific build hosts
AC_SUBST(HOST_OS,$oly_cv_os_type)

dnl Set the "OLY_ON_*" variables based on oly_cv_platform_type
dnl This is used by lib/Support to determine the basic kind of implementation
dnl to use.
case $oly_cv_platform_type in
  Unix)
    AC_DEFINE([OLY_ON_UNIX],[1],[Define if this is Unixish platform])
    AC_SUBST(OLY_ON_UNIX,[1])
    AC_SUBST(OLY_ON_WIN32,[0])
    ;;
  Win32)
    AC_DEFINE([OLY_ON_WIN32],[1],[Define if this is Win32ish platform])
    AC_SUBST(OLY_ON_UNIX,[0])
    AC_SUBST(OLY_ON_WIN32,[1])
    ;;
esac

dnl }}}

dnl command line args {{{
dnl these args have messages at the bottom, after AC_OUTPUT, because
dnl that ensures they are very obvious to maintainers and users.

dnl simple configuration level debug mode, define in makefile.am

AC_ARG_ENABLE(debug,
    [AS_HELP_STRING([--enable-debug],
                    [enables debug defines and asserts (default is no).])],
                    [enable_debug=yes],
                    [enable_debug=no])

AS_IF([test "x$enable_debug" != xyes],
      [AC_SUBST(DEV_DEF,[])],
      [AC_SUBST(DEV_DEF,["-DDEVMODE=1"])])

CFG_LOCALEDIR="${CFG_LOCALEDIR-${datadir}/locale/oly}"

AC_ARG_VAR([CFG_LOCALEDIR],[The directory to store \
            the ICU locale data for the program.  Defaults \
            to ${datadir}/locale/oly.])

dnl }}}

dnl Checks for programs. {{{

AC_PROG_CPP
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MKDIR_P

AM_PROG_CC_C_O

LT_INIT

dnl AC_PATH_PROG([PKGCONFIG],[pkg-config],[])
dnl PKG_PROG_PKG_CONFIG([0.18])

dnl }}}

dnl checks for libraries {{{

dnl adapted to oly. finds two extra variables;
dnl ICUIO_LIBS and ICULAYOUT_LIBS

OLY_CHECK_ICU([4.8],[],[])

dnl }}}

dnl checks for header files {{{
dnl AC_STDC_HEADERS
AC_HEADER_SYS_WAIT

dnl checks for ICU headers at the end; check as needed.
AC_CHECK_HEADERS([locale.h \
                  paths.h \
                  sqlite3.h \
                  unicode/udata.h \
                  unicode/ustdio.h \
                  unicode/ures.h \
                  unicode/ustring.h])

dnl }}}

dnl checks for typedefs and structures {{{
AC_TYPE_PID_T

dnl }}}

dnl checks for library functions {{{

AC_FUNC_MALLOC
AM_WITH_DMALLOC

ACX_PTHREAD

dnl OLY_WITH_READLINE

dnl AC_CHECK_FUNCS([bzero \
dnl                 memset], \
dnl                 [break])
dnl 
dnl AC_CHECK_FUNCS([calloc \
dnl                 strchr \
dnl                 strrchr \
dnl                 setlocale])
dnl 
dnl AC_REPLACE_FUNCS([basename \
dnl                   strspn \
dnl                   strcspn \
dnl                   strerror \
dnl                   strlcpy \
dnl                   strlcat \
dnl                   wcswidth \
dnl                   wcwidth \
dnl                   strsignal \
dnl                   waitpid])

dnl verify how big the system is.
AC_CHECK_SIZEOF([size_t])

AS_IF([test "x$ac_cv_sizeof_size_t" = x],
      [AC_SUBST(ac_cv_sizeof_size_t,[4])],
      [])

dnl }}}

dnl additional checks {{{

AC_C_CONST
AC_C_VOLATILE
AC_C_INLINE

dnl add later; openmp helps with multicore resource sharing.
dnl more details on use here:
dnl http://www.gnu.org/software/autoconf/manual/autoconf.html#Generic-Compiler-Characteristics
dnl (at the bottom)
dnl AC_OPENMP

dnl }}}

dnl defines {{{
dnl I agree with fireeyes; majority of systems I care about are posix 99
dnl compat these days, hence, save a buncha checks.
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

dnl }}}

dnl Substitutions and output {{{

AC_DISABLE_FAST_INSTALL
dnl AC_DISABLE_STATIC

AC_CONFIG_FILES([doc/Makefile \
                 tests/Makefile \
                 Makefile])

AC_SUBST(ac_aux_dir)

AC_OUTPUT

dnl }}}

dnl output messages {{{

dnl this is a graceful way to ensure your messages end up appearing after all
dnl the configure tests and output.

AS_IF([test "x$enable_debug" != xyes],
      [],
      [AC_MSG_NOTICE([Developer debug mode enabled.])])

AC_MSG_NOTICE([CFG_LOCALEDIR  =  $CFG_LOCALEDIR])

dnl sizeof returns the number of BYTES, as opposed to number 
dnl of bits.  Thus, if this func says 8, we're 64 bit.

dnl AC_MSG_NOTICE([sizeof found: $ac_cv_sizeof_size_t])

dnl }}}


