#ifndef CONFIDENCEGENERATOR_H
#define CONFIDENCEGENERATOR_H
/**
 * @file ConfidenceGenerator.h
 * @author Chancelor Cuddeback
 * @brief Uses a signal[Sig] (FSR, Angle, ect) and percent gait phase to generate percent confidence. This simply correlates Sig and gait phase data to generate 
 * trajectories (Sig vs phase). If the current (Sig, Phase) is different from past (Sig, Phase) pairs the confidence will be lower. 
 */
#include <vector>


class ConfidenceGenerator {
    public:
    ConfidenceGenerator(float alpha);

    /**
     * @brief Given a signal and phase pair, this function returns how similar this pair is to prior step pairs. 
     * Confidence is a measure of similarity step similarity. 
     * 
     * @param sig 
     * @param phase 
     * @return float 
     */
    float update_confidence(float sig, float phase);


    private:
    std::vector<float> _avg_step; // All of the steps averaged into one
    std::vector<float> _avg_step_sigma; // The symetric variance of the average step
    float _alpha;

    /**
     * @brief If the new value should be added to the step average
     * 
     * @param percent_error 
     * @return true 
     * @return false 
     */
    bool _should_update(float percent_error);

    /**
     * @brief Update the average step
     * 
     * @param i 
     * @param phase 
     * @param new_value 
     * @param delta
     */
    void _update(int i, float phase, float new_value, float delta);

    /**
     * @brief Linear interpolation
     * 
     * @tparam T 
     * @param a 
     * @param b 
     * @param t 
     * @return T 
     */
    template <typename T>
    T _lerp(T a, T b, T t);

    /**
     * @brief Exponentially weighted moving average.
     * 
     * @tparam T 
     * @param new_value 
     * @param old_value 
     * @param alpha 
     * @return T 
     */
    template <typename T>
    T _ema(T new_value, T old_value, T alpha);
};


#endif 