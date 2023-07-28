#include "GaitPhaseEstimator.h"
#include "Arduino.h"

GaitPhaseEstimator::GaitPhaseEstimator(float lower, float upper)
{
  this->_prev_step_count = 0;
  this->_current_step_count = 0;
  this->_gait_phase = 0;
  this->_prev_state = false;
  this->_lower_contact_threshold = lower;
  this->_upper_contact_threshold = upper;
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
  // Rising edge represents ground strike
  const bool rising_edge = current_state > this->_prev_state;
  this->_current_step_count += 1;
  if (rising_edge) 
  {
    // Calculate the previous steps total step time
    this->_prev_step_count = this->_current_step_count;

    // Reset ground strike timestamp
    this->_current_step_count = 0;
  }

  // Calculate phase, saturate at 100% and 0%
  float gait_phase = 0;
  if (this->_prev_step_count != 0)
  {
    gait_phase = (this->_current_step_count / this->_prev_step_count) * 100.0f;
  }
  this->_gait_phase = max(min(gait_phase, 100.0f), 0.0f);

  // Update previous state
  this->_prev_state = current_state;
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
