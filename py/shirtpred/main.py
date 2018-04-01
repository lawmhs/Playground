import numpy as np
import scipy as sp
import sklearn as sk
import pandas as pd
import csv

data = []

with open('mathewshirt.csv') as csvDataFile:
	csvReader = csv.reader(csvDataFile)
	'''
	for row in csvReader:
		print(row)
	'''
	for row in csvReader:
		data.append(row)
	
data = np.asarray(data)
n, d = len(data), len(data[0])
print(data)
print(n)
print(d)
data = np.reshape(data, (n, d))
print(data)

newdata = []
for i in range(0, len(data)):
	for j in range(0, len(data[i])):
		print(data[i][j])
