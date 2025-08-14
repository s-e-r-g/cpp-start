#ifndef HAVE_LINUX_PERF
#include "perf_counters.hpp"
#include <vector>

PerfSession::~PerfSession() = default;

bool PerfSession::open(const std::vector<PerfCounterSpec>&){ return true; }
void PerfSession::start(){}
void PerfSession::stop(){}
std::vector<PerfResult> PerfSession::read() const { return {}; }

#endif
