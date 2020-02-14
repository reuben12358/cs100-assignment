#ifndef RSHELL_H
#define RSHELL_H
 
#include <iostream>
#include <queue>
#include <unistd.h>
#include "and.h"
#include "or.h"
#include "semicolon.h"
#include "pipe.h"
#include "command.h"
#include "base.h"
#include <stack>
#include <climits>
#include "parser.h"

using namespace std;

class rshell {
    private:
        // queue<Base*> command_queue;
        vector<string> str1;
        Parser parserObj;
        
    public:
        void execute_queue() {
            bool exitbool = false;
            while (!exitbool) {            
                char* username = getlogin();
                if (!username) 
                    perror ("no username");
                
                char hostname[HOST_NAME_MAX];
                gethostname(hostname, HOST_NAME_MAX);
            
                cout << username << "@" << hostname;
                cout << " $ ";

                string str;
            
                getline(cin, str);
            
                if (str == "exit") {
                    exitbool = true;
                    break;
                }

                parserObj.parse(str);

                int parenthesiscounter = 0;

                for (int i = 0; i < parserObj.parsedCommand.size(); ++i) { //For loop testing for uneven parenthesis.
                    if (parserObj.parsedCommand.at(i) == "(") {
                        parenthesiscounter++;
                    }
                    else if (parserObj.parsedCommand.at(i) == ")") {
                        parenthesiscounter--;
                    }
                }

                if (parenthesiscounter != 0) {
                    perror("Error: Uneven Number of parenthesis\n");
                }
                else {    
                    forgeCommands();
                }
                
                parserObj.clearparser();
            }    
        };
        
    private: /* helper functions */
        void forgeCommands() {
            /*
            Base* tempconnector;
            bool istempassigned = false;
            bool isand;

            // Base* tempparent;
            // bool isparentassigned = false;
            // bool isparentand;

            stack <Base*> tempstack; //To ensure that multiple nested parenthesis statements can be managed, we utilized a stack.
            stack <bool> boolstack; //Used to ensure that commands can be chained according to their connectors properly.

            parenthesiscounter = 0;

            bool go_next = false; //Makes sure that every time we use the open parenthesis, a new command is pushed into the stack. For nesting.

            // bool issemicolon;

            for (int a = 0; a < parserObj.parsedCommand.size(); ++a) {
                string cmdstring = parserObj.parsedCommand.at(a);

                // fixme: add parenthesis to & | ;

                if (cmdstring == "&") { //Checks if it is a connector. 
                    if (parenthesiscounter == 0) { //Function is outisde parenthesis.
                        if (istempassigned) { //There is a function that was assigned before the current function.
                            if (!str1.empty()) { //If there are no commands in the vector.
                                Command* cmd = new Command(str1);
                                clearVector();
                            
                                Base* tempand;

                                if (isand) { //Checks if the previous command was an and.
                                    tempand = new And(tempconnector, cmd);    
                                }
                                else {
                                    tempand = new Or(tempconnector, cmd);
                                }

                                tempconnector = tempand; //Now that tempconnector has been chained, we can put tempand in tempconnector.
                            }
                            isand = true;   
                        }
                        else { //If there was no function assigned before the current function. 
                            tempconnector = new Command(str1);
                            clearVector();
                            istempassigned = true;
                            isand = true;
                        }
                    }
                    else { //This branch launches if it is currently inside of a parenthesis.
                        // if (isparentassigned) {
                        //     Base* cmd;

                        //     if (!str1.empty()) {
                        //         cmd = new Command(str1);
                        //         clearVector();
                        //     }
                        //     // else {
                        //     //     cmd = tempconnector;
                        //     // }

                        //     Base* tempand;
                            
                        //     if (isparentand) {
                        //         tempand = new And(tempparent, cmd);
                        //     }
                        //     else {
                        //         tempand = new Or(tempparent, cmd);
                        //     }

                        //     tempparent = tempand;
                        //     isparentassigned = true;
                        //     isparentand = true;
                        // }
                        // else {
                        //     tempparent = new Command(str1);
                        //     clearVector();
                        //     isparentassigned = true;
                        //     isparentand = true;
                        // }
                        if ((tempstack.empty()) || (go_next == true)) { //If tempstack is empty then it is the very first element of a parenthesis and if go_next is true than this is allso the case.
                            Command* cmd = new Command(str1);
                            clearVector();
                            //cout << "executed stack empty\n";
                            tempstack.push(cmd);
                            boolstack.push(true); // true for and 
                            if (go_next) {
                                //cout << "executed go_next\n";
                                go_next = false;
                            }
                        }
                        else { //If a stack is not empty and the current top is a command in between the starting fo the nest and the ending of the nest.
                            if (boolstack.top() == true) { 
                                Command* cmd = new Command(str1);
                                clearVector();
                                And* s_and = new And(tempstack.top(), cmd);
                                tempstack.pop();
                                tempstack.push(s_and);
                                boolstack.pop();
                                boolstack.push(true);
                            }
                            else {
                                Command* cmd = new Command(str1);
                                clearVector();
                                Or* s_or = new Or(tempstack.top(), cmd);
                                tempstack.pop();
                                tempstack.push(s_or);
                                boolstack.pop();
                                boolstack.push(true);
                            }
                        }
                    }    
                }
                else if (cmdstring == "|") {
                    if (parenthesiscounter == 0) {            
                        if (istempassigned) {
                            if (!str1.empty()) {
                                Command* cmd = new Command(str1);
                                clearVector();

                                Base* tempor;

                                if (isand) {
                                    tempor = new And(tempconnector, cmd);    
                                }
                                else {
                                    tempor = new Or(tempconnector, cmd);
                                }

                                tempconnector = tempor;        
                            }
                            isand = false;
                        }
                        else {
                            tempconnector = new Command(str1);
                            clearVector();
                            istempassigned = true;
                            isand = false;
                        }
                    }
                    else {
                        // if (isparentassigned) {
                        //     Base* cmd;

                        //     if (!str1.empty()) {
                        //         cmd = new Command(str1);
                        //         clearVector();
                        //     }
                            
                        //     Base* tempor;
                            
                        //     if (isparentand) {
                        //         tempor = new And(tempparent, cmd);
                        //     }
                        //     else {
                        //         tempor = new Or(tempparent, cmd);
                        //     }

                        //     tempparent = tempor;
                        //     isparentand = false;
                        // }
                        // else {
                        //     tempparent = new Command(str1);
                        //     clearVector();
                        //     isparentassigned = true;
                        //     isparentand = false;
                        // }
                        if ((tempstack.empty()) || (go_next)) {
                            Command* cmd = new Command(str1);
                            clearVector();
                            tempstack.push(cmd);
                            boolstack.push(false);
                            if (go_next) {
                                go_next = false;
                            }
                        }
                        else {
                            if (boolstack.top() == true) {
                                Command* cmd = new Command(str1);
                                clearVector();
                                And* s_and = new And(tempstack.top(), cmd);
                                tempstack.pop();
                                tempstack.push(s_and);
                                boolstack.pop();
                                boolstack.push(false);
                            }
                            else {
                                Command* cmd = new Command(str1);
                                clearVector();
                                Or* s_or = new Or(tempstack.top(), cmd);
                                tempstack.pop();
                                tempstack.push(s_or);
                                boolstack.pop();
                                boolstack.push(false);
                            }
                        }
                    }
                }
                else if (cmdstring == ";") {
                    if (parenthesiscounter == 0) {
                        if (istempassigned){
                            if (!str1.empty()) {
                                if (isand == true){
                                    Command* cmd = new Command(str1);
                                    clearVector();
                                    And* q_and = new And(tempconnector, cmd);
                                    command_queue.push(q_and);
                                    istempassigned = false;
                                }
                                else {
                                    Command* cmd = new Command(str1);
                                    clearVector();
                                    Or* q_or = new Or(tempconnector, cmd);
                                    command_queue.push(q_or);
                                    istempassigned = false;
                                }    
                            }
                            else {
                                command_queue.push(tempconnector);
                                istempassigned = false;
                            }
                        }
                        else {
                            Command* cmd = new Command(str1);
                            clearVector();
                            command_queue.push(cmd);
                        } 
                    }
                    else {
                        // insert new semicolon class
                        // echo 1 && (echo 2 ; echo 3) && echo 4
                    }    
                }
                else if (cmdstring == "(") {
                    parenthesiscounter++;
                    go_next = true;
                }
                else if (cmdstring == ")") {
                    parenthesiscounter--;
                    
                    if (parenthesiscounter == 0) {
                        // Base* cmd;

                        // if (isparentassigned) {
                        //     if (isparentand == true) {
                        //         Command* cmd2 = new Command(str1);
                        //         clearVector(); 
                        //         cmd = new And(tempparent, cmd2);
                        //         isparentassigned = false;
                        //     }
                        //     else {
                        //         Command* cmd2 = new Command(str1);
                        //         clearVector();
                        //         cmd = new Or(tempparent, cmd2);
                        //         isparentassigned = false;
                        //     }
                        // }
                        // else {
                        //     if (str1.empty()) {
                        //         str1.push_back("()");
                        //     }
                        //     cmd = new Command(str1);
                        //     clearVector();    
                        // }
                        // // fixme: fix the else case (echo ())

                        // if (istempassigned) {
                        //     if (isand == true) {
                        //         And* q_and = new And(tempconnector, cmd);
                        //         tempconnector = q_and;
                        //     }
                        //     else {
                        //         Or* q_or = new Or(tempconnector, cmd);
                        //         tempconnector = q_or;
                        //     }
                        // }
                        // else {
                        //     tempconnector = cmd;
                        //     istempassigned = true;
                        // }
                        if (!str1.empty()) {
                            Command* cmd = new Command(str1);
                            clearVector();

                            if (!tempstack.empty()) {
                                if (boolstack.top()) {
                                    And* s_and = new And(tempstack.top(), cmd);
                                    tempstack.pop();
                                    tempstack.push(s_and);
                                    boolstack.pop();
                                }
                                else {
                                    Or* s_or = new Or(tempstack.top(), cmd);
                                    tempstack.pop();
                                    tempstack.push(s_or);
                                    boolstack.pop();
                                }
                            }
                            else {
                                tempstack.push(cmd);
                            }

                            if (istempassigned) {
                                if (isand) {
                                    And* s_and = new And(tempconnector, tempstack.top());
                                    tempconnector = s_and;
                                    tempstack.pop();
                                }
                                else {
                                    Or* s_or = new Or(tempconnector, tempstack.top());
                                    tempconnector = s_or;
                                    tempstack.pop();
                                }
                            }
                            else {
                                tempconnector = tempstack.top();
                                tempstack.pop();
                                istempassigned = true;
                            }
                        }
                        else {
                            if (istempassigned) {
                                if (isand) {
                                    And* s_and = new And(tempconnector, tempstack.top());
                                    tempconnector = s_and;
                                    tempstack.pop();
                                }
                                else {
                                    Or* s_or = new Or(tempconnector, tempstack.top());
                                    tempconnector = s_or;
                                    tempstack.pop();
                                }
                            }
                            else {
                                tempconnector = tempstack.top();
                                tempstack.pop();
                                istempassigned = true;
                            }
                        }
                    }
                    else {
                        Command* cmd = new Command(str1);
                        clearVector();
                        

                        if (boolstack.top()) {
                            And* s_and = new And(tempstack.top(), cmd);
                            tempstack.pop();
                            tempstack.push(s_and);
                            boolstack.pop();
                        }
                        else {
                            Or* s_or = new Or(tempstack.top(), cmd);
                            tempstack.pop();
                            tempstack.push(s_or);
                            boolstack.pop();
                        }

                        //cout << "prepop\n";

                        Base* tempbase = tempstack.top();
                        tempstack.pop();

                        //cout << "survived first pop\n";

                        if (boolstack.top()) {
                            And* s_and = new And(tempstack.top(), tempbase);
                            tempstack.pop();
                            tempstack.push(s_and);
                            boolstack.pop();
                        }
                        else {
                            Or* s_or = new Or(tempstack.top(), tempbase);
                            tempstack.pop();
                            tempstack.push(s_or);
                            boolstack.pop();
                        }
                    }
                    
                    go_next = false;
                }
                else //This means there was no connector found in the current string, therefore push back; we will do this loop until we find a connector
                    str1.push_back(cmdstring);
            }

            if (!str1.empty()) { 
                Command* cmd = new Command(str1);
                clearVector();
            
                if (istempassigned) {
                    if (isand == true){
                        And* q_and = new And(tempconnector, cmd);
                        command_queue.push(q_and);
                    }
                    else {
                        Or* q_or = new Or(tempconnector, cmd);
                        command_queue.push(q_or);
                    }
                    istempassigned = false;
                }
                else 
                    command_queue.push(cmd);
            }
            else if (istempassigned) {
                command_queue.push(tempconnector);
            }
            */

            stack <Base*> cmd;
            stack <int> cmdtracker;
            // 1 -> and
            // 2 -> or
            // 3 -> semicolon
            // 4 -> parenthesis
            // 5 -> special case: echo hi()
            // 6 -> pipe

            for (int i = 0; i < parserObj.parsedCommand.size(); ++i) {
                if (parserObj.parsedCommand.at(i) == "&") {
                    if (!str1.empty()) {
                        Command* tempcmd = new Command(str1);
                        clearVector();

                        if ((!cmdtracker.empty()) && (cmdtracker.top() != 4)) {
                            if (cmdtracker.top() == 1) {
                                And* tempand = new And(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempand);
                                cmdtracker.pop();
                                cmdtracker.push(1);
                            }
                            else if (cmdtracker.top() == 2) {
                                Or* tempor = new Or(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempor);
                                cmdtracker.pop();
                                cmdtracker.push(1);
                            }
                            else if (cmdtracker.top() == 3) {
                                Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempsemi);
                                cmdtracker.pop();
                                cmdtracker.push(1);
                            }
                            else if (cmdtracker.top() == 6) {
                                Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempipe);
                                cmdtracker.pop();
                                cmdtracker.push(1);
                            }
                            else {
                                cout << "This shouldnt happen -> 1A\n";
                            }
                        }
                        else {
                            cmd.push(tempcmd);
                            cmdtracker.push(1);
                        }
                    }
                    else if (!cmd.empty()) {
                        if ((!cmdtracker.empty()) && (cmdtracker.top() != 4)) {
                            Base* tempcmd = cmd.top();
                            cmd.pop();

                            if (cmdtracker.top() == 1) {
                                And* tempand = new And(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempand);
                                cmdtracker.pop();
                                cmdtracker.push(1);
                            }
                            else if (cmdtracker.top() == 2) {
                                Or* tempor = new Or(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempor);
                                cmdtracker.pop();
                                cmdtracker.push(1);
                            }
                            else if (cmdtracker.top() == 3) {
                                Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempsemi);
                                cmdtracker.pop();
                                cmdtracker.push(1);
                            }
                            else if (cmdtracker.top() == 6) {
                                Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempipe);
                                cmdtracker.pop();
                                cmdtracker.push(1);
                            }
                            else {
                                cout << "This shouldnt happen -> 1B\n";
                            }
                        }
                        else {
                            cmdtracker.push(1);
                        }
                    }
                    else {
                        perror("invalid syntax\n");
                        exit(1);
                    }
                }
                else if (parserObj.parsedCommand.at(i) == "|") {
                    if (!str1.empty()) {
                        Command* tempcmd = new Command(str1);
                        clearVector();
                        
                        if ((!cmdtracker.empty()) && (cmdtracker.top() != 4)) {
                            if (cmdtracker.top() == 1) {
                                And* tempand = new And(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempand);
                                cmdtracker.pop();
                                cmdtracker.push(2);
                            }
                            else if (cmdtracker.top() == 2) {
                                Or* tempor = new Or(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempor);
                                cmdtracker.pop();
                                cmdtracker.push(2);
                            }
                            else if (cmdtracker.top() == 3) {
                                Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempsemi);
                                cmdtracker.pop();
                                cmdtracker.push(2);
                            }
                            else if (cmdtracker.top() == 6) {
                                Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempipe);
                                cmdtracker.pop();
                                cmdtracker.push(2);
                            }
                            else {
                                cout << "This shouldnt happen -> 2A\n";
                            }
                        }
                        else {
                            cmd.push(tempcmd);
                            cmdtracker.push(2);
                        }
                    }
                    else if (!cmd.empty()) {
                        if ((!cmdtracker.empty()) && (cmdtracker.top() != 4)) {
                            Base* tempcmd = cmd.top();
                            cmd.pop();

                            if (cmdtracker.top() == 1) {
                                And* tempand = new And(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempand);
                                cmdtracker.pop();
                                cmdtracker.push(2);
                            }
                            else if (cmdtracker.top() == 2) {
                                Or* tempor = new Or(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempor);
                                cmdtracker.pop();
                                cmdtracker.push(2);
                            }
                            else if (cmdtracker.top() == 3) {
                                Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempsemi);
                                cmdtracker.pop();
                                cmdtracker.push(2);
                            }
                            else if (cmdtracker.top() == 6) {
                                Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempipe);
                                cmdtracker.pop();
                                cmdtracker.push(2);
                            }
                            else {
                                cout << "This shouldnt happen -> 2B\n";
                            }
                        }
                        else {
                            cmdtracker.push(2);
                        }
                    }
                    else {
                        perror("invalid syntax\n");
                        exit(1);
                    }
                }
                else if (parserObj.parsedCommand.at(i) == ";") {
                    if (!str1.empty()) {
                        Command* tempcmd = new Command(str1);
                        clearVector();

                        if ((!cmdtracker.empty()) && (cmdtracker.top() != 4)) {
                            if (cmdtracker.top() == 1) {
                                And* tempand = new And(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempand);
                                cmdtracker.pop();
                                cmdtracker.push(3);
                            }
                            else if (cmdtracker.top() == 2) {
                                Or* tempor = new Or(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempor);
                                cmdtracker.pop();
                                cmdtracker.push(3);
                            }
                            else if (cmdtracker.top() == 3) {
                                Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempsemi);
                                cmdtracker.pop();
                                cmdtracker.push(3);
                            }
                            else if (cmdtracker.top() == 6) {
                                Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempipe);
                                cmdtracker.pop();
                                cmdtracker.push(3);
                            }
                            else {
                                cout << "This shouldnt happen -> 3A\n";
                            }
                        }
                        else {
                            cmd.push(tempcmd);
                            cmdtracker.push(3);
                        }
                    }
                    else if (!cmd.empty()) {
                        if ((!cmdtracker.empty()) && (cmdtracker.top() != 4)) {
                            Base* tempcmd = cmd.top();
                            cmd.pop();

                            if (cmdtracker.top() == 1) {
                                And* tempand = new And(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempand);
                                cmdtracker.pop();
                                cmdtracker.push(3);
                            }
                            else if (cmdtracker.top() == 2) {
                                Or* tempor = new Or(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempor);
                                cmdtracker.pop();
                                cmdtracker.push(3);
                            }
                            else if (cmdtracker.top() == 3) {
                                Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempsemi);
                                cmdtracker.pop();
                                cmdtracker.push(3);
                            }
                            else if (cmdtracker.top() == 6) {
                                Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempipe);
                                cmdtracker.pop();
                                cmdtracker.push(3);
                            }
                            else {
                                cout << "This shouldnt happen -> 3B\n";
                            }
                        }
                        else {
                            cmdtracker.push(3);
                        }
                    }
                    else {
                        // do nothing?
                        // perror("invalid syntax\n");
                        // exit(1);
                    }
                }
                else if (parserObj.parsedCommand.at(i) == "@") {
                    if (!str1.empty()) {
                        Command* tempcmd = new Command(str1);
                        clearVector();

                        if ((!cmdtracker.empty()) && (cmdtracker.top() != 4)) {
                            if (cmdtracker.top() == 1) {
                                And* tempand = new And(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempand);
                                cmdtracker.pop();
                                cmdtracker.push(6);
                            }
                            else if (cmdtracker.top() == 2) {
                                Or* tempor = new Or(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempor);
                                cmdtracker.pop();
                                cmdtracker.push(6);
                            }
                            else if (cmdtracker.top() == 3) {
                                Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempsemi);
                                cmdtracker.pop();
                                cmdtracker.push(6);
                            }
                            else if (cmdtracker.top() == 6) {
                                Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempipe);
                                cmdtracker.pop();
                                cmdtracker.push(6);
                            }
                            else {
                                cout << "This shouldnt happen -> 6A\n";
                            }
                        }
                        else {
                            cmd.push(tempcmd);
                            cmdtracker.push(6);
                        }
                    }
                    else if (!cmd.empty()) {
                        if ((!cmdtracker.empty()) && (cmdtracker.top() != 4)) {
                            Base* tempcmd = cmd.top();
                            cmd.pop();

                            if (cmdtracker.top() == 1) {
                                And* tempand = new And(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempand);
                                cmdtracker.pop();
                                cmdtracker.push(6);
                            }
                            else if (cmdtracker.top() == 2) {
                                Or* tempor = new Or(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempor);
                                cmdtracker.pop();
                                cmdtracker.push(6);
                            }
                            else if (cmdtracker.top() == 3) {
                                Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempsemi);
                                cmdtracker.pop();
                                cmdtracker.push(6);
                            }
                            else if (cmdtracker.top() == 6) {
                                Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempipe);
                                cmdtracker.pop();
                                cmdtracker.push(6);
                            }
                            else {
                                cout << "This shouldnt happen -> 6B\n";
                            }
                        }
                        else {
                            cmdtracker.push(6);
                        }
                    }
                    else {
                        perror("invalid syntax\n");
                        exit(1);
                    }
                }
                else if (parserObj.parsedCommand.at(i) == "(") {
                    if (!str1.empty()) {
                        cmdtracker.push(5);    
                    }
                    else {
                        cmdtracker.push(4);
                    }
                }
                else if (parserObj.parsedCommand.at(i) == ")") {
                    if (cmdtracker.top() == 5) {
                        str1.push_back("()");
                        cmdtracker.pop();
                    }
                    else {
                        if (!str1.empty()) {
                            cmd.push(new Command(str1));
                            clearVector();
                        }

                        if ((!cmdtracker.empty()) && (cmdtracker.top() != 4)) {
                            if (cmd.empty()) {
                                perror("Invalid syntax");
                                exit(1);
                            }

                            Base* tempcmd = cmd.top();
                            cmd.pop();

                            if (cmdtracker.top() == 1) {
                                And* tempand = new And(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempand);
                                cmdtracker.pop();
                            }
                            else if (cmdtracker.top() == 2) {
                                Or* tempor = new Or(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempor);
                                cmdtracker.pop();
                            }
                            else if (cmdtracker.top() == 3) {
                                Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempsemi);
                                cmdtracker.pop();
                            }
                            else if (cmdtracker.top() == 6) {
                                Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                                cmd.pop();
                                cmd.push(tempipe);
                                cmdtracker.pop();
                            }
                            else {
                                cout << "This shouldnt happen -> 4\n";
                            }
                        }
                        cmdtracker.pop();
                    }
                }
                else {
                    str1.push_back(parserObj.parsedCommand.at(i));
                }
            }

            if (!str1.empty()) {
                Command* tempcmd = new Command(str1);
                clearVector();

                if (cmdtracker.empty()) {
                    cmd.push(tempcmd);
                }
                else {
                    while (!cmdtracker.empty()) {
                        if (cmdtracker.top() == 1) {
                            And* tempand = new And(cmd.top(), tempcmd);
                            cmd.pop();
                            cmd.push(tempand);
                            cmdtracker.pop();
                        }
                        else if (cmdtracker.top() == 2) {
                            Or* tempor = new Or(cmd.top(), tempcmd);
                            cmd.pop();
                            cmd.push(tempor);
                            cmdtracker.pop();
                        }
                        else if (cmdtracker.top() == 3) {
                            Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                            cmd.pop();
                            cmd.push(tempsemi);
                            cmdtracker.pop();
                        }
                        else if (cmdtracker.top() == 6) {
                            Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                            cmd.pop();
                            cmd.push(tempipe);
                            cmdtracker.pop();
                        }
                        else {
                            cout << "This shouldnt happen -> 5\n";
                        }
                    }   
                }
            }
            else {
                if (!cmdtracker.empty()) {
                    Base* tempcmd = cmd.top();
                    cmd.pop();

                    while (!cmdtracker.empty()) {
                        if (cmdtracker.top() == 1) {
                            And* tempand = new And(cmd.top(), tempcmd);
                            cmd.pop();
                            cmd.push(tempand);
                            cmdtracker.pop();
                        }
                        else if (cmdtracker.top() == 2) {
                            Or* tempor = new Or(cmd.top(), tempcmd);
                            cmd.pop();
                            cmd.push(tempor);
                            cmdtracker.pop();
                        }
                        else if (cmdtracker.top() == 3) {
                            Semi* tempsemi = new Semi(cmd.top(), tempcmd);
                            cmd.pop();
                            cmd.push(tempsemi);
                            cmdtracker.pop();
                        }
                        else if (cmdtracker.top() == 6) {
                            Pipe* tempipe = new Pipe(cmd.top(), tempcmd);
                            cmd.pop();
                            cmd.push(tempipe);
                            cmdtracker.pop();
                        }
                        else {
                            cout << "This shouldnt happen -> 5\n";
                        }
                    }
                }
            }

            if (!cmd.empty()) {
                cmd.top() -> execute_command(0, 1);
            }
        };

        void clearVector() {
            while (!str1.empty()) {
                str1.pop_back();
            }
        };
};

#endif