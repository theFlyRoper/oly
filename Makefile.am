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

MAINTAINERCLEANFILES 		=	.deps \
													deps \
													src/olyconf-h.in~

.PHONY: atool-clean 

atool-clean: distclean 
	@echo "atool-clean is a special command that clears out autotool detritus."
	rm -f resources/Makefile.in
	rm -f tests/Makefile.in
	rm -f doc/Makefile.in
	rm -rf autom4te.cache .deps
	rm -rf aclocal.m4 Makefile.in configure  src/olyconf-h.in~ tests/atconfig


DISTCLEANFILES 					=	*~ src/*~

ACLOCAL_AMFLAGS 				= -I ac-aux/m4

EXTRA_DIST							= ChangeLog tests/oly/loader/sh-oget_home tests/oly/loader/sh-oget_home.output 

SUBDIRS 								= doc \
													replace \
													resources

datadir									= @datadir@
datarootdir							= @datarootdir@
localedir								= @localedir@

noinst_HEADERS					=	src/error.h \
													src/loader.h \
													src/output.h \
													src/common.h

AM_CFLAGS 							= -fstack-protector-all \
													@ICU_CFLAGS@

RESDIR									=	$(top_srcdir)/resources

RESNAME									=	oly_lang
RESLDFLAGS							= 

csvparse_CPPFLAGS 			= @DEFS@ @DEV_DEF@ -DLOCALEDIR=\"$(localedir)\"

bin_PROGRAMS						=	csvparse 

noinst_LIBRARIES				=	src/libloader.a src/liboly.a \
													src/liboly_dev.a

src_liboly_dev_a_SOURCES	=	src/debug/list_icu_countries.c \
														src/debug/list_icu_langs.c \
														src/debug/list_package_locales.c 

src_libloader_a_SOURCES		=	src/loader/init_io.c \
														src/loader/oget_home.c \
														src/loader/oget_user_locale.c \
														src/loader/close_oly.c \
														src/loader/init_all.c \
														src/loader/cleanenv.c 

src_liboly_a_SOURCES		=	src/xmalloc.c \
													src/xstrdup.c \
													src/xstrerror.c \
													src/oly.c \
													src/builtin.c \
													src/eval.c \
													src/syntax.c \
													src/error.c

csvparse_SOURCES				= src/main.c \
													src/print_demo/print_text_range.c \
													src/print_demo/print_each_forward.c \
													src/print_demo/print_each_backward.c \
													src/print_demo/print_at.c \
													src/print_demo/print_first.c \
													src/print_demo/print_last.c \
													src/get_rules.c 

csvparse_LDADD 					=	@ICUIO_LIBS@ @ICU_LIBS@ @DEV_LIB@ \
													src/libloader.a src/liboly.a 

## csvparse_LDFLAGS				= -L$(RESDIR) -l$(RESNAME) 

check_PROGRAMS 								= tests/runtests \
																tests/oly/loader/c-oget_home \
																tests/oly/resources/c-ja \
																tests/oly/resources/c-root \
																tests/oly/resources/c-el_gr 
tests_runtests_CPPFLAGS 			= -DSOURCE='"$(abs_top_srcdir)"' \
																-DBUILD='"$(abs_top_builddir)"'
check_LIBRARIES 							=	tests/tap/libtap.a
tests_tap_libtap_a_CPPFLAGS 	= -I$(abs_top_srcdir)/tests
tests_tap_libtap_a_SOURCES 		= tests/tap/basic.c tests/tap/basic.h \
																tests/tap/float.c tests/tap/float.h \
																tests/tap/macros.h

tests_oly_loader_c_oget_home_SOURCES 	= tests/oly/loader/c-oget_home.c 
tests_oly_loader_c_oget_home_LDADD 		= tests/tap/libtap.a src/libloader.a src/liboly.a \
																				@ICUIO_LIBS@ 
tests_oly_resources_c_el_gr_SOURCES 	= tests/oly/resources/c-el_gr.c src/error.c
tests_oly_resources_c_el_gr_CPPFLAGS 	= @DEFS@ @DEV_DEF@ \
																				-DLOCALEDIR=\"$(abs_top_builddir)/resources\" 
tests_oly_resources_c_el_gr_LDADD 		= tests/tap/libtap.a src/libloader.a \
																				@ICUIO_LIBS@ 
tests_oly_resources_c_ja_SOURCES 			= tests/oly/resources/c-ja.c src/error.c
tests_oly_resources_c_ja_CPPFLAGS 		= @DEFS@ @DEV_DEF@ \
																				-DLOCALEDIR=\"$(abs_top_builddir)/resources\" 
tests_oly_resources_c_ja_LDADD 				= tests/tap/libtap.a src/libloader.a \
																				@ICUIO_LIBS@ 



check-local: $(check_PROGRAMS)
	cd tests && ./runtests -s '$(abs_top_srcdir)/tests' \
	    -b '$(abs_top_builddir)/tests' -l '$(abs_top_srcdir)/tests/TESTS'

