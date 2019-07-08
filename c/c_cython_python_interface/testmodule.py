import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def test(i):
    print("in test module, we wrote this many lines of code : " + str(i))
    print("but using python in the test module, we can write " + str(2 * i) + " lines of code")
    print(np.zeros(10))
    data = pd.read_csv('mathewshirt.csv')
    print(data.head())
    return 2 * i

def roarAnimal(a):
    print("I am a " + str(a['name']) + " and hear me roar!")

def graph(x, y):
    plt.plot(x, y)
    plt.ylabel('Randomly generated numbers')
    plt.show()
