#include <chrono>

// Alias for less typing
using cl = std::chrono::steady_clock; 

class Timer {
    private:
        cl::time_point m_next;
        cl::duration m_delta;

    public:
        Timer(cl::duration delta);

        cl::time_point getNext();
        void next();
        /* Wether `m_next` is less than the current time */
        bool hasPassed();
};
