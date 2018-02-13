import numpy as np
import scipy as sp
import sklearn as sk
import pandas as pd
import csv

with open('mathewshirt.csv') as csvDataFile:
	csvReader = csv.reader(csvDataFile)
	for row in csvReader:
		print(row)
