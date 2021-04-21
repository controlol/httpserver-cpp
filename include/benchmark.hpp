#include <chrono>
#include <functional>
#include <iostream>

class Benchmark
{
    std::chrono::high_resolution_clock::time_point _start;
    std::chrono::high_resolution_clock::time_point _end;
    bool _startInvoked = false;
    bool _endInvoked = false;

    Benchmark() {}; // empty

    public:
    // Set the start time
    inline auto start() -> void
    {
        _startInvoked = true;
        _start = std::chrono::high_resolution_clock::now();
    }

    // Set the end time
    inline auto end() -> void
    {
        _end = std::chrono::high_resolution_clock::now();
        _endInvoked = true;
    }

    // Returns time taken in microseconds
    inline auto measure() -> double
    {
        if (_startInvoked && _endInvoked)
        {
            _startInvoked = false;
            _endInvoked = false;
            return double(std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count());   
        }

        return 0.0;
    }

    // Get instance
    static auto get() -> Benchmark&
    {
        static Benchmark bm;
        return static_cast<Benchmark&>(bm);
    }

    // Test a function any amount of times and get the average.
    // Use start() and end() to set where to measure specifically.
    inline auto test(std::function<void()> experiment, int amountOfRuns = 1) -> void
    {
        double score = 0.0;

        for (int i = 0; i < amountOfRuns; i++)
        {
            _startInvoked = false;
            _endInvoked = false;
            auto s = std::chrono::high_resolution_clock::now();
            experiment();
            auto e = std::chrono::high_resolution_clock::now();
            if (_startInvoked)
                s = _start;
            if (_endInvoked)
                e = _end;
            auto time = std::chrono::duration_cast<std::chrono::microseconds>(e - s);
            score += (double)time.count();
        }

        std::cout << "Test took on average " << (score / double(amountOfRuns)) << " microseconds to execute.\n";
    }
};