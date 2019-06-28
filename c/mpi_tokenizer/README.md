This project is to set up a small proof of concept, to have 4 MPI nodes, each calling python of their own to work on data as it passes through.
The first node will take in a text file and output it as an array of strings
The second node will take in this array, pass it through to a python function which will tokenize the text, then pass it back down as an array of strings.
The third node will take in this array, pass it through to a python function which will filter the array / list, then pass it back down as an array of strings.
The last node will be a dump, perhaps print everything.