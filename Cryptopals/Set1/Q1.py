import numpy as np
import scipy as sp
import codecs as cd

string = '49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d'

raw = cd.decode(string, 'hex')

base64 = cd.encode(raw, 'base64')

test = 'SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t'

print(base64)

