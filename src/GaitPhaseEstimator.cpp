#include "GaitPhaseEstimator.h"
#include "Arduino.h"

GaitPhaseEstimator::GaitPhaseEstimator(int number_of_avg_steps, float valid_step_variance, float lower, float upper) :
  _max_steps(number_of_avg_steps),
  _valid_step_variance(valid_step_variance)
{
  _step_times = new float[_max_steps];
  _avg_step_time = 0;
  _gait_phase = 0;
  _prev_state = false;
  _lower_contact_threshold = lower;
  _upper_contact_threshold = upper;
}

/**
 * @brief Destroy the Gait Phase Estimator:: Gait Phase Estimator object
 * 
 */
GaitPhaseEstimator::~GaitPhaseEstimator()
{
  //delete[] _step_times;
}


void GaitPhaseEstimator::set_thresholds(float lower, float upper)
{
  _lower_contact_threshold = lower;
  _upper_contact_threshold = upper;
}


float GaitPhaseEstimator::get_phase()
{
  return _gait_phase;
}

float GaitPhaseEstimator::update_phase(float norm_fsr)
{
  // Check for ground contact
  const bool current_state = _schmitt_trigger(norm_fsr, this->_prev_state, this->_lower_contact_threshold, this->_upper_contact_threshold);
  
  // Rising edge represents ground strike
  if (current_state > this->_prev_state) {
    int now = millis();
    // Calculate total step time
    float total_step_time = now - this->_ground_strike_timestamp;
    
    // If step time is within variance of average step time, add to average
    const bool too_low = total_step_time < (_avg_step_time * (1 - this->_valid_step_variance));
    const bool too_high = total_step_time > (_avg_step_time * (1 + this->_valid_step_variance));
    if (!too_low && !too_high) {
      // Shift step times
      for (int i = 0; i < this->_max_steps - 1; i++) {
        this->_step_times[i] = this->_step_times[i + 1];
      }
      // Add new step time
      this->_step_times[this->_max_steps - 1] = total_step_time;
    }

    // Calculate new average step time
    float sum = 0;
    for (int i = 0; i < this->_max_steps; i++) {
      sum += this->_step_times[i];
    }
    this->_avg_step_time = sum / this->_max_steps;

    // Reset ground strike timestamp
    this->_ground_strike_timestamp = now;
  }

  // Update previous state
  this->_prev_state = current_state;

  // Calculate phase, saturate at 100%
  this->_gait_phase = min((millis() - this->_ground_strike_timestamp) / this->_avg_step_time * 100.0f, 100.0f);

  return this->_gait_phase;
}

bool GaitPhaseEstimator::_schmitt_trigger(float value, bool is_high, float lower_threshold, float upper_threshold)
{
  bool trigger = 0;
  if (is_high)
  {
    trigger = value > lower_threshold;  //One if true, Zero if false
  }
  else
  {
    trigger = value > upper_threshold;  //One if true, Zero if false
  }
  return trigger;
}
