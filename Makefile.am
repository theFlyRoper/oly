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

SUBDIRS 								= doc lib 
SUFFIXES								= .res .dat .txt
ACLOCAL_AMFLAGS 						= -I ac-aux/m4

## first i18n files, then test files.  Uses $(addsuffix <x>) later on to transform these.
MAIN_I18N								= en en_US root en_US_POSIX
TEST_I18N								= en root el ja zh en_US en_US_POSIX

MAIN_I18N_DIR						= i18n/
TEST_I18N_DIR						= tests/data/i18n/

OLY_I18N_NAME						= @OLY_RESOURCE@
RMV 								= @RMV@

I18N_TESTBUILDDIR				= $(abs_top_builddir)/tests/data/i18n/

if SHA3_64BIT
SHA3_SRC 		= lib/sha_three/KeccakF-1600-opt64.c
SHA3_CFLAGS = -fomit-frame-pointer -O3 -g0 -m64 $(CFLAGS)
else !SHA3_64BIT
SHA3_SRC = lib/sha_three/KeccakF-1600-opt32.c
SHA3_CFLAGS = -fomit-frame-pointer -O3 -g0 -m32 $(CFLAGS)
endif !SHA3_64BIT

if DEBUG_MODE
I18N_DIR_DEF = -DPKGDATADIR=\"$(I18N_TESTBUILDDIR)\"
else !DEBUG_MODE
I18N_DIR_DEF = -DPKGDATADIR=\"$(pkgdatadir)\"
endif !DEBUG_MODE
## logic for i18n is pretty complex so it is in an included .mk file.
include make_i18n.mk

EXTRA_DIST		= $(MAIN_I18N_INDEX_TXT) 	$(TEST_I18N_INDEX_TXT) \
				$(FULL_MAIN_I18N_TXT)  	$(FULL_TEST_I18N_TXT)

HEAD_CPPFLAGS						=  -I$(top_srcdir) \
									   -I$(top_srcdir)/src $(I18N_DIR_DEF)

dist_check_DATA		= tests/TESTS \
						tests/data/test_config.yaml \
						tests/oly/hash/c-char_to_size.output \
						tests/oly/hash/sh-get_hashbits.output \
						tests/oly/hash/sh-read_charhash_from_hex.output \
						tests/oly/hash/sh-read_sizehash_from_hex.output \
						tests/oly/hash/c-get_str_hashlen.output \
						tests/oly/hash/sh-hash_char_to_hash_size.output 


dist_check_SCRIPTS	= tests/tap/libtap.sh \
						tests/oly/oly.t tests/oly/i18n.t \
						tests/oly/core/sh-resex \
						tests/oly/core/sh-get_OChar_args
													
## TODO: this should use nobase_ instead 
dist_noinst_HEADERS			=	src/oly/break_rules.h \
								src/oly/common.h \
								src/oly/core.h \
								src/oly/data_source.h  \
								src/oly/state.h \
								src/oly/oly_dev.h \
								src/oly/output.h \
								src/pvt_hash.h \
								src/pvt_oly.h \
								src/pvt_resources.h \
								src/pvt_state.h \
								tests/tap/basic.h tests/tap/float.h \
								tests/tap/macros.h
AM_CFLAGS 						= -fstack-protector-all @ICU_CFLAGS@ \
							  		@DEV_CFLAGS@
csvparse_CPPFLAGS 				= $(HEAD_CPPFLAGS) @DEV_DEF@ -D_FORTIFY_SOURCE=2 
bin_PROGRAMS							= csvparse 

noinst_PROGRAMS							= resex

resex_SOURCES							= tests/tools/resex.c 

resex_LDADD 							= libcore_dev.a libcore.a \
										  @ICUIO_LIBS@ \
										  @ICU_LIBS@ 


resex_CPPFLAGS							= -D_FORTIFY_SOURCE=2 -fstack-protector-all \
										  @DEV_DEF@ $(HEAD_CPPFLAGS)

csvparse_SOURCES		= src/main.c \
							src/print_demo/print_text_range.c \
							src/print_demo/print_each_forward.c \
							src/print_demo/print_each_backward.c \
							src/print_demo/print_at.c \
							src/print_demo/print_first.c \
							src/print_demo/print_last.c \
							src/get_rules.c 

csvparse_LDADD 				=	@ICUIO_LIBS@ @ICU_LIBS@ @DEV_LIB@ \
								libcore.a 

noinst_LIBRARIES			=	libcore_dev.a libcore.a libhash.a 


libhash_a_SOURCES			=	src/hash/char_to_size.c \
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


libhash_a_CPPFLAGS				= $(HEAD_CPPFLAGS)
libhash_a_CFLAGS				= $(SHA3_CFLAGS)

## libhash_a_LIBADD					= lib/sha_three/libo64_hash.a

libcore_dev_a_CPPFLAGS			= $(HEAD_CPPFLAGS)	

libcore_a_SOURCES				=	\
								src/state/append_state_message.c \
								src/state/check_liberror.c \
								src/state/clear_liberror.c \
								src/state/clear_state_message.c \
								src/state/get_error_message.c \
								src/state/close_state.c \
								src/state/cstr_to_ostr.c \
								src/state/get_resource.c \
								src/state/get_state_message.c \
								src/state/get_status.c \
								src/state/new_state.c \
								src/state/ostr_to_cstr.c \
								src/state/open_state_resource.c \
								src/state/print_state_message.c \
								src/state/set_liberror.c \
								src/state/set_state_message.c \
								src/state/set_status.c \
								src/oly/cleanenv.c \
								src/oly/close_oly.c \
								src/oly/count_file_bytes.c \
								src/oly/init_io.c \
								src/oly/init_oly.c \
								src/oly/new_oly.c \
								src/oly/get_init_charset.c \
								src/oly/get_init_locale.c \
								src/oly/is_big_endian.c \
								src/oly/xstrdup.c \
								src/oly/xmalloc.c \
								src/oly/xstrerror.c 

libcore_a_CPPFLAGS			= $(HEAD_CPPFLAGS)	


## tests/check starts here.
CHECK_DEFS							= @DEFS@ @DEV_DEF@ -D_FORTIFY_SOURCE=2 \
									-DTEST_PKGDATADIR=\"$(I18N_TESTBUILDDIR)\" \
									-DTESTDATADIR=\"$(abs_top_srcdir)/tests/\" \
									-I$(top_srcdir)/src/ 

check_PROGRAMS 	= 	tests/runtests \
					tests/oly/state/c-init_state \
					tests/oly/hash/c-get_hashbits \
					tests/oly/hash/c-get_str_hashlen \
					tests/oly/hash/c-char_to_size \
					tests/oly/hash/c-hash_char_to_hash_size \
					tests/oly/hash/c-read_charhash_from_hex \
					tests/oly/hash/c-read_sizehash_from_hex 
tests_runtests_CPPFLAGS 		= $(CHECK_DEFS) 
check_LIBRARIES 				=	tests/tap/libtap.a
tests_tap_libtap_a_CPPFLAGS 	= $(HEAD_CPPFLAGS) -I$(abs_top_srcdir)/tests
tests_tap_libtap_a_SOURCES 		= tests/tap/basic.c tests/tap/basic.h \
								  tests/tap/float.c tests/tap/float.h \
								  tests/tap/macros.h
tests_oly_hash_c_get_hashbits_SOURCES 	= tests/oly/hash/c-get_hashbits.c 
tests_oly_hash_c_get_hashbits_CPPFLAGS = $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_get_hashbits_LDADD = tests/tap/libtap.a \
									  libcore.a libhash.a
tests_oly_hash_c_read_charhash_from_hex_SOURCES 	= tests/oly/hash/c-read_charhash_from_hex.c 
tests_oly_hash_c_read_charhash_from_hex_CPPFLAGS = $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_read_charhash_from_hex_LDADD 		= tests/tap/libtap.a \
													  libcore.a libhash.a @ICUIO_LIBS@
tests_oly_hash_c_read_sizehash_from_hex_SOURCES 	= tests/oly/hash/c-read_sizehash_from_hex.c 
tests_oly_hash_c_read_sizehash_from_hex_CPPFLAGS = $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_read_sizehash_from_hex_LDADD 		= tests/tap/libtap.a \
													  libcore.a libhash.a @ICUIO_LIBS@
tests_oly_hash_c_hash_char_to_hash_size_SOURCES 	= tests/oly/hash/c-hash_char_to_hash_size.c 
tests_oly_hash_c_hash_char_to_hash_size_CPPFLAGS = $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_hash_char_to_hash_size_LDADD 		= tests/tap/libtap.a \
													  libcore.a libhash.a @ICUIO_LIBS@
tests_oly_hash_c_get_str_hashlen_SOURCES 	= tests/oly/hash/c-get_str_hashlen.c 
tests_oly_hash_c_get_str_hashlen_CPPFLAGS = $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_get_str_hashlen_LDADD 		= tests/tap/libtap.a \
											  libcore.a libhash.a @ICUIO_LIBS@ 
tests_oly_hash_c_char_to_size_SOURCES 	= tests/oly/hash/c-char_to_size.c 
tests_oly_hash_c_char_to_size_CPPFLAGS = $(HEAD_CPPFLAGS) $(CHECK_DEFS)
tests_oly_hash_c_char_to_size_LDADD 		= tests/tap/libtap.a \
											  libcore.a libhash.a


check-local: check-i18n $(check_PROGRAMS) 
	cd tests && ./runtests -s '$(abs_top_srcdir)/tests' \
	    -b '$(abs_top_builddir)/tests' -l '$(abs_top_srcdir)/tests/TESTS'

i18n: i18n/$(OLY_I18N_NAME).dat
	
check-i18n:	tests/data/i18n/$(OLY_I18N_NAME).dat

all-local: i18n check-i18n

## end of tests/check stuff.

## maint and cleaning
CLEANFILES 					= $(MAIN_I18N_CLEAN) $(TEST_I18N_CLEAN) \
							  $(MAIN_I18N_CLEAN) $(TEST_I18N_CLEAN) \
							$(i18n_DATA) tests/data/i18n/@OLY_RESOURCE@.dat

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

