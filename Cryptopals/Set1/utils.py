import sys
import numpy as np
import scipy as np
import argparse
import codecs
import binascii

# takes in UTF-8 / raw bytes and scores it using etaoin and space
# returns an integer score
def etaoinScore(text):
	e = text.count('e', 0, len(text))
	t = text.count('t', 0, len(text))
	a = text.count('a', 0, len(text))
	o = text.count('o', 0, len(text))
	i = text.count('i', 0, len(text))
	n = text.count('n', 0, len(text))
	space = text.count(' ', 0, len(text))
	
	return e + t + a + o + i + n + space
