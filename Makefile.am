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

## may change src/sha_three later but for now, separate.
SUBDIRS 								= doc lib sha_three
SUFFIXES								= .res .dat .txt
ACLOCAL_AMFLAGS 				= -I ac-aux/m4

## first i18n files, then test files.  Uses $(addsuffix <x>) later on to transform these.
MAIN_I18N								= en root en_US_POSIX
TEST_I18N								= en root el ja zh

MAIN_I18N_DIR						= i18n/
TEST_I18N_DIR						= tests/data/i18n/

OLY_I18N_NAME						= @OLY_RESOURCE@
RMV 										= @RMV@

## logic for i18n is pretty complex so it is in an included .mk file.
include make_i18n.mk

EXTRA_DIST							= $(MAIN_I18N_INDEX_TXT) 	$(TEST_I18N_INDEX_TXT) \
													$(FULL_MAIN_I18N_TXT)  	$(FULL_TEST_I18N_TXT)

HEAD_CPPFLAGS						=  -I$(top_srcdir) -I$(top_srcdir)/src

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
													tests/oly/core/sh-get_ochar_args.output 


dist_check_SCRIPTS			= tests/tap/libtap.sh \
													tests/oly/core.t tests/oly/i18n.t \
													tests/oly/core/sh-oget_home \
													tests/oly/core/sh-oget_user_locale \
													tests/oly/core/sh-get_ochar_args
													
## TODO: this should use nobase_ instead 
dist_noinst_HEADERS			=	src/oly/break_rules.h src/oly/builtin.h \
													src/oly/common.h src/oly/core.h \
													src/oly/data_source.h  \
													src/oly/state.h src/oly/eval.h \
													src/oly/list.h src/oly/oly.h \
													src/oly/oly_dev.h \
													src/oly/output.h src/oly/syntax.h \
													tests/tap/basic.h tests/tap/float.h \
													tests/tap/macros.h
AM_CFLAGS 								= -fstack-protector-all @ICU_CFLAGS@
csvparse_CPPFLAGS 				= $(HEAD_CPPFLAGS) @DEV_DEF@ -D_FORTIFY_SOURCE=2 \
														-DLOCALEDIR=\"$(localedir)\"
bin_PROGRAMS							=	csvparse 
noinst_LIBRARIES					=	liboly_dev.a libcore.a libstate.a
liboly_dev_a_SOURCES			=	src/debug/list_icu_countries.c \
														src/debug/list_icu_langs.c \
														src/debug/list_package_locales.c 

liboly_dev_a_CPPFLAGS			= $(HEAD_CPPFLAGS)	

## state.handling is tightly coupled to i18n.
libstate_a_SOURCES				= src/state/init_oly_state.c \
														src/state/print_oly_state.c \
														src/state/flush_oly_state.c \
														src/state/log_oly_state.c \
														src/state/set_oly_state.c \
														src/state/append_oly_state.c \
														src/state/check_oly_state.c

libstate_a_CPPFLAGS				= $(HEAD_CPPFLAGS)	

libcore_a_SOURCES					=	\
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

libcore_a_CPPFLAGS				= $(HEAD_CPPFLAGS)	

csvparse_SOURCES					= src/main.c \
														src/print_demo/print_text_range.c \
														src/print_demo/print_each_forward.c \
														src/print_demo/print_each_backward.c \
														src/print_demo/print_at.c \
														src/print_demo/print_first.c \
														src/print_demo/print_last.c \
														src/get_rules.c 

csvparse_LDADD 						=	@ICUIO_LIBS@ @ICU_LIBS@ @DEV_LIB@ \
														libcore.a 

## tests/check starts here.
I18N_TESTBUILDDIR				= $(abs_top_builddir)/tests/data/i18n/

CHECK_DEFS							= @DEFS@ @DEV_DEF@ -D_FORTIFY_SOURCE=2 \
													-DTEST_LOCALEDIR=\"$(I18N_TESTBUILDDIR)\" \
													-I$(top_srcdir)/src

check_PROGRAMS 					= tests/runtests \
													tests/oly/core/c-oget_home \
													tests/oly/core/c-oget_user_locale \
													tests/oly/core/c-get_ochar_args \
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
tests_tap_libtap_a_CPPFLAGS 	= $(HEAD_CPPFLAGS) -I$(abs_top_srcdir)/tests
tests_tap_libtap_a_SOURCES 		= tests/tap/basic.c tests/tap/basic.h \
																tests/tap/float.c tests/tap/float.h \
																tests/tap/macros.h

tests_oly_core_c_oget_user_locale_SOURCES 	= tests/oly/core/c-oget_user_locale.c 
tests_oly_core_c_oget_user_locale_CPPFLAGS = $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_oget_user_locale_LDADD 		= tests/tap/libtap.a libcore.a \
																			@ICUIO_LIBS@ 
tests_oly_core_c_oget_home_SOURCES 	= tests/oly/core/c-oget_home.c 
tests_oly_core_c_oget_home_CPPFLAGS = $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_oget_home_LDADD 		= tests/tap/libtap.a libcore.a \
																			@ICUIO_LIBS@ 
tests_oly_core_c_get_ochar_args_SOURCES 	= tests/oly/core/c-get_ochar_args.c \
																							src/core/error.c src/core/xmalloc.c \
																							src/core/get_ochar_args.c
tests_oly_core_c_get_ochar_args_CPPFLAGS 	= $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_get_ochar_args_LDADD 		= tests/tap/libtap.a libcore.a \
																						@ICUIO_LIBS@ 
tests_oly_core_c_count_tokens_SOURCES 	= tests/oly/core/c-count_tokens.c \
																					src/core/count_tokens.c
tests_oly_core_c_count_tokens_CPPFLAGS 	= $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_count_tokens_LDADD 		= tests/tap/libtap.a \
																					@ICUIO_LIBS@ 
tests_oly_core_c_count_nondelim_chars_SOURCES 	= tests/oly/core/c-count_nondelim_chars.c \
																									src/core/count_nondelim_chars.c
tests_oly_core_c_count_nondelim_chars_CPPFLAGS 	= $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_count_nondelim_chars_LDADD 		= tests/tap/libtap.a \
																									@ICUIO_LIBS@ 
tests_oly_core_c_token_str_to_array_SOURCES = tests/oly/core/c-token_str_to_array.c \
																							src/core/token_str_to_array.c 
tests_oly_core_c_token_str_to_array_CPPFLAGS 	= $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_token_str_to_array_LDADD 		= tests/tap/libtap.a \
																								libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_el_SOURCES 	= tests/oly/i18n/c-el.c
tests_oly_i18n_c_el_CPPFLAGS 	= $(HEAD_CPPFLAGS) $(CHECK_DEFS) 
tests_oly_i18n_c_el_LDADD 		= tests/tap/libtap.a libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_ja_SOURCES 	= tests/oly/i18n/c-ja.c
tests_oly_i18n_c_ja_CPPFLAGS 			= $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_ja_LDADD 				= tests/tap/libtap.a libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_root_SOURCES 		= tests/oly/i18n/c-root.c 
tests_oly_i18n_c_root_CPPFLAGS 		= $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_root_LDADD 			= tests/tap/libtap.a libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_missing_resource_SOURCES 	= tests/oly/i18n/c-missing_resource.c 
tests_oly_i18n_c_missing_resource_CPPFLAGS 	= $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_missing_resource_LDADD = tests/tap/libtap.a libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_missing_lang_SOURCES 	= tests/oly/i18n/c-missing_lang.c src/core/error.c
tests_oly_i18n_c_missing_lang_CPPFLAGS = $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_missing_lang_LDADD 	= tests/tap/libtap.a libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_fallback_lang_SOURCES 	= tests/oly/i18n/c-fallback_lang.c 
tests_oly_i18n_c_fallback_lang_CPPFLAGS = $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_fallback_lang_LDADD = tests/tap/libtap.a libcore.a @ICUIO_LIBS@ 
tests_oly_i18n_c_alias_SOURCES 			= tests/oly/i18n/c-alias.c 
tests_oly_i18n_c_alias_CPPFLAGS 		= $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_i18n_c_alias_LDADD 				= tests/tap/libtap.a libcore.a @ICUIO_LIBS@ 


check-local: check-i18n $(check_PROGRAMS) 
	cd tests && ./runtests -s '$(abs_top_srcdir)/tests' \
	    -b '$(abs_top_builddir)/tests' -l '$(abs_top_srcdir)/tests/TESTS'

i18n: i18n/oly_lang.dat
	
check-i18n:	tests/data/i18n/oly_lang.dat

all-local: i18n

## end of tests/check stuff.

## maint and cleaning
CLEANFILES 					= $(MAIN_I18N_CLEAN) \
											$(TEST_I18N_CLEAN) \
											$(i18n_DATA) 

DISTCLEANFILES 			= tests/atconfig $(ALL_I18N_CLEAN)

distclean-local:
	test -z "$(MAIN_I18N_CLEAN)" || rm -f $(MAIN_I18N_CLEAN);
	test -z "$(TEST_I18N_CLEAN)" || rm -f $(TEST_I18N_CLEAN);
	test -z "$(MAIN_I18N_OUTPUT)" || rm -f $(MAIN_I18N_OUTPUT);
	test -z "$(TEST_I18N_OUTPUT)" || rm -f $(TEST_I18N_OUTPUT);

.PHONY = i18n-main-built-clean i18n-test-built-clean check-i18n i18n \
	$(MAIN_I18N_INDEX) $(MAIN_ROOT) $(MAIN_I18N_INDEX_TXT) $(MAIN_I18N_OUTPUT) \
	i18n/$(LIST_NAME) $(TEST_I18N_INDEX) $(TEST_ROOT) $(TEST_I18N_INDEX_TXT) \
	$(TEST_I18N_OUTPUT) tests/data/i18n/$(LIST_NAME)

