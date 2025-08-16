#ifdef HAVE_LINUX_PERF
#include "perf_counters.hpp"

#include <cerrno>
#include <cstring>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <vector>

namespace {
static int perf_event_open(struct perf_event_attr* hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags)
{
    return static_cast<int>(syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags));
}
}

struct PerfSession::Impl {
    struct Counter {
        int fd;
        std::string name;
    };
    std::vector<Counter> counters;
};

PerfSession::~PerfSession() { delete impl_; }

bool PerfSession::open(const std::vector<PerfCounterSpec>& specs)
{
    delete impl_;
    impl_ = new Impl();
    int group_fd = -1;
    for (const auto& s : specs) {
        perf_event_attr pea {};
        pea.size = sizeof(pea);
        pea.type = s.type;
        pea.config = s.config;
        pea.disabled = 1;
        pea.exclude_kernel = 0; // include kernel if you want
        pea.exclude_hv = 1;
        int fd = perf_event_open(&pea, 0 /*self*/, -1 /*any CPU*/, group_fd, 0);
        if (fd == -1) {
            // cleanup
            for (auto& c : impl_->counters)
                close(c.fd);
            delete impl_;
            impl_ = nullptr;
            return false;
        }
        if (group_fd == -1)
            group_fd = fd; // first counter is the group leader
        impl_->counters.push_back({fd, s.name});
    }
    return true;
}

void PerfSession::start()
{
    if (!impl_)
        return;
    // reset & enable group leader (fd of first counter)
    if (!impl_->counters.empty()) {
        int leader = impl_->counters.front().fd;
        ioctl(leader, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
        ioctl(leader, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
    }
}

void PerfSession::stop()
{
    if (!impl_)
        return;
    if (!impl_->counters.empty()) {
        int leader = impl_->counters.front().fd;
        ioctl(leader, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
    }
}

std::vector<PerfResult> PerfSession::read() const
{
    std::vector<PerfResult> out;
    if (!impl_)
        return out;
    for (const auto& c : impl_->counters) {
        std::uint64_t value = 0;
        ssize_t n = ::read(c.fd, &value, sizeof(value));
        if (n == (ssize_t)sizeof(value)) {
            out.push_back({c.name, value});
        } else {
            out.push_back({c.name, 0});
        }
    }
    return out;
}

#endif // HAVE_LINUX_PERF
