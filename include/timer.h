#include <chrono>

// Alias for less typing
using cl = std::chrono::steady_clock; 

class Timer {
    private:
        cl::time_point m_next;
        cl::duration m_delta;
        bool m_paused;

    public:
        Timer();

        cl::time_point get() const;
        void set(cl::time_point other);
        /* Wether `m_next` is less than the current time */
        bool hasPassed() const;

        void pause();
        void resume();
        bool isPaused() const;

        void operator=(const cl::time_point other);
        void operator+=(const cl::duration other);
        cl::time_point operator+(const cl::duration other);
        bool operator<(const cl::time_point other);
        bool operator>(const cl::time_point other);
};
