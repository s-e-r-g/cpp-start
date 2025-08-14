# C++ Project Template

## Build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
````

## Run

```bash
# App (shows wall time and, on Linux, CPU perf counters)
./build/app

# Unit tests
ctest --test-dir build --output-on-failure

# Benchmarks
./build/alg_bench
```

### Notes

* Linux perf counters require permissions (either `sudo` or `echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid`).
* On non-Linux, perf counters silently disable; wall time is still printed.
* Toggle components:

  * `-DBUILD_TESTING=OFF` to skip tests
  * `-DBUILD_BENCHMARKS=OFF` to skip benchmarks
  * `-DENABLE_LINUX_PERF=OFF` to force stub perf implementation

### Why this structure
- Library under test allows clean reuse between app/tests/benchmarks.
- FetchContent pins exact versions to ensure reproducible builds.
- Debug builds get sanitizers; Release gets `-O3` and `NDEBUG`.
- Perf counters isolated behind a tiny API for portability.

