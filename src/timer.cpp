#include "timer.h"

// Alias for less typing
using cl = std::chrono::steady_clock; 

Timer::Timer()
    : m_delta(0) {}

Timer::Timer(cl::duration delta)
    : m_delta(delta) {}

cl::time_point Timer::get() const {
    return m_next;
}

void Timer::set(cl::time_point other) {
    m_next = other;
}

bool Timer::hasPassed() const {
    return cl::now() > m_next;
}

void Timer::operator=(cl::time_point other) {
    set(other);
}

cl::time_point Timer::operator+(cl::duration other) {
    return m_next + other;
}

bool Timer::operator<(cl::time_point other) {
    return m_next < other;
}

bool Timer::operator>(cl::time_point other) {
    return m_next > other;
}

