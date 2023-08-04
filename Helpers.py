
import pandas as pd
import subprocess
import pathlib
import os
import numpy as np

# Given a stance state signal return the true gait phase, using rising or falling edge
def true_gait_phase(state_array, on_rising=True):
    # Find the location of all of the edges of concern
    edges = list(map(lambda x: x > 0 if on_rising else x < 0, np.diff(state_array)))
    locs = list()
    for i in range(len(edges)):
        locs.append(i+1) if edges[i] > 0 else None # Plus one because diffs occur at the prior index
    # Generate sawtooth from 0-1 for each of the steps (ie gait phase)
    gait_phase = np.zeros(locs[0]) # pad the beginning with zeros
    for i in range(len(locs)):
        # End condition
        if i == len(locs)-1:
            gait_phase = np.append(gait_phase, np.ones(len(edges)-locs[-1])) # pad the end with ones
            break
        # Generate this steps sawtooth
        sawtooth = np.array(list(range((locs[i+1])-(locs[i]))))
        sawtooth = np.divide(sawtooth, max(sawtooth))
        gait_phase = np.append(gait_phase, sawtooth)
    return gait_phase*100

# Given a csv_name of a trial return a dictionary of its entries
def get_trial_dict(abs_path):
    data_frame = pd.read_csv(abs_path)
    keys = data_frame.values[:,0]
    d = {}
    for index in range(keys.size):
        d.update({keys[index]: data_frame.values[index,1:-1]})
    return d

# Compile the Gait Phase estimator, requires the intall of GCC. Tested on windows 10.
def compile():
    # Get the current path
    path = pathlib.Path().resolve().__str__()
    # Delete the old build file if it exists
    _delete_old_build(path)
    # Check for g++
    _handle_run(["g++", "--version"], should_supress=True)
    # Compile
    abs_build_path = path+"\\build\\GetOutput" # !WindowsGoo!
    executable_ext = ".exe" # !WindowsGoo!
    _handle_run(
        ["g++",
         "-std=c++20",
         path+"\src\*.cpp",
         path+"\GetOutput.cpp", 
         "-o",
          abs_build_path
          ], should_supress=True)
    return abs_build_path+executable_ext

# ============================ Private functions ============================
def _handle_run(args, timeout=60, should_supress=False):
    try:
        subprocess.run(
            args, timeout=timeout, check=True, 
            stdout= subprocess.DEVNULL if should_supress else None
        )
    except FileNotFoundError as exc:
        print(f"Process with {args} failed because the executable could not be found.\n{exc}")
        quit()
    except subprocess.CalledProcessError as exc:
        print(
            f"Process with {args} failed because it did not return a successful return code. "
            f"Returned {exc.returncode}\n{exc}"
            )
        quit()
    except subprocess.TimeoutExpired as exc:
        print(f"Process with {args} time out.\n{exc}")
        quit()

def _delete_old_build(rel_path):
    abs_path = rel_path+"\\build\\GetOutput.exe" # !WindowsGoo!
    if os.path.exists(abs_path):
        os.remove(abs_path)