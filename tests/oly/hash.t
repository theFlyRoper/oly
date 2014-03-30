#! /bin/sh
#
# Test suite for hash 
# Copyright 2014 The Oly Project
#
# See LICENSE for licensing terms.

. "$SOURCE/tap/libtap.sh"
cd "${BUILD}/oly/hash"

# Run a binary, saving its output, and then compare that output to the
# corresponding *.output file.
ok_result () {
    "$2"/oly/hash/"$1" > "$1".result 2>&1
    status=$?
    ok "$1 exit status: $status" [ $status -eq "$3" ]
    diff -u "${SOURCE}/oly/hash/$1".output "$1".result 2>&1
    status=$?
    ok "$1 output" [ $status -eq 0 ]
    if [ $status -eq 0 ] ; then
        rm "$1".result
    fi
}

# Total tests.  There are two tests per row in ok_result.
plan 10

# Run the individual tests.
ok_result sh-read_charhash_from_hex "$SOURCE"  0
# ok_result sh-read_sizehash_from_hex "$SOURCE"  0
ok_result c-get_str_hashlen "$BUILD"  0
ok_result sh-get_hashbits "$SOURCE"  0
ok_result c-char_to_size "$BUILD"  0
ok_result sh-hash_char_to_hash_size "$BUILD"  0

