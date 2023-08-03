## Overview
This is used to test the GaitPhaseEstimator code. Using the data in the 'data' subdirectory, this code compares the
output of the GaitPhaseEstimator to the true gait phase. This checks for functionality and performance. 

The GaitPhase Estimator uses mock Arduino functions and is compiled and tested with GetOutput.cpp. The code is 
compiled by Compile.py and requires that your system has installed GCC. TrueGaitPhase.py generates the true
gait phases that are used for comparison. All of this code is incorporated into Evaluate.py.

Evaluate.py will return the RMSE error of the gait phase estimator. 

## TLDR
Install GCC/G++ on your system and run Evaluate.py to get the RMSE error of the gait phase estimator under test.

## Dependencies
Python V3.8+
- numpy
- pandas
- matplotlib
  
GCC/G++

# Installing dependencies
On Windows GCC/G++ can be installed through MSYS2 [here](https://www.msys2.org/). If your having trouble follow
along [here](https://www.freecodecamp.org/news/how-to-install-c-and-cpp-compiler-on-windows/).

You also must have Python3.8+ and numpy. Install python3 by following along [here](https://phoenixnap.com/kb/how-to-install-python-3-windows#:~:text=How%20to%20Install%20Python%203%20on%20Windows%2010,Variables%20%28Optional%29%207%20Install%20virtualnv%20%28Optional%29%20See%20More.). Numpy can be installed
trivially through pythons package manager pip, ie 
    $python3 -m pip install numpy

# Improvements
Any OS dependent code has been postpended by !WindowsGoo!
