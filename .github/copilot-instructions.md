## Repo quick context
This repository implements a small C++ audio DSP library and a tiny example binary.
Key ideas:
- C++ code lives under `src/` and headers under `inc/`.
- A static library target `wns_audio_dsp` is produced (see `src/CMakeLists.txt`) and an example executable `wns_main` when `src/main.cpp` is present.
- Conventional prefix: most public symbols use the `wns_` or `wns_`-style prefix and namespaces like `wns_Filters` and `wns_Modules`.

## Big-picture architecture / where things live
- inc/ — public headers (filters, modules, utils). Examples: `inc/filters/wns_BiquadFilter.hpp`, `inc/modules/wns_FFT.hpp`.
- src/ — C++ implementation files mirrored to `inc/` (e.g. `src/filters/wns_BiquadFilter.cpp`, `src/modules/wns_FFT.cpp`).
- common/ and utils/ — shared constants and small helper macros (`common/wsn_Common.hpp`, `utils/wsn_utils.hpp`).
- test/ — contains a small Python plotting script; there is no C++ unit-test harness yet.
- docker/ — container bits (Dockerfile and docker-compose) to reproduce a build+python environment.

Design notes an agent should know:
- The library is assembled via file globbing in `src/CMakeLists.txt` (new .cpp files placed under `src/filters` or `src/modules` are picked up automatically).
- Logging is done with simple macros: `WNS_LOG`, `WNS_ERROR`, `WNS_WARNING`, `WNS_DEBUG` in `utils/wsn_utils.hpp`.
- Error codes are centralized in `common/wsn_ErrorCodes.hpp` and typedef'd to `wsn_eF`.
- Sampling frequency and constants live in `common/wsn_Common.hpp` (e.g. `wns_Common::SAMPLING_FREQUENCY`).

## How to build (C++ local dev)
- From the repository root (recommended):
  - Configure & build using CMake. The project writes runtime binaries to `build/bin`.
    - cmake -S . -B build
    - cmake --build build -- -j$(nproc)
  - The example executable (if present) will be at `build/bin/wns_main`.

Notes:
- `src/CMakeLists.txt` creates a static library `wns_audio_dsp` from all `src/*.cpp` files via `file(GLOB ...)` and links it into `wns_main`.
- Source files use relative includes (e.g. `#include "../inc/filters/wns_BiquadFilter.hpp"`) but CMake already adds `inc/`, `common/`, and `utils/` to the include paths.

## How to run the small example
- After building, run the example (it prints one filtered sample):
  - ./build/bin/wns_main

## Docker / reproducible environment
- The repository includes `docker/Dockerfile` and `docker/docker-compose.yml` to create a Python + C++ build environment.
- To build and run the container (from repo root):
  - docker compose -f docker/docker-compose.yml up --build

Caveat: the Compose file expects the `docker/` directory relative paths configured in the compose file; if you change folder layout, check build `context` in `docker/docker-compose.yml`.

## Tests and Python scripts
- `test/scripts/plot_frequency_response.py` is a small Python plotting script that references a Python `filters.BiquadFilter` class. There is no packaged Python module in this repo by default. The `docker` image includes pip install from `requirements.txt`.
- `scripts/build.sh` and `scripts/run_tests.sh` exist but are empty—don't assume they perform actions.

## Project-specific conventions and patterns (agents should follow)
- Naming: C/C++ identifiers typically use `wns_` prefix for types and files (`wns_BiquadFilter`, `wns_FFT`, `WNS_Limiter`). Keep new public symbols consistent.
- Namespaces: use `wns_Filters`, `wns_Modules`, or `wns_Common` when adding new components.
- Error handling: return a `wsn_eF` (alias of `WSN_ErrorCodes`) for operations that can fail. See `common/wsn_ErrorCodes.hpp`.
- Logging: use `WNS_LOG`, `WNS_DEBUG`, `WNS_WARNING`, `WNS_ERROR` macros in `utils/wsn_utils.hpp` instead of raw std::cout/stderr prints.
- Header placement: public API headers go into `inc/` and implementation into `src/` with mirrored subfolders (e.g. `inc/filters/` <-> `src/filters/`).

## Integration points to watch
- `src/CMakeLists.txt` — controls library creation with file globs. If you add new languages or custom targets, update this file.
- `common/wsn_Common.hpp` — single source for DSP constants (PI, SAMPLING_FREQUENCY). Changing them affects algorithms globally.
- `utils/wsn_utils.hpp` — central simple logging macros used across modules.

## Files/places to inspect for DSP logic
- Biquad filter algorithm and parameterization:
  - `inc/filters/wns_BiquadFilter.hpp`
  - `src/filters/wns_BiquadFilter.cpp`
- FFT placeholder and bit-reversal util:
  - `inc/modules/wns_FFT.hpp`
  - `src/modules/wns_FFT.cpp`
- Limiter implementation:
  - `inc/modules/wns_Limiter.hpp`
  - `src/modules/wns_Limiter.cpp`

## Common pitfalls for agents (learned from repo)
- The project uses relative includes in .cpp files; adding files with different include styles may introduce duplicate/inconsistent include paths. Prefer adding headers to `inc/` and including them by path consistent with existing code.
- Build scripts under `scripts/` are placeholders — rely on CMake instructions above unless the scripts are filled.
- Python test utilities assume a separate Python implementation which may not exist; verify before running.

If any section above is unclear or you'd like CI/PR-ready templates (GitHub Actions for CMake build + Docker), tell me which area to expand and I will update the file.
