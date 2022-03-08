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

training_path=r"C:\Users\Ryan\Documents\GitHub\SDP\CDR\data\train_data.csv"

names= ['input1', 'input2', 'class']

training_data = pd.read_csv(training_path, names=names)

X = training_data.iloc[:, :-1].values
y = training_data.iloc[:, 2].values

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.20)

scaler = StandardScaler()
scaler.fit(X_train)

X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)

classifier = KNeighborsClassifier(n_neighbors=5)
classifier.fit(X_train, y_train)

port = 'COM7'

ser = serial.Serial(port, 9600, timeout=10)  ##(port, 115200, timeout=10)

os.system('cls')

prev = ['none']
while True:
    x = ser.readline().decode('utf-8', 'ignore').strip('\r\n').split(", ")
    temp = [int(num) for num in x]
    vals = np.asarray(temp).reshape(1,-1)

    if vals.any() > 0:
        y_pred = classifier.predict(vals)
        if y_pred != prev:
            print(y_pred)
        prev = y_pred






saved_model = pickle.dumps(classifier)

print(confusion_matrix(y_test, y_pred))
print(classification_report(y_test, y_pred))