import numpy as np

def mergeSort(array):
	
	size = np.size(array)
	# size of the array we are sorting
	
	sort_array = np.zeros(size)
	for i in range(0, size):
		sort_array[i] = array[i]
	# could also use copy.copy(array) here
	
	mergeSortSplit(array,sort_array, 0, size)
	# python rounds down
	return sort_array
	
def mergeSortSplit(ref_array, sort_array, startind, endind):
	
	if (endind - startind < 2):
		# the size of the array is 1, so it is trivially sorted
		return
		
	midind = int((startind + endind) / 2)
	# find the midpoint of the array
	
	mergeSortSplit(ref_array, sort_array, 0, midind)
	mergeSortSplit(ref_array, sort_array, midind, endind)
	
	merge(ref_array, sort_array, startind, midind, endind)
	
def merge(ref_array, sort_array, startind, midind, endind):
	
	i = startind
	j = midind
	
	k = startind
	while k < endind:
		if (i < midind and (j >= endind  or (ref_array[i] <= ref_array[j]))):
			sort_array[k] = ref_array[i]
			i += 1
		else:
			sort_array[k] = ref_array[j]
			j += 1
		
		k += 1
	
		
	
	
	
