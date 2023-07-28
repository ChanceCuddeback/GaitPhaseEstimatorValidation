from Compile import compile
from GetTrialDict import get_trial_dict
from TrueGaitPhase import true_gait_phase
import subprocess
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import pathlib 


# TODO: Perfrom for all files in data and aggregate results
data_path = "data\\1649109361258579.csv" # !WindowsGoo!
plotting_window = [0.2, 0.8] # Starting percent to final percent

# Compile the code 
compile()

# Pull in the data
data = get_trial_dict(data_path)

# Generate true gait phase
l_phase = true_gait_phase(data["LState"])
r_phase = true_gait_phase(data["RState"])

# Generate estimated gait phase for both legs
abs_path = pathlib.Path().resolve().__str__()
abs_data_path = abs_path + "\\" + data_path
subprocess.run([abs_path+"\\build\\GetOutput.exe", abs_data_path]) # !WindowsGoo!

# Read estimated gait phase for both legs
abs_output_path = abs_path + "\\output\\data.csv" # !WindowsGoo!
est_phases = get_trial_dict(abs_output_path)

# Compare
# l_err = np.subtract(l_phase*100, est_phases["LGaitPhase"][0:len[l_phase]]) # TODO: Make this not... hacky
r_err = np.subtract(r_phase*100, est_phases["RGaitPhase"][0:len(r_phase)])

# Plot
plt.subplot(2,1,1)
# plt.plot(l_err)
# plt.title("Left Error vs Time")
# plt.xlabel("Time (counts)")
# plt.ylabel("True - Estimated (%)")
plt.subplot(2,1,2)
plt.plot(r_err)
plt.title("Right Error vs Time")
plt.xlabel("Time (counts)")
plt.ylabel("True - Estimated (%)")
plt.show()