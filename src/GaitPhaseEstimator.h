/**
 * @file GaitPhaseEstimator.h
 * @author Chancelor Cuddeback
 * @brief Uses normalized Heel FSR data to estimate the Gait phase of a single leg. 
 * 
 */

#ifndef GAITPHASEESTIMATOR_H
#define GAITPHASEESTIMATOR_H

// The gait phase estimator provides gait phase as a percent given normailized heel fsr data, and thresholds used to define ground contact.
class GaitPhaseEstimator {
  public:
    /**
     * @brief Construct a new Gait Phase Estimator object
     * 
     * @param number_of_avg_steps 
     * @param valid_step_variance 
     * @param lower 
     * @param upper 
     */
    GaitPhaseEstimator(int number_of_avg_steps, float valid_step_variance, float lower, float upper);
    /**
     * @brief Destroy the Gait Phase Estimator object
     * 
     */
    ~GaitPhaseEstimator();
    /**
     * @brief Set the ground contact thresholds. These values are used for the schmitt trigger
     * 
     * @param lower 
     * @param upper 
     */
    void set_thresholds(float lower, float upper);
    /**
     * @brief Get the current gait phase estimate
     * 
     * @return float 
     */
    float get_phase();
    /**
     * @brief Add a new normalized FSR value to update the gait phase estimate
     * 
     * @param norm_fsr 
     * @return float 
     */
    float update_phase(float norm_fsr);
  private:
    float _gait_phase; /** Current gait phase estimate (0-100)*/
    bool _prev_state; /** Previous state of schmit trigger */
   
    float _lower_contact_threshold, _upper_contact_threshold; /** Ground contact thresholds for schmit trigger */
    float _ground_strike_timestamp; /** Most recent ground strike time */

    const int _max_steps; /** Number of steps to save */ 
    float* _step_times; /** Steps time of n lenght. Heap allocated in constructor */
    float _avg_step_time; /** Average step time */
    
    const float _valid_step_variance; /** Percentage deviation from mean to determine acceptable step */

    bool _schmitt_trigger(float value, bool is_high, float lower_threshold, float upper_threshold);
};


#endif