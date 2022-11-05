3 major functionalities in my shell:
1. Display a random quote followed the prompt for the user to enter the command (read all the quotes from quotes.txt when the shell starts up).
2. It should be able to support any number of commands connected by pipe (similar to bash):
    Usage: command1 | command2 | command3 | command4 ....
3. It should allow 2 commands to pipe the data to each other (new). This can be achieved by having 2 pipes (= signifies that!)
    Usage: command1 = command2 
