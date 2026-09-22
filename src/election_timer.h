#pragma once
#include <chrono>
#include <functional>
#include <random>

namespace raft {

// Decides "has enough time passed since we last heard from a leader
// that we should start an election?"
//
// The clock is injected as a std::function instead of calling
// std::chrono::steady_clock::now() directly. This is the key design
// choice: in production we pass the real clock, but in tests we pass
// a fake one we control by hand -- so a test can simulate "301ms have
// passed" instantly, with no real sleeping and no flakiness.
class ElectionTimer {
public:
    using Clock = std::function<std::chrono::steady_clock::time_point()>;

    // minTimeout/maxTimeout: the randomized range to pick from on each
    // reset (Raft paper typically uses 150-300ms). clock: defaults to
    // the real steady_clock; tests override this.
    ElectionTimer(std::chrono::milliseconds minTimeout,
                  std::chrono::milliseconds maxTimeout,
                  Clock clock = &std::chrono::steady_clock::now,
                  unsigned seed = std::random_device{}());

    // Call whenever we hear from a legitimate leader, grant a vote, or
    // start our own election. Records "now" and draws a NEW random
    // timeout for the next window.
    void reset();

    // Has the timeout elapsed since the last reset()?
    bool hasExpired() const;

    // Exposed mainly for logging/debugging.
    std::chrono::milliseconds currentTimeout() const { return timeout_; }

private:
    std::chrono::milliseconds randomTimeout();

    std::chrono::milliseconds minTimeout_;
    std::chrono::milliseconds maxTimeout_;
    Clock clock_;
    std::mt19937 rng_;

    std::chrono::steady_clock::time_point lastReset_;
    std::chrono::milliseconds timeout_;
};

} // namespace raft
