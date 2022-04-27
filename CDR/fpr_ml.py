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
from statistics import mode

training_path=r"C:\Users\sdp\Desktop\GitHub\SDP\CDR\wombocombo.CSV"

names= ['input1', 'input2', 'class']

training_data = pd.read_csv(training_path, names=names)

X = training_data.iloc[:, :-1].values
y = training_data.iloc[:, 2].values

X_train, y_train = X, y

scaler = StandardScaler()
scaler.fit(X_train)

X_train = scaler.transform(X_train)

classifier = KNeighborsClassifier(n_neighbors=21)
classifier.fit(X_train, y_train)

port = 'COM12'

ser = serial.Serial(port, 9600, timeout=10)  ##(port, 115200, timeout=10)

os.system('cls')

'''prev =['none']
while True:
    dat = ser.readline().decode('utf-8', 'ignore').strip('\r\n').split(",")
    temp = [int(num.lstrip()) for num in dat]
    vals = np.asarray(temp).reshape(1,-1)
    if vals.any() > 0:
        vals = scaler.transform(vals)
        y_pred = classifier.predict(vals)
        if y_pred != prev:
            print(y_pred)
        prev = y_pred'''
x = [None] * 20
y = [None] * 20

prediction = [None] * 5
counter = 0
counter_pred = 0

prev = ['none']
while True:
    dat = ser.readline().decode('utf-8', 'ignore').strip('\r\n').split(",")
    temp = [int(num.lstrip()) for num in dat]
    try:
        x[counter] = temp[0]
        y[counter] = temp[1]
        counter += 1
        if counter == 20:
            rms_input1 = np.sqrt(sum(map(lambda i: i * i, x)) / 20)
            rms_input2 = np.sqrt(sum(map(lambda i: i * i, y)) / 20)
            rms_final = np.array([rms_input1, rms_input2]).reshape(1, -1)
            rms_final = scaler.transform(rms_final)
            y_pred = classifier.predict(rms_final)
            if y_pred != prev:

                print(y_pred)
            prev = y_pred

            counter = 0
            x = [None] * 20
            y = [None] * 20
    except:
        pass