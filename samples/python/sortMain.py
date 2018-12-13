import numpy as np
import sort as s

# implementations of a few sorts


# this is so that we can adjust the size of the array we sort easily
SIZE = 3

array = np.arange(SIZE)
# array that we sort

np.random.shuffle(array)

# fill the array
#randomize the array

print(array)
result = s.mergeSort(array)
print(result)
