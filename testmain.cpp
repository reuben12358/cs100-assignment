#include <iostream>
#include <string> 
#include <vector> 
#include <list>  
#include "../src/base.h"
#include "testshell.h"
#include "testparser.h"
#include "../src/connector.h"
#include "testcommand.h"
#include "../src/and.h"
#include "../src/or.h"
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
    string str1;

    getline(cin, str1);

    mainShell.parserObj.parse(str1);

    mainShell.forgeCommands();
    
    //cout << "forging done" << endl;

    mainShell.execute_queue();
 
    return 0;
}