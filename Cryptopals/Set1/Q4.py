import sys
import numpy as np
import scipy as np
import argparse
import codecs
import binascii
import Q3 as q3

txtfile = 'Q4txt.txt'
text = open(txtfile, 'r')
lines = text.readlines()

#print(lines[10])
# this will print the 10th line. From here we can tell that readlines()
# arranges the text into an array.

score = 0

for line in lines:
	line = line.strip()
	# this gets rid of trailing newline at the end of the file.
	# this will strip the string of it.
	# print(line)
	raw = binascii.unhexlify(line)
	# print(raw)
	res = q3.singleKeyXorDecode(raw)
	
	e = res.count('e', 0, len(res))
	t = res.count('t', 0, len(res))
	a = res.count('a', 0, len(res))
	o = res.count('o', 0, len(res))
	i = res.count('i', 0, len(res))
	n = res.count('n', 0, len(res))
	space = res.count(' ', 0, len(res))
	
	testscore = e + t + a + i + o + n + space
		
	if (testscore > score):
		score = testscore
		ret = res
		
print(ret)





