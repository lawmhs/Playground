import readtext as rt
import numpy as np

from libc.stdlib cimport malloc, free
from libc.string cimport strcmp

encoding = "utf-8"

cdef public struct Corpus:
    int size
    char **strs

cdef extern from "Python.h":
    char* PyUnicode_AsUTF8(object unicode)

## convenient function found on stack overflow: https://stackoverflow.com/questions/17511309/fast-string-array-cython
cdef char ** to_cstring_array(list_str):
    cdef char ** ret = <char **>malloc(len(list_str) * sizeof(char *))
    for i in xrange(len(list_str)):
        ret[i] = PyUnicode_AsUTF8(list_str[i])
    return ret

# test function to make sure everything is working
cdef public int greeting(int number):
    number = number + 1
    print("Hello! The current value of number is " + str(number))
    return number

# This function will take in a file name
cdef public void fillCorpus(char * file_name, Corpus *c):
    f = file_name.decode(encoding)
    print("The file passed in is " + f)
    strings = rt.tokenize(file_name.decode(encoding))

    # now we want to fill the corpus structure passed in:
    c.size = len(strings)
    c.strs = to_cstring_array(strings)

# This function will filter out the strings in the corpus's str field
# and also remove the punctuation
cdef public void filter(Corpus *c):
    words = []
    for i in xrange(c.size):
        words.append(c.strs[i].decode(encoding))
    res = rt.filterCorpus(words)
    c.size = len(res)
    c.strs = to_cstring_array(res)


    