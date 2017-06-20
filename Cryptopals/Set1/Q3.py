import sys
import numpy as np
import scipy as np
import argparse
import codecs
import Q2 as q2

# First let's test the Q2 function
str1 = '1c0111001f010100061a024b53535009181c'
str2 = '686974207468652062756c6c277320657965'

a = codecs.decode(str1, 'hex_codec')
b = codecs.decode(str2, 'hex_codec')

c = q2.xor(a, b)
print(c)

d = codecs.encode(c, 'hex_codec')
print(d)


