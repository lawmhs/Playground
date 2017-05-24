import sys
import numpy as np
import scipy as np
import argparse
import codecs

def xor(s1, s2):
	# xor will take in two byte arrays of equal length
	# will return a corresponding byte array
	
	ret = bytearray(len(s1))
	for i in range(len(s1)):
		ret[i] = s1[i] ^ s2[i]
		
	return ret
	
	
	
	
	
