# Linux Shell
3 major functionalities in my shell:
1. Displays a random quote followed a prompt for the user to enter the command (read all the quotes from quotes.txt when the shell starts up).
2. It supports any number of commands connected by pipe (similar to bash):
```
Usage: command1 | command2 | command3 | command4 ....
```
4. It allows for 2 commands to pipe data to each other (new). This is achieved by having 2 pipes (= signifies this)
```
Usage: command1 = command2 
```
# Sample Run
```
mysh
<random quote>
# cat data1.txt data2.txt     <-- simple command, without | or =
..
<random quote>
# cat data1.txt | cat | cat | cat data1.txt data2.txt | cat | cat | cat
...
<random quote>
# computer = user
...
<random quote>
# mecho -s 5 -n 10 7 = mecho 3
...
```
