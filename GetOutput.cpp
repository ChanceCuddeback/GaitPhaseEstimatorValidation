/**
 * @file GetOutput.cpp
 * @author Chancelor Cuddeback
 * @brief Generates an output file of the estimated gait phase for each timestep of a Trial. Stores the data in 
 * output/filename.csv. Takes a filename string as its input. 
 * @date 2023-07-26
 * 
 */

// ============================= App ===============================
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <format>
#include <charconv>
#include "src/CSVHelpers.h"

/**
 * @brief Unpack a csv file handle and return a std::map. Currently ignores csv cell to float conversion errors. 
 * 
 * @param file 
 * @return std::map<std::string, std::vector<float>> 
 */
static std::map<std::string, std::vector<float>> unpack_file(std::istream& file)
{
    std::map<std::string, std::vector<float>> dict;
    for(auto& row: CSVRange(file))
    {
        std::string key = std::string(row[0]);
        std::vector<float> data;
        for (std::size_t i = 1; i < row.size(); i++)
        {
            const std::string str = std::string(row[i]);
            float val{0.0f};
            auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), val);
            if (ec != std::errc())
            {
                continue;
            }
            data.emplace_back(val);
        }
        dict[key] = std::vector<float>();
        std::swap(dict[key], data);
    }
    return dict;
}
    

/**
 * @brief Saves a std::maps to a csv file, such that the keys are on the zero column and the the data is a row vector. 
 * 
 * @tparam VecType 
 * @param fname Name of file path to save the data
 * @param map Vector of maps
 */
template<class VecType>
static void save_map_as_csv(std::string fname, std::map<std::string, std::vector<VecType>> map)
{
    std::ofstream out_file;
    out_file.open("output/"+fname+".csv");
    // Write Header
    // out_file << "Head,\n";
    for (auto pair : map)
    {
        std::string k = pair.first;
        std::vector<VecType> v = pair.second;
        // Save key
        out_file << k << ",";
        // Save vector
        for (auto val : v)
        {
            if (val > 100 || val < -1)
            {
                std::cout << "Out of bounds\n";
            }
            std::string str;
            std::format_to(std::back_inserter(str), "{},", val);
            out_file << str;
        }
        out_file << "\n";
    }
    out_file.close();
}

/**
 * @brief Test the Gait phase estimator by calling its float update_phase(float) method. This function serializes a vector and returns the 
 * phase for each timestep.  
 * 
 * @tparam VecType 
 * @tparam GPE 
 * @param with_data 
 * @param gpe 
 * @return std::vector<VecType> 
 */
template <class VecType, class GPE>
static std::vector<VecType> test(std::vector<VecType> with_data, GPE gpe)
{
    std::vector<VecType> out;
    for (auto val : with_data)
    {
        const VecType new_sample = gpe.update_phase(val);
        out.emplace_back(gpe.update_phase(val));
    }
    return out;
}
// ============================= App ===============================

#include "src/Arduino.h"
#include "src/GaitPhaseEstimator.h"

int main(int argc, char* argv[])
{
    // Handle Input 
    // TODO: check input
    std::string path = argv[1];
    std::ifstream file(path);

    // Unpack the CSV into dict
    std::map<std::string, std::vector<float>> dict = unpack_file(file);
    
    // Start the clock on millis
    arduino_init();

    // Construct your GaitPhaseEstimator(s)
    const float lower_contact_thresh{0.05f};
    const float upper_contact_thresh{0.15f};
    static GaitPhaseEstimator lgpe = GaitPhaseEstimator(lower_contact_thresh, upper_contact_thresh);
    static GaitPhaseEstimator rgpe = GaitPhaseEstimator(lower_contact_thresh, upper_contact_thresh);

    // Test the estimator
    std::vector<float> l_output = test(dict["LFsr"], lgpe);
    std::vector<float> r_output = test(dict["RFsr"], rgpe);

    // Pack into map
    // std::map<std::string, std::vector<float>> outmap = {{"LGaitPhase", l_output}, {"RGaitPhase", r_output}};
    
    // Save the output to a csv, pack the data into a map such that the keys will be data headers
    save_map_as_csv("data", std::map<std::string, std::vector<float>>(
        {{"LGaitPhase", l_output}, {"RGaitPhase", r_output}}));

    return 0;
}