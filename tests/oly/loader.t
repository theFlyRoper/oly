#! /bin/sh
#
# Test suite for loader functions
# Copyright 2014 The Oly Project
#
# See LICENSE for licensing terms.

. "$SOURCE/tap/libtap.sh"
cd "${BUILD}/oly/loader"

# Run a binary, saving its output, and then compare that output to the
# corresponding *.output file.
ok_result () {
    "$2"/oly/loader/"$1" > "$1".result 2>&1
    status=$?
    ok "$1 exit status" [ $status -eq "$3" ]
    case "$1" in
        c-oget_home)
            diff -u "${BUILD}/oly/loader/$1".output "$1".result 2>&1
            ;;
        *)
            diff -u "${SOURCE}/oly/loader/$1".output "$1".result 2>&1
            ;;
    esac
    status=$?
    ok "$1 output" [ $status -eq 0 ]
    if [ $status -eq 0 ] ; then
        rm "$1".result
    fi
}

# Total tests.  There are two tests per row in ok_result.
plan 2

# Run the individual tests.
ok_result sh-oget_home "$BUILD"  0

