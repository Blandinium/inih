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

"${CC:-cc}" -std=c89 -pedantic-errors -Wall -Wextra -g $sanitize_flags \
    -c ../../ini.c -o "$build_dir/ini.o"
for test_source in INIReaderTest.cpp INIReaderCopyTest.cpp; do
    "${CXX:-c++}" -std=c++98 -pedantic-errors -Wall -Wextra -g $sanitize_flags \
        ../INIReader.cpp "$test_source" "$build_dir/ini.o" -o "$build_dir/test"
    "$build_dir/test"
done
