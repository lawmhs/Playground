import sys
import numpy as np
import scipy as np
import argparse
import codecs
import binascii
import utils as util
import Q5 as q5

plaintext = "Burning 'em, if you ain't quick and nimble I go crazy when I hear a cymbal"
key = "ICE"

rawPlaintext = plaintext.encode()
rawKey = key.encode()

print(rawPlaintext)
print(rawKey)

