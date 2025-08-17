# C++ Project Template

A modern CMake starter that builds:

- **`mylib`** — small library with a sample function.
- **`app`** — demo binary; prints wall time and (on Linux) optional CPU perf counters via `perf_event_open`.
- **Unit tests** — GoogleTest (fetched via CMake).
- **Benchmarks** — Google Benchmark (fetched via CMake).
---

## Requirements

- CMake ≥ **3.21** (presets use v10 schema; tested with 3.29+).
- C++20 compiler: GCC/Clang/MSVC.
- **Ninja** recommended on Linux/macOS (VS generator on Windows is fine).
- (Optional) Linux with `perf_event_open` for hardware counters.

---

## Quick start (Linux/macOS)

### Using CMake Presets (recommended)

Each preset keeps **all** CMake files (Cache, CMakeFiles, compile_commands.json, binaries) isolated under its own directory.

```bash
# Configure (Debug GCC)
cmake --preset gcc-debug

# Build
cmake --build --preset gcc-debug -j

# Run the app
./build/linux-gcc-debug/app

# Run tests
ctest --preset test-gcc-debug --output-on-failure

# Run benchmarks
./build/linux-gcc-debug/alg_bench
```

### Release build

```bash
cmake --preset gcc-release
cmake --build --preset gcc-release
```

### ASan (Clang)

```bash
cmake --preset clang-asan
cmake --build --preset clang-asan
ASAN_OPTIONS=detect_leaks=1 ./build/clang-asan/app
```

### Windows (MSVC)

```powershell
cmake --preset msvc-debug
cmake --build --preset msvc-debug
ctest --preset test-msvc-debug --output-on-failure
```

---

## Classic (manual) build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

# App
./build/app

# Tests
ctest --test-dir build --output-on-failure

# Benchmarks
./build/alg_bench
```


---

## Targets

- **Library:** `mylib`
- **Binary:** `app`
- **Tests:** `alg_tests` (GoogleTest)
- **Benchmarks:** `alg_bench` (Google Benchmark)

Dependencies are fetched automatically via CMake **FetchContent** (first configure needs internet).

---

## Perf counters on Linux (optional)

If built with `-DENABLE_LINUX_PERF=ON` (enabled in Linux presets), `app` reads counters like `cycles`, `instructions`, `cache-misses` using `perf_event_open`.

Permissions:

```bash
# Temporary (current boot)
echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid
# or run as root
sudo ./build/gcc-release/app
```

If unavailable, the app still runs and prints wall time.

---

## Sanitizers

Enable with the `clang-asan` preset, or flags:

```
-fsanitize=address,undefined -fno-omit-frame-pointer
```

Common runtime options:

```bash
export ASAN_OPTIONS=detect_leaks=1:halt_on_error=1
```

---

## Coverage (GCC)

```bash
cmake --preset gcc-coverage
cmake --build --preset gcc-coverage -j
ctest --preset test-gcc-debug --output-on-failure
# Post-process with gcovr or lcov+genhtml (not included).
```

---
## TODO:
- Add clang-tidy
- Add devcontainer


---
## License
MIT
