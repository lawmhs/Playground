import sys
import numpy as np
import scipy as np
import argparse
import codecs
import binascii

# this file is the function that detects the single key that decodes things
# will take in raw bytes

def singleKeyXorDecode(encrypted):
	score = 0
	ret = ''

	for key in range(256):
		strings = (''.join(chr(num ^ key) for num in encrypted))
		# formulate 
	
		# count ETAOIN and ' '
	
		e = strings.count('e', 0, len(strings))
		t = strings.count('t', 0, len(strings))
		a = strings.count('a', 0, len(strings))
		o = strings.count('o', 0, len(strings))
		i = strings.count('i', 0, len(strings))
		n = strings.count('n', 0, len(strings))
		space = strings.count(' ', 0, len(strings))
	
		testscore = e + t + a + i + o + n + space
		
		if (testscore > score):
			score = testscore
			ret = strings
		
	return ret


