from helloworld import write_code
from testmodule import test

cdef public void greeting(int i):
    print("We are greeting you from caller!")
    write_code(i)

