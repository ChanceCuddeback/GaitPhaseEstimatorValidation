#ifndef ARDUINO_H
#define ARDUINO_H
// Arduino.h to mock necessary functionality
// Gait Phase estiamator requires millis, and min

/**
 * @brief Initialize the mock Arduino functionality
 * 
 */
void arduino_init();

/**
 * @brief Milliseconds since init was called
 * 
 * @return unsigned long 
 */
unsigned long millis();

// Bring in min
#include <algorithm>
using namespace std;


#endif