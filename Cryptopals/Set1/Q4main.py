import sys
import numpy as np
import scipy as np
import argparse
import codecs
import binascii
import Q3 as q3
import Q4 as q4

txtfile = 'Q4txt.txt'
text = open(txtfile, 'r')
lines = text.readlines()

print(q4.detectSingleBitXor(lines))
