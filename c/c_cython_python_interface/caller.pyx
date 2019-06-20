from helloworld import write_code
from testmodule import test
import numpy

cdef public struct Animal:
    int age
    int size
    char *name

cdef public int greeting(int i):
    print("We are greeting you from caller!")
    write_code(i)
    cdef int x = test(i)
    a = numpy.zeros(10)
    print(a)
    return x

cdef public void animal_greeting(Animal *a):
    print("Hello!, I am a " + a.name.decode("utf-8") + ". I am " + str(a.age) + " years old.")

cdef public void test_cdef():
    print("we can run a cdef function within caller!")

