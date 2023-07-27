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


// ============================= App ===============================

#include "src/Arduino.h"
#include "src/GaitPhaseEstimator.h"


int main(int argc, char* argv[])
{
    // Handle Input
    std::string path = "data/1649109361258579.csv";
    std::ifstream   file(path);

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
                continue;
            }
            data.emplace_back(val);
        }
        dict[key] = std::vector<float>();
        std::swap(dict[key], data);
    }
    
    
    arduino_init();
    // Construct your GaitPhaseEstimator
    static GaitPhaseEstimator gpe = GaitPhaseEstimator(5, 0.20, 0.1, 0.2);

    // Which data do you need serialized



    return 0;
}