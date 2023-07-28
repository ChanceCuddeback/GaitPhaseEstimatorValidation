from Compile import compile
from GetTrialDict import get_trial_dict
from TrueGaitPhase import true_gait_phase
import subprocess
import numpy as np
import pandas as pd
import matplotlib as plt

# Compile the code 
compile()
# Pull in the data
data = get_trial_dict("data/1649109361258579.csv")
# Generate true gait phase
l_phase = true_gait_phase(data["LState"])
r_phase = true_gait_phase(data["RState"])
# Generate estimated gait phase
subprocess.run(["./build/GeteOutput.exe", "data/1649109361258579.csv"])
# Read estimated gait phase

# Compare

# Plot