
# Problems and thoughts...

## Implement the debugger
The debugger information will be implemented as text messages sent over an ethernet connection. The application side software is implemented in the runtime to interpret debugger commands and run the program line by line using a function that waits after each line of source code. When the line is run, then the runtime debugger reports that it's done back to the debugger UI. Design the protocol and messages. The runtime is smart enough to be able to run if the debugger is not active. 