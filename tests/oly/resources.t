#! /bin/sh
#
# Test suite for i18n resources
# Copyright 2014 The Oly Project
#
# See LICENSE for licensing terms.

. "$SOURCE/tap/libtap.sh"
cd "${BUILD}/oly/resources"

# Run a binary, saving its output, and then compare that output to the
# corresponding *.output file.
ok_result () {
    "$2"/oly/resources/"$1" > "$1".result 2>&1
    status=$?
    ok "$1 exit status" [ $status -eq "$3" ]
    case "$1" in
        c-el_gr)
            diff -u "${BUILD}/oly/resources/$1".output "$1".result 2>&1
            ;;
        *)
            diff -u "${SOURCE}/oly/resources/$1".output "$1".result 2>&1
            ;;
    esac
    status=$?
    ok "$1 output" [ $status -eq 0 ]
    if [ $status -eq 0 ] ; then
        rm "$1".result
    fi
}

# Total tests.  There are two tests per row in ok_result.
plan 6

# Run the individual tests.
ok_result c-el_gr "$BUILD"  0
ok_result c-ja "$BUILD"  0
ok_result c-root "$BUILD"  0

