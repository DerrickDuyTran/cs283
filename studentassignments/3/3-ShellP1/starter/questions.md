1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice for this application because it reads input line by line, essential to step 3 where we have to read one line at a time. 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  Using malloc is better here because it allows more flexibility, especially with this assignment where we handle unknown input sizes. If we used a fixed-size array, we'd have to assign a specific size which could lead to buffer overflows. Instead, with malloc, we use the required amount of memory for each input.  


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Removing leading and trailing spaces here was necessary because if not, it would cause errors if a command did have leading spaces with being able to recognize the command. For example, given "    ls", this could be misinterpreted because of the leading spaces and the command wouldn't be executed, but with removing it, it'd be recognized as ls no matter what. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  One implmenetation for our custom shell could be command > file, and this would just redirect the output of a command into a file, the difficulties with this would be handling the file since the shell has to open the file and add the output and overwriting the file correctly.
    Another implemntation could be command >> file, and this would do the same thing as the above redirection, but instead of rewriting, it'd be appending. It'd hold the same difficulty of handling the file for writing, but instead of overwriting, the difficulty of appending to the existing file would be added.
    Finally, we can do command 2> file, where this writes the stderr of the ls to a file. The challenge would be differentiating from stderr and stdout and the same file handling issues mentioned before. 

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Pipes are typically involved with connecting the output of one command to the input of another command which allows us to combine multiple commands together, while redirection sends input, output, or errors of a command to a specified place, typically files. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  You want to separate these in a shell because, for example, if you are trying to redirect the output of a command to a file, you'd want the output only which you can do with >, but, if they were to be the same, you'd get either one which could cause errors instead of being able to choose between > and 2>. This also helps with error handling and debugging, where you can differentiate from errors and regular output and look at the errors only. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  If a command fails, our shell should output it as stderr for the reasons listed in the previous question, for flexibility of redirecting output and piping commands together, keeping the handling of errors and stdout separate is essential for that flexibility. We should be able to have the option of merging stdout and stderr, we should have specific redirect syntax for this, being 2>&1, which should redirect stderr to the typical place for stdout. 