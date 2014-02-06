## Makefile.am LICENSE (GPL 2+) {{{
## main automake file
## Process this file with automake to produce Makefile.in
## Copyright (C) Oly Project
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
## MA 02110-1301, USA. }}}

## you might want to check out doc/inline_in_automake.html.
## it's how I was doing this a little while ago.

MAINTAINERCLEANFILES 				=	aclocal.m4 \
															configure \
															Makefile.in \
															ltmain.sh \
															deps \
															src/olyconf-h.in~

DISTCLEANFILES 							=	*~

ACLOCAL_AMFLAGS 						= -I ac-aux/m4

EXTRA_DIST									= ac-aux/config.rpath \
															ChangeLog

SUBDIRS 										= tests \
															doc 

DEFS 										= @DEFS@ @DEV_DEF@

AM_CFLAGS 							= -fstack-protector-all \
													@ICU_CFLAGS@

bin_PROGRAMS					=	csvparse

csvparse_SOURCES			= src/main.c 

noinst_HEADERS				=	src/csvparse_dev.h \
												src/csvparse_main.h

csvparse_LDADD 				=	@ICUIO_LIBS@ \
												@ICU_LIBS@ 

