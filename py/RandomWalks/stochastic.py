import numpy as np
import scipy as sp
import random as ran
import matplotlib.pyplot as plt

# quick simple random walk program in one dimension that plots the resulting walk
# will count the number of times we return to 0

ran.seed()

length = input("How long should the random walk be? ")
type(length)
length = int(length)

positions = np.zeros(length)
time = np.zeros(length)

pos = 0
hitzero = 0

for i in range(0, length):
	if(ran.randint(0,1) == 0):
		# interestingly a small change to the probabilities
		# will make the random walk barrel up or down very quickly
		# and hitzero approaches zero very rapidly
		pos = pos + 1
	else:
		pos = pos - 1
		
	positions[i] = pos
	time[i] = i
	
	if(pos == 0):
		hitzero = hitzero + 1
	
print("Our random walk has ended at: " + str(pos))
print("We came back to zero " + str(hitzero) + " times")

plt.plot(time, positions)
plt.show()
		
