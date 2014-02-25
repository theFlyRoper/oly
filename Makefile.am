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

SUFFIXES									= .res .dat .txt
SUBDIRS 								= doc lib 
ACLOCAL_AMFLAGS 				= -I ac-aux/m4

AM_CPPFLAGS							= -I$(top_srcdir) -I$(top_srcdir)/src
BUILT_SOURCES						= i18n/res_index.txt tests/data/i18n/res_index.txt

## first i18n files, then test files.
MAIN_I18N								= i18n/en.txt i18n/root.txt i18n/el.txt i18n/ja.txt i18n/zh.txt 
EXTRA_DIST							= $(MAIN_I18N) \
													tests/data/i18n/en.txt tests/data/i18n/root.txt \
													tests/data/i18n/el.txt tests/data/i18n/ja.txt

dist_check_DATA					= tests/TESTS \
													tests/oly/i18n/c-alias.output \
													tests/oly/i18n/c-el.output \
													tests/oly/i18n/c-fallback_lang.output \
													tests/oly/i18n/c-ja.output \
													tests/oly/i18n/c-missing_lang.output \
													tests/oly/i18n/c-missing_resource.output \
													tests/oly/i18n/c-root.output \
													tests/oly/core/c-count_nondelim_chars.output \
													tests/oly/core/c-count_tokens.output \
													tests/oly/core/c-token_str_to_array.output \
													tests/oly/core/sh-oget_home.output \
													tests/oly/core/sh-oget_user_locale.output \
													tests/oly/core/sh-oly_getOCharArgs.output 


dist_check_SCRIPTS			= tests/tap/libtap.sh \
													tests/oly/core.t tests/oly/i18n.t \
													tests/oly/core/sh-oget_home \
													tests/oly/core/sh-oget_user_locale \
													tests/oly/core/sh-oly_getOCharArgs
													
## TODO: this should use nobase_ instead 
dist_noinst_HEADERS			=	src/oly/break_rules.h src/oly/builtin.h \
													src/oly/common.h src/oly/core.h \
													src/oly/data_source.h  \
													src/oly/error.h src/oly/eval.h \
													src/oly/list.h src/oly/oly.h \
													src/oly/oly_dev.h \
													src/oly/output.h src/oly/syntax.h \
													tests/tap/basic.h tests/tap/float.h \
													tests/tap/macros.h
AM_CFLAGS 								= -fstack-protector-all @ICU_CFLAGS@
csvparse_CPPFLAGS 				= $(AM_CPPFLAGS) @DEV_DEF@ -D_FORTIFY_SOURCE=2 \
														-DLOCALEDIR=\"$(localedir)\"
bin_PROGRAMS							=	csvparse 
noinst_LIBRARIES					=	src/liboly_dev.a src/libcore.a src/libstate.a
src_liboly_dev_a_SOURCES	=	src/debug/list_icu_countries.c \
														src/debug/list_icu_langs.c \
														src/debug/list_package_locales.c 

## error handling is tightly coupled to i18n.
src_libstate_a_SOURCES		= src/state/init_oly_state.c \
														src/state/print_oly_state.c \
														src/state/flush_oly_state.c \
														src/state/log_oly_state.c \
														src/state/set_oly_state.c \
														src/state/append_oly_state.c \
														src/state/check_oly_state.c

src_libcore_a_SOURCES			=	\
														src/core/builtin.c \
														src/core/cleanenv.c \
														src/core/clean_io_open.c \
														src/core/close_oly.c \
														src/core/count_tokens.c \
														src/core/count_nondelim_chars.c \
														src/core/error.c \
														src/core/eval.c \
														src/core/init_all.c \
														src/core/init_io.c \
														src/core/list.c \
														src/core/oget_home.c \
														src/core/oget_user_locale.c \
														src/core/oly.c \
														src/core/syntax.c \
													  src/core/token_str_to_array.c \
														src/core/xstrdup.c \
														src/core/xmalloc.c \
														src/core/xstrerror.c 

csvparse_SOURCES					= src/main.c \
														src/print_demo/print_text_range.c \
														src/print_demo/print_each_forward.c \
														src/print_demo/print_each_backward.c \
														src/print_demo/print_at.c \
														src/print_demo/print_first.c \
														src/print_demo/print_last.c \
														src/get_rules.c 

csvparse_LDADD 						=	@ICUIO_LIBS@ @ICU_LIBS@ @DEV_LIB@ \
														src/libcore.a 

## i18n, both for tests and the main library.
OLY_I18N_NAME							= @OLY_RESOURCE@

i18ndir										= $(datadir)/i18n/

i18n_DATA									= i18n/oly_lang.dat tests/data/i18n/oly_lang.dat

i18n_oly_lang_dat_RES			= i18n/en.res i18n/root.res i18n/el.res i18n/ja.res i18n/zh.res
tests_data_i18n_oly_lang_dat_RES = tests/data/i18n/en.res tests/data/i18n/root.res \
																	 tests/data/i18n/el.res tests/data/i18n/ja.res 
i18n_oly_lang_dat_PREFLIGHT	= $(i18n_oly_lang_dat_RES) i18n/res_index.txt \
															i18n/res_list.txt i18n/res_index.res
														
tests_data_i18n_oly_lang_dat_PREFLIGHT =  $(tests_data_i18n_oly_lang_dat_RES) \
																			tests/data/i18n/res_list.txt \
																			tests/data/i18n/res_index.txt \
																			tests/data/i18n/res_index.res
# pkgdata version.
PKGDATAOPTS							=	-s $(abs_top_builddir) --name $(OLY_I18N_NAME) \
													--tempdir .i18ndeps -v 
GENRBOPT								= -s $(abs_top_srcdir) 

## this worked before. Shrug.
i18n/root.res: i18n/res_index.txt
	@echo "generating $@"
	SRCDIR_VAL=`echo "$(top_srcdir)" | $(SED) -e 's@[\.\*]@\\\\&@g'`; \
		$(GENRB) $(GENRBOPT) -d `echo "$^" | $(SED) -e 's@/[^/]*$$@@g' \
		-e "s@^$$SRCDIR_VAL@$(abs_top_builddir)@g"` $(MAIN_I18N) 

.txt.res: 
	@echo "generating $@"; \
	SRCDIR_VAL=`echo "$(top_srcdir)" | $(SED) -e 's@[\.\*]@\\\\&@g'`; \
	$(GENRB) $(GENRBOPT) $^ -d `echo "$^" | $(SED) -e 's@/[^/]*$$@@g' \
		-e "s@^$$SRCDIR_VAL@$(abs_top_builddir)@g"`

## if the build_index breaks again this is useful for looking at the vars.
## echo "ITEMS: $$ITEMS FILELIST: $$filelist INDEX_NAME: $$INDEX_NAME"; 
define build_index
$(RMV) -f $@; 
@echo "generating $@ (list of installed language name locales)"; \
INDEX_NAME=`echo "$@" | $(SED) -e 's@.*/@@g' -e 's@\..*@@g'`; \
filelist=`echo "$@" | $(SED) 's@/[^/]*$$@@g'`; \
ITEMS=`ls $$filelist/*.txt | $(SED) -e 's@.*/@@g' -e 's@\..*@@g'`; \
echo "// Warning this file is automatically generated" > $@; \
echo "$$INDEX_NAME:table(nofallback) {" >> $@; \
echo "    InstalledLocales {" >> $@; \
for file in $$ITEMS; do \
	echo "        $$file {\"\"}" >> $@; \
done; \
echo "    }" >> $@; \
echo "}" >> $@;
endef

i18n/res_index.txt:
	$(build_index)

tests/data/i18n/res_index.txt:
	$(build_index)

i18n/res_list.txt: i18n/root.res i18n/res_index.res
	ls i18n/*.res > $@

tests/data/i18n/res_list.txt: $(tests_data_i18n_oly_lang_dat_RES) tests/data/i18n/res_index.res
	ls tests/data/i18n/*.res > $@

i18n/oly_lang.dat: i18n/res_list.txt
	$(PKGDATA) --mode common $(PKGDATAOPTS) \
		-d i18n i18n/res_list.txt  

tests/data/i18n/oly_lang.dat: $(tests_data_i18n_oly_lang_dat_PREFLIGHT) 
	$(PKGDATA) --mode common $(PKGDATAOPTS) \
		-d `echo "$@" | $(SED) 's@/[^/]*$$@@g'` tests/data/i18n/res_list.txt

# end of i18n.

## tests/check starts here.
I18N_TESTBUILDDIR				= $(abs_top_builddir)/tests/data/i18n
CHECK_DEFS							= @DEFS@ @DEV_DEF@ -D_FORTIFY_SOURCE=2 \
													-DTEST_LOCALEDIR=\"$(I18N_TESTBUILDDIR)\" \
													-I$(top_srcdir)/src

check_PROGRAMS 					= tests/runtests \
													tests/oly/core/c-oget_home \
													tests/oly/core/c-oget_user_locale \
													tests/oly/core/c-oly_getOCharArgs \
													tests/oly/core/c-count_tokens \
													tests/oly/core/c-count_nondelim_chars \
													tests/oly/core/c-token_str_to_array \
													tests/oly/i18n/c-alias \
													tests/oly/i18n/c-ja \
													tests/oly/i18n/c-el \
													tests/oly/i18n/c-root \
													tests/oly/i18n/c-missing_resource \
													tests/oly/i18n/c-missing_lang \
													tests/oly/i18n/c-fallback_lang 
tests_runtests_CPPFLAGS 			= $(CHECK_DEFS) 
check_LIBRARIES 							=	tests/tap/libtap.a
tests_tap_libtap_a_CPPFLAGS 	= $(CPPFLAGS) -I$(abs_top_srcdir)/tests
tests_tap_libtap_a_SOURCES 		= tests/tap/basic.c tests/tap/basic.h \
																tests/tap/float.c tests/tap/float.h \
																tests/tap/macros.h

tests_oly_core_c_oget_user_locale_SOURCES 	= tests/oly/core/c-oget_user_locale.c 
tests_oly_core_c_oget_user_locale_CPPFLAGS = $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_oget_user_locale_LDADD 		= tests/tap/libtap.a src/libcore.a \
																			@ICUIO_LIBS@ 
tests_oly_core_c_oget_home_SOURCES 	= tests/oly/core/c-oget_home.c 
tests_oly_core_c_oget_home_CPPFLAGS = $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_oget_home_LDADD 		= tests/tap/libtap.a src/libcore.a \
																			@ICUIO_LIBS@ 
tests_oly_core_c_oly_getOCharArgs_SOURCES 	= tests/oly/core/c-oly_getOCharArgs.c \
																							src/core/error.c src/core/xmalloc.c \
																							src/core/oly_getOCharArgs.c
tests_oly_core_c_oly_getOCharArgs_CPPFLAGS 	= $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_oly_getOCharArgs_LDADD 		= tests/tap/libtap.a src/libcore.a \
																						@ICUIO_LIBS@ 
tests_oly_core_c_count_tokens_SOURCES 	= tests/oly/core/c-count_tokens.c \
																					src/core/count_tokens.c
tests_oly_core_c_count_tokens_CPPFLAGS 	= $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_count_tokens_LDADD 		= tests/tap/libtap.a \
																					@ICUIO_LIBS@ 
tests_oly_core_c_count_nondelim_chars_SOURCES 	= tests/oly/core/c-count_nondelim_chars.c \
																									src/core/count_nondelim_chars.c
tests_oly_core_c_count_nondelim_chars_CPPFLAGS 	= $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_count_nondelim_chars_LDADD 		= tests/tap/libtap.a \
																									@ICUIO_LIBS@ 
tests_oly_core_c_token_str_to_array_SOURCES = tests/oly/core/c-token_str_to_array.c \
																							src/core/token_str_to_array.c 
tests_oly_core_c_token_str_to_array_CPPFLAGS 	= $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_token_str_to_array_LDADD 		= tests/tap/libtap.a \
																								src/libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_el_SOURCES 	= tests/oly/i18n/c-el.c
tests_oly_i18n_c_el_CPPFLAGS 	= $(CPPFLAGS) $(CHECK_DEFS) 
tests_oly_i18n_c_el_LDADD 		= tests/tap/libtap.a src/libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_ja_SOURCES 	= tests/oly/i18n/c-ja.c
tests_oly_i18n_c_ja_CPPFLAGS 			= $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_ja_LDADD 				= tests/tap/libtap.a src/libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_root_SOURCES 		= tests/oly/i18n/c-root.c 
tests_oly_i18n_c_root_CPPFLAGS 		= $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_root_LDADD 			= tests/tap/libtap.a src/libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_missing_resource_SOURCES 	= tests/oly/i18n/c-missing_resource.c 
tests_oly_i18n_c_missing_resource_CPPFLAGS 	= $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_missing_resource_LDADD = tests/tap/libtap.a src/libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_missing_lang_SOURCES 	= tests/oly/i18n/c-missing_lang.c src/core/error.c
tests_oly_i18n_c_missing_lang_CPPFLAGS = $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_missing_lang_LDADD 	= tests/tap/libtap.a src/libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_fallback_lang_SOURCES 	= tests/oly/i18n/c-fallback_lang.c 
tests_oly_i18n_c_fallback_lang_CPPFLAGS = $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_fallback_lang_LDADD = tests/tap/libtap.a src/libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_alias_SOURCES 			= tests/oly/i18n/c-alias.c 
tests_oly_i18n_c_alias_CPPFLAGS 		= $(CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_alias_LDADD 				= tests/tap/libtap.a src/libcore.a @ICUIO_LIBS@ 


check-local: $(check_PROGRAMS)
	cd tests && ./runtests -s '$(abs_top_srcdir)/tests' \
	    -b '$(abs_top_builddir)/tests' -l '$(abs_top_srcdir)/tests/TESTS'

## end of tests/check stuff.

## maint and cleaning
CLEANFILES 					= $(i18n_oly_lang_dat_PREFLIGHT) \
											$(tests_data_i18n_oly_lang_dat_PREFLIGHT) \
											$(i18n_DATA) 

DISTCLEANFILES 			= tests/atconfig

