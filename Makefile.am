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

ACLOCAL_AMFLAGS 						= -I ac-aux/m4

AM_CPPFLAGS =
AM_LDFLAGS =

BUILT_SOURCES =
EXTRA_DIST =
CLEANFILES =
MOSTLYCLEANFILES =

include_HEADERS =
noinst_LTLIBRARIES =
lib_LTLIBRARIES =
EXTRA_LTLIBRARIES =

include lib/libltdl/Makefile.inc

SUBDIRS 								= doc lib 
SUFFIXES								= .res .dat .txt
INCLUDES								= @INCLTDL@

## first i18n files, then test files.  Uses $(addsuffix <x>) later on to transform these.
MAIN_I18N			= en en_US root en_US_POSIX
TEST_I18N			= en root el ja zh en_US en_US_POSIX

MAIN_I18N_DIR		= i18n/
TEST_I18N_DIR		= tests/data/i18n/

OLY_I18N_NAME		= @OLY_RESOURCE@
RMV 				= @RMV@

I18N_TESTBUILDDIR	= $(abs_top_builddir)/tests/data/i18n/

TESTCONFDIR			= $(abs_top_srcdir)/tests:$(abs_top_builddir)/tests/data:$(abs_top_srcdir)/tests/data

if SHA3_64BIT
SHA3_SRC 		= lib/sha_three/KeccakF-1600-opt64.c
SHA3_CFLAGS 	= -fomit-frame-pointer -O3 -g0 -m64 $(CFLAGS)
else !SHA3_64BIT
SHA3_SRC 		= lib/sha_three/KeccakF-1600-opt32.c
SHA3_CFLAGS 	= -fomit-frame-pointer -O3 -g0 -m32 $(CFLAGS)
endif !SHA3_64BIT

DEV_CFLAGS		= -Wall -Werror -O0

if DEBUG_MODE
I18N_DIR_DEF 		= -DPKGDATADIR=\"$(I18N_TESTBUILDDIR)\"
SYSCONFDIR_DEF		= -DSYSCONFDIR=\"$(TESTCONFDIR)\" \
					  -DDATASOURCEDIR=\"$(abs_top_builddir)\"
else !DEBUG_MODE
I18N_DIR_DEF 		= -DPKGDATADIR=\"$(pkgdatadir)\"
SYSCONFDIR_DEF		= -DSYSCONFDIR=\"$(sysconfdir)/oly:$(pkgdatadir)\" \
					  -DDATASOURCEDIR=\"$(pkglibdir)\"
endif !DEBUG_MODE
   
## logic for i18n is pretty complex so it is in an included .mk file.
include make_i18n.mk

AM_CPPFLAGS	+= $(SYSCONFDIR_DEF) $(I18N_DIR_DEF) -I$(top_srcdir) -I$(top_srcdir)/src \
			   -fstack-protector-all 

EXTRA_DIST	+= $(MAIN_I18N_INDEX_TXT) $(TEST_I18N_INDEX_TXT) \
			$(FULL_MAIN_I18N_TXT) $(FULL_TEST_I18N_TXT)

					

dist_check_DATA	= tests/TESTS tests/data/olyrc \
			tests/oly/core/c-get_errmsg.output \
			tests/oly/hash/c-char_to_size.output \
			tests/oly/hash/sh-get_hashbits.output \
			tests/oly/hash/sh-read_charhash_from_hex.output \
			tests/oly/hash/sh-read_sizehash_from_hex.output \
			tests/oly/hash/c-get_str_hashlen.output \
			tests/oly/hash/sh-hash_char_to_hash_size.output 


dist_check_SCRIPTS	= tests/tap/libtap.sh tests/oly/core.t \
					tests/oly/core/sh-resex 
													
## TODO: this should use nobase_ instead 

dist_noinst_HEADERS		=	src/oly/break_rules.h src/oly/common.h \
							src/oly/core.h src/oly/data_source.h  \
							src/oly/state.h src/oly/oly_dev.h \
							src/oly/output.h src/pvt_hash.h \
							src/pvt_core.h src/pvt_resources.h \
							src/pvt_state.h tests/tap/basic.h \
							tests/tap/float.h tests/tap/macros.h
AM_CFLAGS 			= -fstack-protector-all @ICU_CFLAGS@ @GDB_FLAG@ -Wall

bin_PROGRAMS		= oly 

noinst_PROGRAMS		= resex yaml_to_sqlite 

oly_CFLAGS			= $(DEV_CFLAGS)

oly_CPPFLAGS 		= $(AM_CPPFLAGS) @DEV_DEF@ \
					  -D_FORTIFY_SOURCE=2 

oly_SOURCES			= src/main.c 
oly_LDADD 			= liboly_dev.a libcore.a libhash.a \
					  @ICUIO_LIBS@ @ICU_LIBS@ @DEV_LIB@ 
resex_CFLAGS		= $(DEV_CFLAGS)
resex_SOURCES		= tests/prototypes/resex.c 
resex_LDADD 		= liboly_dev.a libcore.a @ICUIO_LIBS@ \
					  @ICU_LIBS@ @YAML_LIBS@
resex_CPPFLAGS		= -D_FORTIFY_SOURCE=2 \
					@DEV_DEF@ $(AM_CPPFLAGS)

yaml_to_sqlite_CFLAGS		= $(DEV_CFLAGS)
yaml_to_sqlite_SOURCES		= tests/prototypes/yaml_to_sqlite.c \
							  src/data_source/boundary.c \
							  src/node/node.c
yaml_to_sqlite_LDFLAGS		= @SQLITE3_LDFLAGS@
yaml_to_sqlite_LDADD 		= liboly_dev.a libcore.a @ICUIO_LIBS@ \
							  @ICU_LIBS@ @YAML_LIBS@ 
yaml_to_sqlite_CPPFLAGS		= -D_FORTIFY_SOURCE=2 \
							  @DEV_DEF@ $(AM_CPPFLAGS)

## 						src/print_demo/print_text_range.c 
## 						src/print_demo/print_each_forward.c 
## 						src/print_demo/print_each_backward.c
## 						src/print_demo/print_at.c 
## 						src/print_demo/print_first.c 
## 						src/print_demo/print_last.c 
## 						src/get_rules.c 

noinst_LIBRARIES	=	liboly_dev.a libcore.a libhash.a libtestcore.a


libhash_a_SOURCES	=	src/hash/char_to_size.c \
						src/hash/write_hex_from_charhash.c \
						src/hash/write_hex_from_sizehash.c \
						src/hash/read_charhash_from_hex.c \
						src/hash/read_sizehash_from_hex.c \
						src/hash/get_str_hashlen.c \
						src/hash/get_hashbits.c \
						lib/sha_three/genKAT.c \
						lib/sha_three/KeccakDuplex.c \
						lib/sha_three/KeccakNISTInterface.c \
						lib/sha_three/KeccakSponge.c \
						$(SHA3_SRC)

libhash_a_CPPFLAGS				= $(AM_CPPFLAGS)
libhash_a_CFLAGS				= $(SHA3_CFLAGS)

## libhash_a_LIBADD					= lib/sha_three/libo64_hash.a

liboly_dev_a_SOURCES	=	src/debug/list_icu_countries.c \
							src/debug/bash_printing.c \
						src/debug/list_table_resources.c \
						src/debug/list_array_resources.c \
						src/debug/level_indent.c \
						src/debug/flag_res_display.c \
						src/debug/init_res_disp_flag.c \
						src/debug/flag_check.c \
						src/debug/get_resource_type.c \
						src/debug/list_icu_langs.c \
						src/debug/list_package_locales.c 

liboly_dev_a_CPPFLAGS	= $(AM_CPPFLAGS)	
liboly_dev_a_CFLAGS		= $(DEV_CFLAGS)

libcore_a_SOURCES	=	src/resources/get_resource_data.c \
						src/resources/open_resource.c \
						src/resources/close_resource.c \
						src/resources/new_resource.c \
						src/resources/get_charset.c \
						src/resources/get_locale.c \
						src/core/basic_functions.c \
						src/core/dispatch.c \
						src/core/config.c \
						src/node/node_queue.c \
						src/node/node.c \
						src/core/string_buffer.c \
						src/core/get_errmsg.c \
						src/core/init_oly.c \
						src/core/oly_mem_string.c \
						src/core/olystate.c \
						src/data_source/data_source.c \
						src/data_source/get_ltdl_errmsg.c 

##					src/core/config.c \

libcore_a_CPPFLAGS			= $(AM_CPPFLAGS)	
libcore_a_CFLAGS			= $(DEV_CFLAGS) 

## tests/check starts here.
CHECK_DEFS	= @DEFS@ @DEV_DEF@ \
			-DTESTING=1 -DTEST_PKGDATADIR=\"$(I18N_TESTBUILDDIR)\" \
			-DTESTDATADIR=\"$(abs_top_srcdir)/tests/\" \
			-I$(top_srcdir)/src/ $(SYSCONFDIR_DEF)

libtestcore_a_SOURCES		= $(libcore_a_SOURCES)
libtestcore_a_CPPFLAGS		= $(CHECK_DEFS) 

## tests/oly/core/c-oly_config \

check_PROGRAMS 	= 	tests/runtests \
					tests/oly/core/c-get_errmsg \
					tests/oly/core/c-basic_functions \
					tests/oly/core/c-string_buffer \
					tests/oly/data_source/c-oly_yaml \
					tests/oly/hash/c-read_sizehash_from_hex \
					tests/oly/hash/c-get_hashbits \
 					tests/oly/hash/c-get_str_hashlen \
 					tests/oly/hash/c-char_to_size \
 					tests/oly/hash/c-hash_char_to_hash_size \
 					tests/oly/hash/c-read_charhash_from_hex 

tests_runtests_CPPFLAGS 		= $(CHECK_DEFS) 
check_LIBRARIES 				=	tests/tap/libtap.a
tests_tap_libtap_a_CPPFLAGS 	= $(AM_CPPFLAGS) -I$(abs_top_srcdir)/tests 
## modifications to tap - added is_unicode_string.
tests_tap_libtap_a_SOURCES 		= tests/tap/basic.c tests/tap/basic.h \
								  tests/tap/is_unicode_string.h \
								  tests/tap/float.c tests/tap/float.h \
								  tests/tap/macros.h

tests_oly_hash_c_get_hashbits_SOURCES 	= tests/oly/hash/c-get_hashbits.c 
tests_oly_hash_c_get_hashbits_CPPFLAGS = $(AM_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_get_hashbits_LDADD = tests/tap/libtap.a \
									libtestcore.a libhash.a \
									@YAML_LIBS@ @LIBLTDL@
tests_oly_hash_c_read_charhash_from_hex_SOURCES  = tests/oly/hash/c-read_charhash_from_hex.c 
tests_oly_hash_c_read_charhash_from_hex_CPPFLAGS = $(AM_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_read_charhash_from_hex_LDADD 	= tests/tap/libtap.a \
						libtestcore.a libhash.a @ICUIO_LIBS@
tests_oly_hash_c_read_sizehash_from_hex_SOURCES = tests/oly/hash/c-read_sizehash_from_hex.c 
tests_oly_hash_c_read_sizehash_from_hex_CPPFLAGS = $(AM_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_read_sizehash_from_hex_LDADD = tests/tap/libtap.a \
								libtestcore.a libhash.a @ICUIO_LIBS@
tests_oly_hash_c_hash_char_to_hash_size_SOURCES = tests/oly/hash/c-hash_char_to_hash_size.c 
tests_oly_hash_c_hash_char_to_hash_size_CPPFLAGS = $(AM_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_hash_char_to_hash_size_LDADD 	= tests/tap/libtap.a libtestcore.a \
												  libhash.a @ICUIO_LIBS@
tests_oly_hash_c_get_str_hashlen_SOURCES = tests/oly/hash/c-get_str_hashlen.c 
tests_oly_hash_c_get_str_hashlen_CPPFLAGS = $(AM_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_get_str_hashlen_LDADD 	= tests/tap/libtap.a libtestcore.a \
										  libhash.a @ICUIO_LIBS@ 
tests_oly_hash_c_char_to_size_SOURCES 	= tests/oly/hash/c-char_to_size.c 
tests_oly_hash_c_char_to_size_CPPFLAGS 	= $(AM_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_char_to_size_LDADD 	= 	tests/tap/libtap.a \
											libtestcore.a libhash.a @ICUIO_LIBS@
tests_oly_core_c_get_errmsg_SOURCES 	= tests/oly/core/c-get_errmsg.c 
tests_oly_core_c_get_errmsg_CPPFLAGS 	= $(AM_CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_get_errmsg_LDADD 		= tests/tap/libtap.a \
									  	  libtestcore.a @ICUIO_LIBS@ @ICU_LIBS@ 

tests_oly_data_source_c_oly_yaml_SOURCES = tests/oly/data_source/c-oly_yaml.c \
									  src/core/oly_mem_string.c \
									  src/debug/bash_printing.c 
tests_oly_data_source_c_oly_yaml_CPPFLAGS = $(AM_CPPFLAGS) $(CHECK_DEFS)
tests_oly_data_source_c_oly_yaml_LDADD 	= libtestcore.a @ICUIO_LIBS@ @ICU_LIBS@ \
									  tests/tap/libtap.a @YAML_LIBS@ 

tests_oly_core_c_basic_functions_SOURCES = tests/oly/core/c-basic_functions.c 
tests_oly_core_c_basic_functions_CPPFLAGS = $(AM_CPPFLAGS) $(CHECK_DEFS)
tests_oly_core_c_basic_functions_CFLAGS = $(CFLAGS) -Wall -Werror -pedantic \
										  -ansi -std=gnu99 -Werror=format-security
tests_oly_core_c_basic_functions_LDADD 	= libtestcore.a tests/tap/libtap.a \
										  @ICUIO_LIBS@ @ICU_LIBS@ 
tests_oly_core_c_basic_functions_LDFLAGS = -lm


tests_oly_core_c_string_buffer_SOURCES = tests/oly/core/c-string_buffer.c \
										 src/core/string_buffer.c \
										 src/debug/bash_printing.c 
tests_oly_core_c_string_buffer_CPPFLAGS = $(AM_CPPFLAGS) $(CHECK_DEFS) 
##										  -DDEBUG_STRING_BUFFER=1
## 										  this def adds the debug functions.
##										  They are not needed for the test.
tests_oly_core_c_string_buffer_CFLAGS = $(CFLAGS) -Wall -Werror -pedantic \
										  -ansi -std=gnu99 -Werror=format-security 
tests_oly_core_c_string_buffer_LDADD 	= tests/tap/libtap.a libtestcore.a \
										  @ICUIO_LIBS@ @ICU_LIBS@ 

## tests_oly_core_c_oly_config_SOURCES = tests/oly/core/c-oly_config.c \
## 									  src/core/oly_mem_string.c \
## 									  src/debug/bash_printing.c 
## tests_oly_core_c_oly_config_CPPFLAGS = $(AM_CPPFLAGS) $(CHECK_DEFS)
## tests_oly_core_c_oly_config_LDADD 	= libtestcore.a @ICUIO_LIBS@ @ICU_LIBS@ \
## 									  tests/tap/libtap.a @YAML_LIBS@ 
## 

check-local: check-i18n $(check_PROGRAMS) 
	cd tests && ./runtests -s '$(abs_top_srcdir)/tests' \
	    -b '$(abs_top_builddir)/tests' -l '$(abs_top_srcdir)/tests/TESTS'

i18n: 		i18n/$(OLY_I18N_NAME).dat
	
check-i18n:	tests/data/i18n/$(OLY_I18N_NAME).dat

all-local:  i18n check-i18n


## end of tests/check stuff.

## maint and cleaning
CLEANFILES 					+= $(MAIN_I18N_CLEAN) $(TEST_I18N_CLEAN) \
							  $(MAIN_I18N_CLEAN) $(TEST_I18N_CLEAN) \
							$(i18n_DATA) tests/data/i18n/@OLY_RESOURCE@.dat \
							tests/data/test_sqlite.sqlite3

DISTCLEANFILES 			= tests/atconfig 

distclean-local:
	test -z "$(MAIN_I18N_CLEAN)" || rm -f $(MAIN_I18N_CLEAN);
	test -z "$(TEST_I18N_CLEAN)" || rm -f $(TEST_I18N_CLEAN);
	test -z "$(MAIN_I18N_OUTPUT)" || rm -f $(MAIN_I18N_OUTPUT);
	test -z "$(TEST_I18N_OUTPUT)" || rm -f $(TEST_I18N_OUTPUT);

.PHONY = i18n-main-built-clean i18n-test-built-clean check-i18n i18n \
	$(MAIN_I18N_INDEX) $(MAIN_ROOT) $(MAIN_I18N_INDEX_TXT) $(MAIN_I18N_OUTPUT) \
	i18n/$(LIST_NAME) $(TEST_I18N_INDEX) $(TEST_ROOT) $(TEST_I18N_INDEX_TXT) \
	$(TEST_I18N_OUTPUT) tests/data/i18n/$(LIST_NAME)

