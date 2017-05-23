import sys
import numpy as np
import scipy as np
import argparse
import codecs

def xor(s1, s2, encoding):
	# xor will take in two strings encoded as type 'encoding' and xor them together
	# requires that the strings are of equal length, naturally
	# returns the result as whatever the types inputted was
	
	# for example, if 'encoding' is 'hex', then the inputs are hex and
	# the output will also be hex
	
	raw1 = codecs.decode(s1, encoding)
	a = bytearray()
	a.extend(map(ord, raw1))
	
	raw2 = codecs.decode(s2, encoding)
	b = bytearray()
	b.extend(map(ord, raw2))
	
	c = bytearray(len(a))
	for i in range(len(a)):
		c[i] = a[i] ^ b[i]
		
	result = codecs.encode(c, encoding)
	return result
	
	
	
	
	
