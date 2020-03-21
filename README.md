# Intro

This program is supposed to parse a electronic circuit from a file and simulate th output in a file.

I'm still working on it. 


# Files
there should be two files named simple.cir and simple.inc, they have circuit configuration and input signals

simple.inc
```
110
111
100
011
000
010
101
```

simple.cir
```
nodes:6
input:3
output:2
gates:3
AND:1-2->4
OR:2-3->5
NOT:5->6
```

the program will parse them and generate a circuit structure as per the .cir file and output the signal output of circuit. 

# TODO:
A lot to do, it can only read the circuit configuration for now. 
