1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

It ensures that all child processes complete before continuing by putting all the processes in pids[] and then using the waitpid() to wait for each process to complete. If I didn't call waitpid(), the processes would become terminated but still consume resources which is bad.  

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

If you leave the pipes open, it could lead to a descriptor leak. Eventually, this would cause your program to run out of available file descriptors which will make you unable to open new files and pipes. 

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

If cd were used as an external command, its possible that the directory change after using cd would be lost because it'd run in a child process which terminates the command when the child process is temrianted while the parent shell stays the same, this reverses the effects of cd. 

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

Instead of using the fixed-size array in the current struct, I'd use malloc and realloc instead to resize the array to whatever number of piped commands I'd need. This would make it harder to manage memory.
