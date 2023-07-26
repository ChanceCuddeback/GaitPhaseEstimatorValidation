# Given a csv_name of a trial return a dictionary of its entries
import pandas as pd

def get_trial_dict(abs_path):
    data_frame = pd.read_csv(abs_path)
    keys = data_frame.values[:,0]
    d = {}
    for index in range(keys.size):
        d.update({keys[index]: data_frame.values[index,1:-1]})
    return d


if __name__ == "__main__":
    import pathlib
    path = pathlib.Path().resolve().__str__()
    d = get_trial_dict(path+"\\data\\1649109361258579.csv") # !WindowsGoo!
    print(d.keys())
    print(d)