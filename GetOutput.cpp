/**
 * @file GetOutput.cpp
 * @author Chancelor Cuddeback
 * @brief Generates an output file of the estimated gait phase for each timestep of a Trial. Stores the data in 
 * output/filename.csv. Takes a filename string as its input. 
 * @date 2023-07-26
 * 
 */
#include <iostream>
#include <string>

#include "src/GaitPhaseEstimator.h"

int main(int argc, char* argv[])
{
    // Handle Input
    std::string path = argv[1];
    std::cout << "Got path " << path << "\n";

    static GaitPhaseEstimator gpe = GaitPhaseEstimator(5, 0.20, 0.1, 0.2);

    

    return 0;
}