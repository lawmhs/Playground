import sys
import numpy as np
import scipy as np
import argparse
import codecs
import binascii
import Q3 as q3
import utils as util

# takes in a ciphertext block with one encrypted line
# and detects it
# requires that each line be separated
def detectSingleBitXor(ciphertext):
	
	score = 0
	
	for line in ciphertext:
		line = line.strip()
		raw = binascii.unhexlify(line)
		res = q3.singleKeyXorDecode(raw)
		
		testscore = util.etaoinScore(res)
		
		if (testscore > score):
			score = testscore
			ret = res
	
	return ret
		



