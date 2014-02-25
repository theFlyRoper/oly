#! /bin/sh
#
# Test suite for core 
# Copyright 2014 The Oly Project
#
# See LICENSE for licensing terms.

. "$SOURCE/tap/libtap.sh"
cd "${BUILD}/oly/core"

# Run a binary, saving its output, and then compare that output to the
# corresponding *.output file.
ok_result () {
    "$2"/oly/core/"$1" > "$1".result 2>&1
    status=$?
    ok "$1 exit status: $status" [ $status -eq "$3" ]
    diff -u "${SOURCE}/oly/core/$1".output "$1".result 2>&1
    status=$?
    ok "$1 output" [ $status -eq 0 ]
    if [ $status -eq 0 ] ; then
        rm "$1".result
    fi
}

# Total tests.  There are two tests per row in ok_result.
plan 10

# Run the individual tests.
ok_result c-count_tokens "$BUILD"  0
ok_result c-count_nondelim_chars "$BUILD"  0
ok_result c-token_str_to_array "$BUILD"  0
ok_result sh-oget_home "$SOURCE"  0
ok_result sh-get_ochar_args "$SOURCE"  0


