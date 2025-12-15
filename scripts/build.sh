#!/usr/bin/env bash
set -euo pipefail

# Build script for the project. By default this configures and builds the
# full tree (library and tests). Usage:
#   ./scripts/build.sh [--clean] [--build-type TYPE] [--jobs N]

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
BUILD_TYPE="Release"
JOBS="$(nproc)"

while [[ $# -gt 0 ]]; do
	case "$1" in
		--clean)
			echo "[build] Cleaning build directory: $BUILD_DIR"
			rm -rf "$BUILD_DIR"
			shift
			;;
		--build-type)
			BUILD_TYPE="$2"
			shift 2
			;;
		--jobs)
			JOBS="$2"
			shift 2
			;;
		-j*)
			JOBS="${1#-j}"
			shift
			;;
		--help|-h)
			echo "Usage: $0 [--clean] [--build-type TYPE] [--jobs N]"
			exit 0
			;;
		*)
			echo "Unknown option: $1"
			exit 1
			;;
	esac
done

echo "[build] Configuring (build type: $BUILD_TYPE)"
cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

echo "[build] Building all targets (jobs=$JOBS)"
cmake --build "$BUILD_DIR" -- -j"$JOBS"

echo "[build] Build finished. Artifacts are under: $BUILD_DIR"
