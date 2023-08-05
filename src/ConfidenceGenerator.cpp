#include "ConfidenceGenerator.h"
#include <cmath>

ConfidenceGenerator::ConfidenceGenerator(float alpha)
{
    _avg_step.reserve(100);
    _avg_step_sigma.reserve(100);
    _alpha = alpha;
}


float ConfidenceGenerator::update_confidence(float sig, float phase)
{   
    const int floored_phase = static_cast<int>(floor(phase)); // Only works for an array size of 100
    // Compare to prior phase pair
    const float prior = _lerp(_avg_step[floored_phase], _avg_step[floored_phase+1], phase-floored_phase);
    const float delta = sig - prior;
    const float percent_error = abs(delta/prior)*100;
    // Conditionally add the new pair (weighted update)
    if (_should_update(percent_error))
    {
        const float new_value = _ema(sig, prior, _alpha);
        _update(floored_phase, phase, new_value, delta);
    }
    // Compute the confidence
    return percent_error;
}

bool _should_update(float percent_error)
{
    return true;
}

void _update(int i, float phase, float new_value, float delta)
{

}

template <typename T>
T _lerp(T a, T b, T t)
{
    return a + t * (b - a);
}

template <typename T>
T _ema(T new_value, T old_value, T alpha)
{
    return old_value + alpha*(new_value-old_value);
}