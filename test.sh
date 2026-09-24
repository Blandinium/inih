#!/bin/sh
set -eu

for compiler in gcc clang; do
    case "$compiler" in
        gcc)   cxx=g++ ;;
        clang) cxx=clang++ ;;
    esac

    build_dir="build-$compiler-sanitize"

    CC="$compiler" CXX="$cxx" meson setup --reconfigure "$build_dir" \
        -Dcpp_std=c++11 \
        -Db_sanitize=address,undefined \
        -Db_lundef=false \
        -Dc_args=-fno-omit-frame-pointer \
        -Dcpp_args=-fno-omit-frame-pointer

    UBSAN_OPTIONS=halt_on_error=1 \
        meson test -C "$build_dir" --print-errorlogs
done
