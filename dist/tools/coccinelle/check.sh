#!/bin/sh

# Copyright 2017 Kaspar Schleiser <kaspar@schleiser.de>
#
# This file is subject to the terms and conditions of the GNU Lesser
# General Public License v2.1. See the file LICENSE in the top level
# directory for more details.

: ${RIOTBASE:=$(pwd)}

. ${RIOTBASE}/dist/tools/ci/changed_files.sh

filter() {
    if [ $COCCINELLE_QUIET -eq 0 ]; then
        cat
    else
        grep '^---' | cut -f 2 -d ' '
    fi
}

EXIT_CODE=0

indent() {
    sed 's/^/    /g'
}

coccinelle_checkall() {
    local dir="$1"
    local warn="${2:-0}"

    [ -d "$dir" ] || {
        echo "$0: coccinelle_checkall(): $dir doesn't exist!"
        exit 1
    }

    for patch in $dir/*; do
        OUT="$(spatch --very-quiet \
            --macro-file-builtins ${RIOTBASE}/dist/tools/coccinelle/include/riot-standard.h \
            --file-groups "${COCCINELLE_TMP}" \
            --sp-file $patch | filter)"

        if [ -n "$OUT" ]; then
            if [ $COCCINELLE_QUIET -eq 1 ]; then
                echo "$patch:"
                echo "$OUT" | indent
                if [ COCCINELLE_WARNONLY -eq 0 ]; then
                    EXIT_CODE=1
                fi
            else
                echo "$OUT"
            fi
        fi
    done
}

FILES=$(FILEREGEX='\.c$' changed_files)

if [ -z "${FILES}" ]; then
    exit
fi

trap 'rm -f ${COCCINELLE_TMP}' EXIT

COCCINELLE_TMP=$(mktemp)
[ -f "$COCCINELLE_TMP" ] || {
    echo "$0: cannot create tempfile"
    exit 1
}

echo "$FILES" > ${COCCINELLE_TMP}

: ${COCCINELLE_QUIET:=0}

coccinelle_checkall ${RIOTBASE}/dist/tools/coccinelle/force

COCCINELLE_WARNONLY=1 \
    coccinelle_checkall ${RIOTBASE}/dist/tools/coccinelle/warn

exit $EXIT_CODE
