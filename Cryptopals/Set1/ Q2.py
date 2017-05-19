import numpy as np
import scipy as sp
import codecs as cd

string = '1c0111001f010100061a024b53535009181c'
raw = cd.decode(string, 'hex')
rawarray = bytearray()
rawarray.extend(map(ord, raw))


