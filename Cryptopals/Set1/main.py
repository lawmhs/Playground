import sys
import numpy as np
import scipy as np
import argparse
import codecs
import Q2 as q2

chce = range(101)
for i in range(1, 101):
	chce[i] = str(chce[i])

if __name__ == '__main__':
	argv = sys.argv[1:]
	
	parser = argparse.ArgumentParser()
	parser.add_argument('-c', '--challenge', required=True, choices=chce)
	io_args = parser.parse_args()
	challenge = io_args.challenge
		
	if challenge == '1':
	# convert hex to base64
		hexstr = '49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d'
		b64 = codecs.encode(codecs.decode(hexstr, 'hex'), 'base64').decode()
		print(b64)	

	if challenge == '2':
	# Fixed XOR, write a function that takes two equal-length buffers and produces their XOR combination

		string1 = '1c0111001f010100061a024b53535009181c'
		string2 = '686974207468652062756c6c277320657965'
		
		result = q2.xor(string1, string2, 'hex')
		print(result)
		
		
	if challenge == '3':
		string = '1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736'
		score = 0
		raw = codecs.decode(string, 'hex')
		rawarray = bytearray()
		rawarray.extend(map(ord, raw))
		result = bytearray(len(rawarray))
		
		#for key in range(0, 15):
			#for i in range(len(rawarray)):
				
				
			
			
			
			
