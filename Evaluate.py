from Helpers import *
import subprocess
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import pathlib 
import csv
import math


# TODO: Perfrom for all files in data and aggregate results
data_path = "data\\1649109361258579.csv" # !WindowsGoo!
plotting_window = [0.475, 0.575] # Starting percent to final percent

# Compile the code 
abs_estimator_path = compile()

# Pull in the data
data = get_trial_dict(data_path)

# Generate true gait phase
l_phase = true_gait_phase(data["LState"])
r_phase = true_gait_phase(data["RState"])

# Generate estimated gait phase for both legs
abs_path = pathlib.Path().resolve().__str__()
abs_data_path = abs_path + "\\" + data_path # !WindowsGoo!
subprocess.run([abs_estimator_path, abs_data_path]) # !WindowsGoo!

# Read estimated gait phase for both legs
abs_output_path = abs_path + "\\output\\data.csv" # !WindowsGoo!
# opening the CSV file
est_phases = {}
with open(abs_output_path, mode ='r')as file:
   
  # reading the CSV file
  csvFile = csv.reader(file)
 
  # displaying the contents of the CSV file
  for lines in csvFile:
        est_phases[lines[0]] = np.array(lines[1:-1]).astype(np.float64)

# Compare
est_l_phase = est_phases["LGaitPhase"][1:-1]
l_err = np.subtract(l_phase, est_l_phase) # TODO: Make this not... hacky
est_r_phase = est_phases["RGaitPhase"][1:-1]
r_err = np.subtract(r_phase, est_r_phase)

# Plot
window = range(
    math.floor(plotting_window[0]*len(est_l_phase)), 
    math.floor(plotting_window[1]*len(est_l_phase)))
plt.subplot(2,1,1)
plt.subplots_adjust(left=0.1,
                    bottom=0.1,
                    right=0.9,
                    top=0.9,
                    wspace=0.4,
                    hspace=0.4)
plt.plot(est_l_phase[window])
plt.plot(l_phase[window])
plt.title("Left Gait Phase vs Time")
plt.xlabel("Time (counts)")
plt.ylabel("Percent")
plt.legend(["Estimated", "Optimal"], loc = "upper left")
plt.subplot(2,1,2)
plt.plot(est_r_phase[window])
plt.plot(r_phase[window])
plt.title("Right Gait Phase vs Time")
plt.xlabel("Time (counts)")
plt.ylabel("Percent")
plt.legend(["Estimated", "Optimal"], loc = "upper left")

plt.show()