#include "GaitPhaseEstimator.h"
#include "Arduino.h"

GaitPhaseEstimator::GaitPhaseEstimator(float lower, float upper)
{
  _prev_step_time = 0;
  _gait_phase = 0;
  _prev_state = false;
  _lower_contact_threshold = lower;
  _upper_contact_threshold = upper;
}

/**
 * @brief Destroy the Gait Phase Estimator:: Gait Phase Estimator object
 * 
 */
GaitPhaseEstimator::~GaitPhaseEstimator() {}


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
  const bool rising_edge = current_state > this->_prev_state;
  // Rising edge represents ground strike
  if (rising_edge) 
  {
    // Calculate the previous steps total step time
    const int now = millis();
    this->_prev_step_time = now - this->_ground_strike_timestamp;

    // Reset ground strike timestamp
    this->_ground_strike_timestamp = now;
  }

  // Update previous state
  this->_prev_state = current_state;

  // Calculate phase, saturate at 100%
  int now = millis();
  this->_gait_phase = min((millis() - this->_ground_strike_timestamp) / this->_prev_step_time * 100.0f, 100.0f);

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
