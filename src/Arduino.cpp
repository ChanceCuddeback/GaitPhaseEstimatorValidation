#include "Arduino.h"
#include <chrono>
using sysclock = std::chrono::system_clock;
using ms = std::chrono::duration<double, std::milli>;
static auto start_time = sysclock::now();

void init()
{
    start_time = sysclock::now();
}

unsigned long millis()
{
    ms elapsed = sysclock::now() - start_time;
    return static_cast<unsigned long>(elapsed.count());
}