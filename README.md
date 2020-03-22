# Intro

This program parses a electronic circuit from a file and simulate the output, reading the signals and writing the output in the file.

## Uses 
You have to make 2 files for a simulation, `.cir` and `.inc`. More information about the files are in (Files)[Files] section.

After you have those files which specify the circuit layout and the input signals you can run the program specifying from which file to read from.

either specify the filename (without extension) as parameter to executable or manually enter it after the program asks you.
```
./emulator simple
```
OR
```
./emulator

>>Enter filename (without extension):simple
```



# Files
there should be two files named simple.cir and simple.inc, they have circuit configuration and input signals

### Circuit info file (.cir)
This file includes the layout of the electronic circuits. there are two example files given in example folder (simple.cir and complex.cir).

the format can be seen in the example below, don't use any excess spaces or linebreaks.

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
A lot to do, it can only read the circuit configuration for now. 
