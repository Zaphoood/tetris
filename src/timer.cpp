#include "timer.h"

// Alias for less typing
using cl = std::chrono::steady_clock; 

Timer::Timer(cl::duration delta)
    : m_delta(delta) {}

cl::time_point Timer::getNext(){
    return m_next;
}

void Timer::next(){
    m_next += m_delta;
}

bool Timer::hasPassed(){
    return cl::now() > m_next;
}
