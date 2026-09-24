#!/bin/sh
# Build and run from any working directory. Optional: SANITIZE=1.
set -eu

cd "$(dirname "$0")"
build_dir=$(mktemp -d "${TMPDIR:-/tmp}/inih-cpp-tests.XXXXXX")
trap 'rm -rf "$build_dir"' EXIT
trap 'exit 1' HUP INT TERM

sanitize_flags=
if [ "${SANITIZE:-0}" = 1 ]; then
    sanitize_flags="-fsanitize=address,undefined -fno-omit-frame-pointer"
fi

# Build twice: with the parser's default settings, and with new-section callbacks
# enabled so sections without fields are reported.
for new_section in 0 1; do
    defines="-DINI_CALL_HANDLER_ON_NEW_SECTION=$new_section"
    "${CC:-cc}" -std=c89 -pedantic-errors -Wall -Wextra -g $sanitize_flags $defines \
        -c ../../ini.c -o "$build_dir/ini.o"
    for test_source in INIReaderTest.cpp INIReaderCopyTest.cpp INIReaderMoveTest.cpp INIReaderBehaviorTest.cpp; do
        "${CXX:-c++}" -std=c++11 -pedantic-errors -Wall -Wextra -g $sanitize_flags $defines \
            ../INIReader.cpp "$test_source" "$build_dir/ini.o" -o "$build_dir/test"
        "$build_dir/test"
    done
done
