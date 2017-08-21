import sys
import numpy as np
import scipy as sp
import argparse
import codecs
import binascii
import utils as util

# this file is the function that detects the single key that decodes things
# will take in raw bytes

def singleKeyXorDecode(encrypted):
	score = 0
	ret = ''

	for key in range(256):
		strings = (''.join(chr(num ^ key) for num in encrypted))
		# formulate 
	
		# count ETAOIN and ' '
	
		testscore = util.etaoinScore(strings)
		
		if (testscore > score):
			score = testscore
			ret = strings
		
	return ret


