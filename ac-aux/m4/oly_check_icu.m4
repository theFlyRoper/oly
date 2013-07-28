dnl ===================================================================
dnl       http://www.gnu.org/software/autoconf-archive/ax_check_icu.html
dnl ===================================================================
dnl
dnl SYNOPSIS
dnl
dnl   AX_CHECK_ICU(version, action-if, action-if-not)
dnl
dnl DESCRIPTION
dnl
dnl   Defines ICU_LIBS, ICU_CFLAGS, ICU_CXXFLAGS. See icu-config(1) man page.
dnl
dnl LICENSE
dnl
dnl   Copyright (c) 2008 Akos Maroy <darkeye@tyrell.hu>,
dnl             (c) 2012 Oly Project (small modifications to meet
dnl                                   oly needs).
dnl
dnl
dnl   Copying and distribution of this file, with or without modification, are
dnl   permitted in any medium without royalty provided the copyright notice
dnl   and this notice are preserved. This file is offered as-is, without any
dnl   warranty.

dnl serial 6

AU_ALIAS([OLY_CHECK_ICU], [AX_CHECK_ICU])
AU_ALIAS([AC_CHECK_ICU], [AX_CHECK_ICU])
AC_DEFUN([AX_CHECK_ICU], [
  succeeded=no

  if test -z "$ICU_CONFIG"; then
    AC_PATH_PROG(ICU_CONFIG, icu-config, no)
  fi

  if test "$ICU_CONFIG" = "no" ; then
    echo "*** The icu-config script could not be found. Make sure it is"
    echo "*** in your path, and that taglib is properly installed."
    echo "*** Or see http://ibm.com/software/globalization/icu/"
  else
    ICU_VERSION=`$ICU_CONFIG --version`
    AC_MSG_CHECKING(for ICU >= $1)
        VERSION_CHECK=`expr $ICU_VERSION \>\= $1`
        if test "$VERSION_CHECK" = "1" ; then
            AC_MSG_RESULT(yes)
            succeeded=yes
            
            AC_MSG_CHECKING(ICU_CFLAGS)
            ICU_CFLAGS=`$ICU_CONFIG --cflags`
            AC_MSG_RESULT($ICU_CFLAGS)

            AC_MSG_CHECKING(ICU_CXXFLAGS)
            ICU_CXXFLAGS=`$ICU_CONFIG --cxxflags`
            AC_MSG_RESULT($ICU_CXXFLAGS)
            
            AC_MSG_CHECKING(ICU_SUBDIR)
            ICU_SUBDIR=`$ICU_CONFIG --cppflags-searchpath`
            AC_MSG_RESULT($ICU_SUBDIR)
            
            AC_MSG_CHECKING(ICUIO_LIBS)
            ICUIO_LIBS=`$ICU_CONFIG --ldflags-icuio`
            AC_MSG_RESULT($ICUIO_LIBS)
            
            AC_MSG_CHECKING(ICULAYOUT_LIBS)
            ICULAYOUT_LIBS=`$ICU_CONFIG --ldflags-layout`
            AC_MSG_RESULT($ICULAYOUT_LIBS)

            AC_MSG_CHECKING(ICU_LIBS)
            ICU_LIBS=`$ICU_CONFIG --ldflags`
            AC_MSG_RESULT($ICU_LIBS)
        else
            ICU_CFLAGS=""
            ICU_CXXFLAGS=""
            ICU_LIBS=""
            ICU_SUBDIR=""
            ICULAYOUT_LIBS=""
            ICUIO_LIBS=""
            ## If we have a custom action on failure, don't print errors, but
            ## do set a variable so people can do so.
            ifelse([$3], ,echo "can't find ICU >= $1",)
        fi

        AC_SUBST(ICU_CPPFLAGS)
        AC_SUBST(ICU_CFLAGS)
        AC_SUBST(ICU_CXXFLAGS)
        AC_SUBST(ICULAYOUT_LIBS)
        AC_SUBST(ICUIO_LIBS)
        AC_SUBST(ICU_LIBS)
  fi

  if test $succeeded = yes; then
     ifelse([$2], , :, [$2])
  else
     ifelse([$3], , AC_MSG_ERROR([Library requirements (ICU) not met.]), [$3])
  fi
])
