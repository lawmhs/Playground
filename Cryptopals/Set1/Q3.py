import sys
import numpy as np
import scipy as np
import argparse
import codecs
import binascii

encrypted = '1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736'
# encrypted is encoded by a single character

print(binascii.unhexlify(encrypted))

raw = binascii.unhexlify(encrypted)
# trying to do "ETAOIN SHRDLU" myself:

score = 0
ret = ''

for key in range(256):
	strings = (''.join(chr(num ^ key) for num in raw))
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
		
print(ret)
		


