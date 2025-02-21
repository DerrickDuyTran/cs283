1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  fork lets us keep running the current program by making a copy of it while execvp ends the current program altogether. 

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If it fails, it returns -1, my implementation handles this by priting and error using perror("fork").

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  It looks in the directories in the path environment variable, if it is given a full path it'll use that instead. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  Using wait in the parent process makes the parent wait until the child is finished, otherwise, the child might continue without knowing. 

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  It gets the exit code of the child so that we know whether or not it worked. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  It makes everything inside quotes one argument. 

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  It combines removing extra spaces and the argument parsing into one function, there weren't any problems. 

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are used for event-driven communication, so they deliver notificaitons immediately and can interrupt the process at any point as it is executing. 

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL kills a process instantly, it can't be ignored. SIGTERM asks a process to stop but it can be ignored. SIGINT is done with CTRL C and it asks the process to terminate gracefully. 

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  A process pauses when it receives a SIGSTOP and it cannot be ignored because thats function is to forcefully stop a process. 
