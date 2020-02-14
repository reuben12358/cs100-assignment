# Bash Command Terminal - Assignment 2

The command terminal revolves around a class called rShell which contains a series of functions that creates Command class objects out of parsed user inputs and stores them in a queue for execution. User input is passed into a Parser structure, read into the Parser's vector for storage, and then rShell uses this data to create Commands. 

The actual act of executing these commands is contained within the Command class, which is assisted by the Connector class that denotes when the next Command will execute. This README will elaborate on the purpose and function of the classes and starting from the abstract Base class to the main rShell execution class.


### The Base Class

Because our terminal utilizes a Composite design structure to represent and house readable Commands, an abstract Base class is used to establish the function `bool execute_command` that the Command and Connector classes will use to run commands and determine the condition of running the command respectively. 


### The Command Class

The Command class is what the program uses to store user input as a command that can be read by the system call `execvp()`. It houses a private member `vector <string> cmdvector`, a constructor `Command(vector<string> str_command)`, and a definition for the inherited `bool execute_command` function. It is instantied through the rShell class and must have a `vector<string>` passed into its constructor. When a Command object is instantiated, it checks to see if the parsed command specifies a known connector (&&, ||, or ;). 

If a connector is found at the end of the command string vector, the constructor will erase the connector particle leftover from the parser using `str_command.at(i).erase(str_command.at(i).size() - 1)`. If it does not find a connector, the constructor will simply call push_back on the command within the vector since there is no leftover connector.

The `bool execute_command` function reads the command stored in the `vector<string> cmdvector>` data member, which hosts an executable in element zero and the argument list in the following elements. Inside of this function, a `char* cmd` is instantiated with a number of elements equal to `cmdvector.size()`. A for loop runs through the cmdvector and sets the elements of the cmd array to their equivalent position and casts them as char pointers using `cmd[i] = (char*) cmdvector[i].c_str()`. Afterwards, the array element at `cmd[cmdvector.size()]` is assigned a NULL character.

The Command class has additional functionality for custom commands "exit" and "test". Command will recognize the keywords "exit", "test" and "[" at the beginning of the cmdvector and will execute the appropriate command. For exit, it simply exits the program. For test, the Command class is capable of recognizing whether it is the full version (test -e test.cpp) or the symbolic version ([ e- test.cpp ]).

### The Connector Class

Relative to the Command class, the Connector class and its children are very simple. Connector has two pointers as its protected data members - `Base* current` and `Base* next`. 

Connector has two children - And and Or which represent && and || respectively. 
An And connector will execute Base pointer current if Base pointer current returns true. An Or connector will do just the opposite, executing current if current returns false. 


### The Parser Struct
The Parser struct is one of the two primary objects that are essential for this program to work. The Parser struct houses a `vector<string> parsedCommand` for storing the user input. 

Parser contains a single function `bool parse(string inputStr)` to parse this input. It utilizes the tokenizer from the boost library which seperates the user input through `char` tokens. An object `char_seperator<char> sep` is declared with arguments `("", " &;|#")`. The first argument which is left blank indicates that no tokens will be dropped, while the second argument defines the characters that will cause the tokenizer to split the string into tokens. 

Tokens are pointers to split portions of the string, thus a command like `ls -a; mkdir folder` will be split into tokens that point to `ls -a`, `;`, and `mkdir folder`. A for loop using an iterator sets a temporary string to the current token, which must be dereferenced since it is a pointer. An if branch prevents spaces from being stored as commands, as well as preventing the second particle in && and || from being parsed. It also takes into account for comments denoted by # by simply stopping the loop once it reads a `#` token. 

There is also a `void clearparser()` helper function to empty the parsedCommand vector.


### The rShell Class

The rShell class possesses the main functionality of the terminal. It contains several private helper functions along with the primary function `void execute_queue()`. The primary function contains a method for grabbing the username and hostname as well as calling the parsing function for the Parser parseobj that is contained as a private data member. After the string is parsed within the parser, the helper function `void forgeCommands()` is called. After `forgeCommands()` finishes creating the actual Command and Connector objects, the function begins executing the commands from the `queue<Base*> command_queue`.

`void forgeCommands()` can be described as a parser for parsed commands. Essentially, it constructs the necessary Commands and Connector, hence the name forgeCommands. 

There are two bool variables: `bool istempassigned` and `bool isand`. These are checks which are used to assign Connectors correctly to preceding Commands so that the execute_queue() function works correctly. forgeCommands is structed in such a way that various cases of command inputs are handled using these bool variables. istempassigned is used to check if a Connector was previously assigned so that the newest Connector can be chained to it. Additionally, `forgeCommands()` also provides a means for reading in precedence for commands. By using rshell's private `int parenthesiscounter`, the `forgeCommands()` function will execute different branches depending on whether the nubmer of parenthesis is greater than zero, whether `Base* tempconnector` or `Base* tempparent` have been assigned, and whether the previous connector was an And, an Or, or a Semicolon. 

A Base* tempconnector is created to store the type of connector which the parsed command requires. For example, if the user input a semi colon, then it would create a simple Command. We did not create a semi-colon class because it was essentially a Command that would always executed, thus there is no reason to create a unique subclass for it.

`void clearVector()` is self-explanatory. It clears vectors; a handy cleanup tool. 

### List of Known Bugs

* If you place a semi-colon pair of commands inside of a parenthesis, the command will not execute properly. For example, `ls && (echo A ; echo B)` should normally execute ls and then print A and B, but the program crashes instead.
* If we try chaining pure semi-colons, the program will attempt to execute the semicolons which is incorrect. However, it will still execute the succeeding commands without error. If user inputs `;;ls` the output is `; no such file or directory` plus the files listed in ls. Really, it should not show the `;` executing and instead only just the files listed by ls.
* Using googletest on parenthesis is difficult because `forgeCommands()` and `execute_queue()` are void functions and since commands have to fork to execute, we cannot directly access the results of rshell without getting a segmentation fault in gtests.
* If there is a command in between two parenthesis without any connectors, such as `((echo A))`, then the program will throw a segmentation fault. This happens as there is no if case to deal with the connector stack being empty.
 