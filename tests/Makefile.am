## Makefile.am LICENSE (GPL 2+) {{{
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

check_PROGRAMS 					= runtests
runtests_CPPFLAGS 			= -DSOURCE='"$(abs_top_srcdir)/tests"' \
													-DBUILD='"$(abs_top_builddir)/tests"'
check_LIBRARIES 				=	tap/libtap.a
tap_libtap_a_CPPFLAGS 	= -I$(abs_top_srcdir)/tests
tap_libtap_a_SOURCES 		= tap/basic.c tap/basic.h \
													tap/float.c tap/float.h tap/macros.h


