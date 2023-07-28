/**
 * @file GetOutput.cpp
 * @author Chancelor Cuddeback
 * @brief Generates an output file of the estimated gait phase for each timestep of a Trial. Stores the data in 
 * output/filename.csv. Takes a filename string as its input. 
 * @date 2023-07-26
 * 
 */

// ============================= App ===============================
// https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
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

class CSVRow
{
    public:
        std::string_view operator[](std::size_t index) const
        {
            return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1));
        }
        std::size_t size() const
        {
            return m_data.size() - 1;
        }
        void readNextRow(std::istream& str)
        {
            std::getline(str, m_line);

            m_data.clear();
            m_data.emplace_back(-1);
            std::string::size_type pos = 0;
            while((pos = m_line.find(',', pos)) != std::string::npos)
            {
                m_data.emplace_back(pos);
                ++pos;
            }
            // This checks for a trailing comma with no data after it.
            pos   = m_line.size();
            m_data.emplace_back(pos);
        }
    private:
        std::string         m_line;
        std::vector<int>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}   

class CSVIterator
{   
    public:
        typedef std::input_iterator_tag     iterator_category;
        typedef CSVRow                      value_type;
        typedef std::size_t                 difference_type;
        typedef CSVRow*                     pointer;
        typedef CSVRow&                     reference;

        CSVIterator(std::istream& str)  :m_str(str.good()?&str:nullptr) { ++(*this); }
        CSVIterator()                   :m_str(nullptr) {}

        // Pre Increment
        CSVIterator& operator++()               {if (m_str) { if (!((*m_str) >> m_row)){m_str = nullptr;}}return *this;}
        // Post increment
        CSVIterator operator++(int)             {CSVIterator    tmp(*this);++(*this);return tmp;}
        CSVRow const& operator*()   const       {return m_row;}
        CSVRow const* operator->()  const       {return &m_row;}

        bool operator==(CSVIterator const& rhs) {return ((this == &rhs) || ((this->m_str == nullptr) && (rhs.m_str == nullptr)));}
        bool operator!=(CSVIterator const& rhs) {return !((*this) == rhs);}
    private:
        std::istream*       m_str;
        CSVRow              m_row;
};

class CSVRange
{
    std::istream&   stream;
    public:
        CSVRange(std::istream& str)
            : stream(str)
        {}
        CSVIterator begin() const {return CSVIterator{stream};}
        CSVIterator end()   const {return CSVIterator{};}
};

template<class VecType>
void save_map_as_csv(std::string fname, std::map<std::string, std::vector<VecType>> map)
{
    std::ofstream out_file;
    out_file.open("output/"+fname+".csv");
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
}

template <class VecType, class GPE>
std::vector<VecType> test(std::vector<VecType> with_data, GPE gpe)
{
    std::vector<VecType> out;
    for (auto val : with_data)
    {
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
    std::string path = "data/1649109361258579.csv";
    std::ifstream file(path);

    // Unpack the CSV into dict
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
                std::cout << "Err converting: " << str << " from chars\n";
                continue;
            }
            data.emplace_back(val);
        }
        dict[key] = std::vector<float>();
        std::swap(dict[key], data);
    }
    
    // Start the clock on millis
    arduino_init();

    // Construct your GaitPhaseEstimator(s)
    const float lower_contact_thresh{0.1f};
    const float upper_contact_thresh{0.2f};
    static GaitPhaseEstimator lgpe = GaitPhaseEstimator(lower_contact_thresh, upper_contact_thresh);
    static GaitPhaseEstimator rgpe = GaitPhaseEstimator(lower_contact_thresh, upper_contact_thresh);

    // Test the estimator
    std::vector<float> l_output = test(dict["LFsr"], lgpe);
    std::vector<float> r_output = test(dict["RFsr"], rgpe);

    // Pack into map
    std::map<std::string, std::vector<float>> outmap = {{"LGaitPhase", l_output}, {"RGaitPhase", r_output}};
    
    // Save the output to a csv
    save_map_as_csv("data", outmap);

    return 0;
}