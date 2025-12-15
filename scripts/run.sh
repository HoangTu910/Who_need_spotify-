#!/usr/bin/env bash
set -euo pipefail

# Simple runner for the example executable. If the binary is missing it will try to build using scripts/build.sh
SELF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="${SELF_DIR}/.."
BUILD_DIR="${ROOT_DIR}/build"
BIN="${BUILD_DIR}/bin/wns_main"

if [ ! -x "${BIN}" ]; then
    echo "[run.sh] Executable not found: ${BIN}"
    echo "[run.sh] Attempting to build..."
    "${SELF_DIR}/build.sh" --build-type Debug --jobs 4
fi

if [ ! -x "${BIN}" ]; then
    echo "[run.sh] Build failed or executable still missing." >&2
    exit 2
fi

exec "${BIN}" "$@"
