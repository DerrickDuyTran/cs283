1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines that the command’s output is fully received by looking for an EOF. When the client how many bytes to expect, it can use recv() multiple times until the message is received.   

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol should define the beginning and end of a command by adding delimiters, typically null terminators where it detects both the beginning and the end
of the command. If this isn't done, a command may be split prematurely into seperate commands and commands will merge into one. 

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols remember interactions across several interactions between the server while stateless protocols see each interaction as independent and assume that it has
all the required information inside of it, so it doesn't remember anything from previous interactions.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

Although UDP is unreliable, it is used because messages are packaged and delivered as complete units so they preserve message boundaries. Along with that, it is faster than TCP is because it doesn't create a connection, finally, using UDP, messages can reach multiple clients at one time. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The socket programming model is the programming interface provided by the operation system that enables applications to write network programs
