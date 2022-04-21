#include "timer.h"

// Alias for less typing
using cl = std::chrono::steady_clock; 

Timer::Timer()
    : m_delta(0), m_paused(false) {}

cl::time_point Timer::get() const {
    return m_next;
}

void Timer::set(cl::time_point other) {
    m_next = other;
}

bool Timer::hasPassed() const {
    return cl::now() > m_next;
}

void Timer::pause() {
    m_paused = true;
    m_delta = m_next - cl::now();
}

void Timer::resume() {
    m_paused = false;
    m_next = cl::now() + m_delta;
}

bool Timer::isPaused() const {
    return m_paused;
}

void Timer::operator=(cl::time_point other) {
    set(other);
}

void Timer::operator+=(cl::duration other) {
    set(m_next + other);
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

