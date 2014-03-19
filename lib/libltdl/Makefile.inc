## Makefile.inc -- includable Makefile.am snippet
##
##   Copyright (C) 2003, 2004, 2005, 2007 Free Software Foundation, Inc.
##   Written by Gary V. Vaughan, 2003
##
##   NOTE: The canonical source of this file is maintained with the
##   GNU Libtool package.  Report bugs to bug-libtool@gnu.org.
##
## GNU Libltdl is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2 of the License, or (at your option) any later version.
##
## As a special exception to the GNU Lesser General Public License,
## if you distribute this file as part of a program or library that
## is built using GNU libtool, you may include this file under the
## same distribution terms that you use for the rest of that program.
##
## GNU Libltdl is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU LesserGeneral Public
## License along with GNU Libltdl; see the file COPYING.LIB.  If not, a
## copy can be downloaded from http://www.gnu.org/licenses/lgpl.html,
## or obtained by writing to the Free Software Foundation, Inc.,
## 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#####

## DO NOT REMOVE THIS LINE -- make depends on it

# -I$(srcdir) is needed for user that built libltdl with a sub-Automake
# (not as a sub-package!) using 'nostdinc':
AM_CPPFLAGS	       += -DLT_CONFIG_H='<$(LT_CONFIG_H)>' \
			  -DLTDL -I. -I$(srcdir) -Ilib/libltdl \
			  -I$(srcdir)/lib/libltdl -I$(srcdir)/lib/libltdl/libltdl
AM_LDFLAGS	       += -no-undefined
LTDL_VERSION_INFO	= -version-info 10:0:3

noinst_LTLIBRARIES	+= $(LT_DLLOADERS)

if INSTALL_LTDL
ltdlincludedir		= $(includedir)/libltdl
ltdlinclude_HEADERS	= lib/libltdl/libltdl/lt_system.h \
			  lib/libltdl/libltdl/lt_error.h \
			  lib/libltdl/libltdl/lt_dlloader.h
include_HEADERS	       += lib/libltdl/ltdl.h
lib_LTLIBRARIES	       += lib/libltdl/libltdl.la
endif

if CONVENIENCE_LTDL
noinst_LTLIBRARIES     += lib/libltdl/libltdlc.la
endif

lib_libltdl_libltdl_la_SOURCES = lib/libltdl/libltdl/lt__alloc.h \
			  lib/libltdl/libltdl/lt__dirent.h \
			  lib/libltdl/libltdl/lt__glibc.h \
			  lib/libltdl/libltdl/lt__private.h \
			  lib/libltdl/libltdl/lt__strl.h \
			  lib/libltdl/libltdl/lt_dlloader.h \
			  lib/libltdl/libltdl/lt_error.h \
	                  lib/libltdl/libltdl/lt_system.h \
			  lib/libltdl/libltdl/slist.h \
			  lib/libltdl/loaders/preopen.c \
			  lib/libltdl/lt__alloc.c \
			  lib/libltdl/lt_dlloader.c \
			  lib/libltdl/lt_error.c \
			  lib/libltdl/ltdl.c \
			  lib/libltdl/ltdl.h \
			  lib/libltdl/slist.c

EXTRA_DIST	       += lib/libltdl/lt__dirent.c \
			  lib/libltdl/lt__strl.c

lib_libltdl_libltdl_la_CPPFLAGS	= -DLTDLOPEN=$(LTDLOPEN) $(AM_CPPFLAGS)
lib_libltdl_libltdl_la_LDFLAGS	= $(AM_LDFLAGS) $(LTDL_VERSION_INFO) $(LT_DLPREOPEN)
lib_libltdl_libltdl_la_LIBADD	= $(ltdl_LTLIBOBJS)
lib_libltdl_libltdl_la_DEPENDENCIES	= $(LT_DLLOADERS) $(ltdl_LTLIBOBJS)

lib_libltdl_libltdlc_la_SOURCES	= $(lib_libltdl_libltdl_la_SOURCES)
lib_libltdl_libltdlc_la_CPPFLAGS	= -DLTDLOPEN=$(LTDLOPEN)c $(AM_CPPFLAGS)
lib_libltdl_libltdlc_la_LDFLAGS	= $(AM_LDFLAGS) $(LT_DLPREOPEN)
lib_libltdl_libltdlc_la_LIBADD	= $(lib_libltdl_libltdl_la_LIBADD)
lib_libltdl_libltdlc_la_DEPENDENCIES= $(lib_libltdl_libltdl_la_DEPENDENCIES)

## The loaders are preopened by libltdl, itself always built from
## pic-objects (either as a shared library, or a convenience library),
## so the loaders themselves must be made from pic-objects too.  We
## use convenience libraries for that purpose:
EXTRA_LTLIBRARIES	       += lib/libltdl/dlopen.la \
				  lib/libltdl/dld_link.la \
				  lib/libltdl/dyld.la \
				  lib/libltdl/load_add_on.la \
				  lib/libltdl/loadlibrary.la \
				  lib/libltdl/shl_load.la

lib_libltdl_dlopen_la_SOURCES	= lib/libltdl/loaders/dlopen.c
lib_libltdl_dlopen_la_LDFLAGS	= -module -avoid-version
lib_libltdl_dlopen_la_LIBADD 	= $(LIBADD_DLOPEN)

lib_libltdl_dld_link_la_SOURCES	= lib/libltdl/loaders/dld_link.c
lib_libltdl_dld_link_la_LDFLAGS	= -module -avoid-version
lib_libltdl_dld_link_la_LIBADD	= -ldld

lib_libltdl_dyld_la_SOURCES		= lib/libltdl/loaders/dyld.c
lib_libltdl_dyld_la_LDFLAGS		= -module -avoid-version

lib_libltdl_load_add_on_la_SOURCES	= lib/libltdl/loaders/load_add_on.c
lib_libltdl_load_add_on_la_LDFLAGS	= -module -avoid-version

lib_libltdl_loadlibrary_la_SOURCES	= lib/libltdl/loaders/loadlibrary.c
lib_libltdl_loadlibrary_la_LDFLAGS	= -module -avoid-version

lib_libltdl_shl_load_la_SOURCES	= lib/libltdl/loaders/shl_load.c
lib_libltdl_shl_load_la_LDFLAGS	= -module -avoid-version
lib_libltdl_shl_load_la_LIBADD	= $(LIBADD_SHL_LOAD)

## Make sure these will be cleaned even when they're not built by default:
CLEANFILES		       += lib/libltdl/libltdl.la \
				  lib/libltdl/libltdlc.la \
				  lib/libltdl/libdlloader.la

## Automake-1.9.6 doesn't clean subdir AC_LIBOBJ compiled objects
## automatically:
CLEANFILES	       += $(ltdl_LIBOBJS) $(ltdl_LTLIBOBJS)

EXTRA_DIST	       += lib/libltdl/COPYING.LIB \
			  lib/libltdl/README

## --------------------------- ##
## Gnulib Makefile.am snippets ##
## --------------------------- ##

BUILT_SOURCES	+= lib/libltdl/$(ARGZ_H)
EXTRA_DIST	+= lib/libltdl/argz_.h \
		   lib/libltdl/argz.c

# We need the following in order to create an <argz.h> when the system
# doesn't have one that works with the given compiler.
all-local $(lib_OBJECTS): lib/libltdl/$(ARGZ_H)
lib/libltdl/argz.h: lib/libltdl/argz_.h
	$(mkinstalldirs) . lib/libltdl/
	cp $(srcdir)/lib/libltdl/argz_.h $@-t
	mv $@-t $@
MOSTLYCLEANFILES += lib/libltdl/argz.h \
		    lib/libltdl/argz.h-t
