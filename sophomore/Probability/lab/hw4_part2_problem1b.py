"""
Sample code of HW4-Part II, Problem 1
"""

import matplotlib.pyplot as plt
import pickle
import numpy as np
from scipy import linalg

myfile = open('hw4_part2_problem1_data.pickle', 'rb')
mydict = pickle.load(myfile)

X_train = mydict['X_train']
X_test = mydict['X_test']
Y_train = mydict['Y_train']
Y_test = mydict['Y_test']

predictive_mean = np.empty(X_test.shape[0])
predictive_std = np.empty(X_test.shape[0])

sigma = 0.1
sigma_f = 1.0
ls = [0.5,0.1,2]

for cur in ls:
    #-------- Your code (~10 lines) ---------
    cov = np.empty((X_train.shape[0]+1,X_train.shape[0]))
    for i,x in enumerate(X_train):
        for j,y in enumerate(X_train):
            cov[i][j] = sigma_f**2 * np.exp(-(x-y)**2/(2*cur**2))+sigma**2*(1 if i==j else 0)
    for it,x_test in enumerate(X_test):
        for i,x in enumerate(X_train):
            cov[-1][i] = sigma_f**2 * np.exp(-(x-x_test)**2/(2*cur**2))
        predictive_mean[it] = np.dot(np.dot(cov[-1],linalg.inv(cov[:-1]+sigma**2*np.identity(X_train.shape[0]))),np.transpose(Y_train))
        predictive_std[it] = np.sqrt(sigma_f**2+sigma**2-np.dot(np.dot(cov[-1],linalg.inv(cov[:-1]+sigma**2*np.identity(X_train.shape[0]))),np.transpose(cov[-1])))
    #---------- End of your code -----------

    # Optional: Visualize the training data, testing data, and predictive distributions
    fig = plt.figure()
    plt.plot(X_train, Y_train, linestyle='', color='b', markersize=5, marker='+',label="Training data")
    plt.plot(X_test, Y_test, linestyle='', color='orange', markersize=2, marker='^',label="Testing data")
    plt.plot(X_test, predictive_mean, linestyle=':', color='green')
    plt.fill_between(X_test.flatten(), predictive_mean - predictive_std, predictive_mean + predictive_std, color='green', alpha=0.13)
    plt.fill_between(X_test.flatten(), predictive_mean - 2*predictive_std, predictive_mean + 2*predictive_std, color='green', alpha=0.07)
    plt.fill_between(X_test.flatten(), predictive_mean - 3*predictive_std, predictive_mean + 3*predictive_std, color='green', alpha=0.04)
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.show()
