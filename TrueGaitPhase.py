# Given a stance state signal return the true gait phase, using rising or falling edge
import numpy as np

def true_gait_phase(state_array, on_rising=False):
    # Find the location of all of the edges of concern
    edges = list(map(lambda x: x > 0 if on_rising else x < 0, np.diff(state_array)))
    locs = edges.index(1)
    # Generate sawtooth from 0-1 for each of the steps (ie gait phase)
    gait_phase = np.zeros_like(state_array)
    


    return edges, locs

if __name__ == "__main__":
    from GetTrialDict import get_trial_dict
    import pathlib
    import matplotlib.pyplot as plt
    path = pathlib.Path().resolve().__str__()
    data = get_trial_dict(path+"\\data\\1649109361258579.csv") # !WindowsGoo!
    edges = true_gait_phase(data['LState'])

    fig, ax = plt.subplots()
    ax.plot(edges[0:1000])
    ax.plot(data['LState'][0:1000])
    plt.show()
