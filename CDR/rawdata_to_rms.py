import numpy as np
import pandas as pd

def raw_to_rms(filepath):

    counter = 0  # Set counter for 20 RMS point looping

    # Setup for 3 MyoWare inputs currently
    df = pd.DataFrame(columns=['input1', 'input2', 'input3', 'class'])

    # TODO: add processing for rms after rms_final is calculated. Store in array? list? other data structure?

    x = [None] * 20
    y = [None] * 20
    z = [None] * 20

    f = open(filepath, "r")
    while True:
        # read line and break if no other lines present
        data = f.readline().split(", ")
        if len(data) != 3:
            break

        x[counter] = int(data[0])
        y[counter] = int(data[1])
        z[counter] = int(data[2].strip("\n"))

        if counter == 19:
            rms_input1 = np.sqrt(sum(map(lambda i: i * i, x)) / 20)
            rms_input2 = np.sqrt(sum(map(lambda i: i * i, y)) / 20)
            rms_input3 = np.sqrt(sum(map(lambda i: i * i, z)) / 20)
            df = df.append({"input1": rms_input1, "input2": rms_input2, "input3": rms_input3}, ignore_index=True)

            x = [None] * 20
            y = [None] * 20
            z = [None] * 20

            counter = 0
        else:
            counter += 1

    return df  # this is just place holder... must change

raw_to_rms("/Users/aidasjakubenas/Downloads/fist.csv")