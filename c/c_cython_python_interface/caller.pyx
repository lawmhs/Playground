from helloworld import write_code
import testmodule as ts
import numpy as np

cdef public struct Animal:
    int age
    int size
    char *name

cdef public int greeting(int i):
    print("We are greeting you from caller!")
    write_code(i)
    cdef int x = ts.test(i)
    a = np.zeros(10)
    print(a)
    return x

cdef public void animal_greeting(Animal *a):
    print("Hello!, I am a " + a.name.decode("utf-8") + ". I am " + str(a.age) + " years old.")
    anim = {}
    anim['age'] = a.age
    anim['size'] = a.size
    anim['name'] = a.name.decode("utf-8")
    print(anim)
    ts.roarAnimal(anim)

cdef public void test_cdef():
    print("we can run a cdef function within caller!")

cdef public void graph(int *x, int x_size, int *y, int y_size):
    print("we are now graphing the arrays")
    x_py = np.zeros(x_size)
    for i in range(0, x_size):
        x_py[i] = x[i]
    
    y_py = np.zeros(y_size)
    for i in range(0, y_size):
        y_py[i] = y[i]

    ts.graph(x_py, y_py)
    print("finished graphing!")
    
