# !/usr/bin/env bash
# set -euo pipefail

# Small helper to configure, build, and run unit tests
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"

echo "[test] Configuring cmake in $BUILD_DIR"
cmake -S "$ROOT_DIR" -B "$BUILD_DIR"

echo "[test] Building tests"
cmake --build "$BUILD_DIR" -- -j$(nproc)

UTEST_BINARY="$BUILD_DIR/test/utest/wns_fft_utest"
# CMake places executables under ${CMAKE_BINARY_DIR}/bin by default in this repo
FALLBACK_BINARY="$BUILD_DIR/bin/wns_fft_utest"

if [ -x "$FALLBACK_BINARY" ]; then
	UTEST_BINARY="$FALLBACK_BINARY"
fi

if [ -x "$UTEST_BINARY" ]; then
	echo "[test] Running FFT unit test: $UTEST_BINARY"
	"$UTEST_BINARY"
else
	echo "[test] Unit test binary not found: $UTEST_BINARY"
	exit 1
fi

echo "[test] Done"
