## Makefile.am for sha_three, Keccak.
# all variables defined below.

MAINTAINERCLEANFILES 			=	Makefile.in

cflags_o64 = -fomit-frame-pointer -O3 -g0 -m64 
cflags_o32 = -fomit-frame-pointer -O3 -g0 -m32

##														libo32_hash.la
noinst_LTLIBRARIES				=	libo64_hash.la 

libo64_hash_la_SOURCES		=	$(sources_optimized_64) \
														$(headers_optimized_64)

libo64_hash_la_CFLAGS			= $(cflags_o64) $(CFLAGS)
libo64_hash_la_CPPFLAGS		= $(CPPFLAGS) -I/usr/include/x86_64-linux-gnu/

##libo32_hash_la_SOURCES		=	$(sources_optimized_32) \
##														$(headers_optimized_32)
##libo32_hash_la_CPPFLAGS		= $(CPPFLAGS) -I/usr/include/x86_64-linux-gnu/ 
##
##libo32_hash_la_CFLAGS			= $(cflags_o32) $(CFLAGS)

sources_common = \
    genKAT.c \
    KeccakDuplex.c \
    KeccakNISTInterface.c \
    KeccakSponge.c

sources_optimized = \
    $(sources_common) 

sources_optimized_32 = \
    $(sources_optimized) \
    KeccakF-1600-opt32.c

sources_optimized_64 = \
    $(sources_optimized) \
    KeccakF-1600-opt64.c

sources_simple = \
    Keccak-simple.c

sources_inplace = \
    Keccak-inplace.c 

sources_compact = \
    Keccak-compact.c 


headers_common = \
    KeccakDuplex.h \
    KeccakNISTInterface.h \
    KeccakSponge.h \
    KeccakF-1600-interface.h

headers_optimized = \
    $(headers_common) \
    timing.h \
    brg_endian.h \
    KeccakF-1600-unrolling.macros

headers_optimized_32 = \
    $(headers_optimized) \
    KeccakF-1600-opt32-settings.h \
    KeccakF-1600-32.macros \
    KeccakF-1600-32-s1.macros \
    KeccakF-1600-32-s2.macros \
    KeccakF-1600-32-rvk.macros

headers_optimized_64 = \
    $(headers_optimized) \
    KeccakF-1600-opt64-settings.h \
    KeccakF-1600-64.macros \
    KeccakF-1600-simd64.macros \
    KeccakF-1600-simd128.macros




