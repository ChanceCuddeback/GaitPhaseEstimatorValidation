# Given a stance state signal return the true gait phase, using rising or falling edge
import numpy as np

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

if __name__ == "__main__":
    from GetTrialDict import get_trial_dict
    import pathlib
    import matplotlib.pyplot as plt
    path = pathlib.Path().resolve().__str__()
    data = get_trial_dict(path+"\\data\\1649109361258579.csv") # !WindowsGoo!
    state = data["LState"]
    gait_phase = true_gait_phase(state)

    fig, ax = plt.subplots()
    ax.plot(gait_phase[0:1000])
    ax.plot(state[0:1000])
    plt.show()