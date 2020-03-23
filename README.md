# Intro

This program parses a electronic circuit from a file and simulate the output, reading the signals and writing the output in the file.

## Uses 
You have to make 2 files for a simulation, `.cir` and `.inc`. More information about the files are in [Files](#Files) section.

After you have those files which specify the circuit layout and the input signals you can run the program specifying from which file to read from.

either specify the filename (without extension) as parameter to executable or manually enter it after the program asks you.
```
./emulator examples/simple
```
OR
```
./emulator

>>Enter filename (without extension):examples/simple
```



# Files <a id='Files' />
there should be two files named simple.cir and simple.inc, they have circuit configuration and input signals

### Circuit info file (.cir)
This file includes the layout of the electronic circuits. there are two example files given in example folder (simple.cir and complex.cir).

The things to be careful about making the .cir files are:
* The format can be seen in the example below, don't use any excess spaces or linebreaks.
* The number of nodes are started from zero and are continuous.
* The first node numbers are reserved for input signals, if your input is 3, input nodes will be (0,1,2).
* Similaryly last node numbers are for output, if you specify output as 2, signals of last 2 node numbers will be output in file.


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
### Input signals file (.inc)
This file includes the signals at the input nodes. The signals has to match the number of inputs, if there is less missing inputs will be made zero, and if excess then they'll be ignored. don't use excess spaces or any characters.

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


# TODO:
A graphical interface with drag and drop to make the circuit. Maybe if I feel like it. 
