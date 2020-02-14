#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "base.h"
#include "rshell.h"
#include "parser.h"
#include "connector.h"
#include "command.h"
#include "and.h"
#include "or.h"
#include <boost/tokenizer.hpp>
#include <unistd.h>


using namespace std;
using namespace boost;

//main.cpp contains the method for running the Command Shell
//as well as Parsing in commands from the command line. 

//Currently working on PARSER

//Abandoned the usage of strtok(). I simply don't know how to use it properly and I believe substr will do the same thing. 

int main() {
    rshell mainShell;

    mainShell.execute_queue();
 
    return 0;
}