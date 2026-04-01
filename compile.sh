#!/usr/bin/env bash

set -euo pipefail

binary="pgn_parser"
build_dir="build"
build_type="RelWithDebInfo" # Debug, Release, RelWithDebInfo, MinSizeRel
compiler="gcc"
generator="make"
jobs="$(nproc)"

usage() {
	cat <<EOF
Usage: ./compile.sh [options]

Options:
  --compiler <gcc|clang>     Compiler toolchain (default: gcc)
  --generator <make|ninja>   Build system generator (default: make)
  --build-type <type>        CMake build type (default: Release)
  --jobs <N>                 Parallel jobs for build (default: nproc)
  --build-dir <dir>          Build directory (default: build/<compiler>-<generator>-<build-type>)
  -h, --help                 Show this help

Examples:
  ./compile.sh --compiler clang --generator ninja
  ./compile.sh --compiler gcc --generator make --build-type Debug --jobs 8
EOF
}

while [[ $# -gt 0 ]]; do
	case "$1" in
		--compiler)
			compiler="$2"
			shift 2
			;;
		--generator)
			generator="$2"
			shift 2
			;;
		--build-type)
			build_type="$2"
			shift 2
			;;
		--jobs)
			jobs="$2"
			shift 2
			;;
		--build-dir)
			build_dir="$2"
			shift 2
			;;
		-h|--help)
			usage
			exit 0
			;;
		*)
			echo "Unknown option: $1" >&2
			usage
			exit 1
			;;
	esac
done

case "$compiler" in
	gcc)
		c_compiler="gcc"
		cxx_compiler="g++"
		;;
	clang)
		c_compiler="clang"
		cxx_compiler="clang++"
		;;
	*)
		echo "Unsupported compiler: $compiler (use gcc or clang)" >&2
		exit 1
		;;
esac

case "$generator" in
	make)
		cmake_generator="Unix Makefiles"
		;;
	ninja)
		cmake_generator="Ninja"
		;;
	*)
		echo "Unsupported generator: $generator (use make or ninja)" >&2
		exit 1
		;;
esac

if [[ "$build_dir" == "build" ]]; then
	build_dir="build/${compiler}-${generator}-${build_type}"
fi

build_cmd=(cmake --build "$build_dir" -- -j"$jobs")

cmake -S . -B "$build_dir" \
	-G "$cmake_generator" \
	-DCMAKE_BUILD_TYPE="$build_type" \
	-DCMAKE_C_COMPILER="$c_compiler" \
	-DCMAKE_CXX_COMPILER="$cxx_compiler"

"${build_cmd[@]}"

cp "./$build_dir/$binary" ./pytest/

echo "Build complete: ./$build_dir/$binary"
