def readLinesIntoListOfString(fileName):

    ret = []
    with open(fileName, 'r') as f:
        for line in f:
            strings = line.split(' ')
            for s in strings:
                ret.append(s)
    return ret
        
