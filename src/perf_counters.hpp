#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <optional>

struct PerfCounterSpec {
    std::string name;   // e.g. "cycles", "instructions"
    std::uint32_t type; // PERF_TYPE_* (Linux) or 0 on non-Linux
    std::uint64_t config; // PERF_COUNT_* (Linux) or 0 on non-Linux
};

struct PerfResult {
    std::string name;
    std::uint64_t value = 0;
};

class PerfSession {
public:
    PerfSession() = default;
    ~PerfSession();

    // Open chosen counters for the current process/thread.
    bool open(const std::vector<PerfCounterSpec>& specs);

    void start();
    void stop();

    std::vector<PerfResult> read() const;

private:
    struct Impl;
    Impl* impl_ = nullptr; // pimpl to hide OS specifics
};
