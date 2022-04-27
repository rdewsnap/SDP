import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import classification_report, confusion_matrix
import pickle
import serial
import os

def train_model(myoware_inputs=2, neighbors=21, training_path=r"C:\Users\sdp\Desktop\GitHub\SDP\CDR\wombocombo.CSV"):

    names = ['class'] * (myoware_inputs + 1)
    for i in range(0, myoware_inputs):
        names[i] = 'input' + str(i+1)

    training_data = pd.read_csv(training_path, names=names)

    x = training_data.iloc[:, :-1].values
    y = training_data.iloc[:, myoware_inputs].values

    x_train, y_train = x, y

    scale = StandardScaler()
    scale.fit(x_train)

    x_train = scale.transform(x_train)

    classifier = KNeighborsClassifier(n_neighbors=neighbors)
    classifier.fit(x_train, y_train)

    # filename for trained model saved as a pickle for future use
    filename = 'trained_model.sav'
    pickle.dump(classifier, open(filename, 'wb'))


def run_model_live_inputs(port='COM12', scale_type='standard', model_name='trained_model.sav'):

    if scale_type == 'standard':
        scale = StandardScaler()
    else:
        raise ValueError("Scaler should be the same as trained model")

    classifier = pickle.load(open(model_name, 'rb'))

    ser = serial.Serial(port, 9600, timeout=10)  # (port, 115200, timeout=10)
    os.system('cls')

    prev = ['none']

    x = [None] * 20
    y = [None] * 20

    counter = 0

    while True:
        dat = ser.readline().decode('utf-8', 'ignore').strip('\r\n').split(",")

        if len(dat[0]) == 4:
            temp = [int(num.lstrip()) for num in dat]
            x[counter] = temp[0]
            y[counter] = temp[1]
            counter += 1
            if counter == 20:
                rms_input1 = np.sqrt(sum(map(lambda i: i * i, x)) / 20)
                rms_input2 = np.sqrt(sum(map(lambda i: i * i, y)) / 20)
                rms_final = np.array([rms_input1, rms_input2]).reshape(1, -1)
                rms_fin = scale.transform(rms_final)
                y_pred = classifier.predict(rms_fin)
                print(y_pred)
                if y_pred != prev:
                    print(y_pred)
                prev = y_pred

                counter = 0
                x = [None] * 20
                y = [None] * 20

run_model_live_inputs()