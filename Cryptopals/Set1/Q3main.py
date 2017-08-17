import sys
import numpy as np
import scipy as sp
import argparse
import codecs
import binascii
import Q3 as q3

encrypted = '1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736'
# encrypted is encoded by a single character

print(binascii.unhexlify(encrypted))

raw = binascii.unhexlify(encrypted)

print(q3.singleKeyXorDecode(raw))
