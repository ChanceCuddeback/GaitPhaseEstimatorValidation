#ifndef CONFIDENCEGENERATOR_H
#define CONFIDENCEGENERATOR_H
/**
 * @file ConfidenceGenerator.h
 * @author Chancelor Cuddeback
 * @brief Uses a signal[Sig] (FSR, Angle, ect) and percent gait phase to generate percent confidence. This simply correlates Sig and gait phase data to generate 
 * trajectories (Sig vs phase). If the current (Sig, Phase) is different from past (Sig, Phase) pairs the confidence will be lower. 
 */


class ConfidenceGenerator {
    public:
    ConfidenceGenerator();

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

};


#endif 