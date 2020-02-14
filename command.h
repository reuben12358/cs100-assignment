#ifndef ___COMMAND_H__
#define ___COMMAND_H__

#include "base.h"
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <fcntl.h> // for the O_ commands

using namespace std;

class Command : public Base {
    private:
        vector <string> cmdvector;
        
    public:
        Command(vector <string> str_command) {
            for (int i = 0; i < str_command.size(); ++i) { //This for loop erases the extra connector symbol at the end of a command.
                if ((str_command.at(i).at(str_command.at(i).size() - 1) == '&') || (str_command.at(i).at(str_command.at(i).size() - 1) == '|') || (str_command.at(i).at(str_command.at(i).size() - 1) == ';') || (str_command.at(i).at(str_command.at(i).size() - 1) == '#')) { 
                    str_command.at(i).erase(str_command.at(i).size() - 1);
                }
                cmdvector.push_back(str_command.at(i));
            }
        };
        
        //Execute command calls the proper syscalls for execution of a command. 
        //It pushes a vector of strings into a char pointer array.
        bool execute_command(int indir, int outdir) {
            if (cmdvector.at(0) == "exit") {
                exit(0);
            }
            else if (cmdvector.at(0) == "test" || (cmdvector.at(0) == "[") ){
                //cmdvector.at(0) will either turn out to be "test" or "["
                //cmdvecotr.at(cmdvector.size() - 1) might be questionable however...

                if (cmdvector.at(0) == "[" && cmdvector.size() < 4) {
                    perror("syntax error: incomplete input of test command");
                    return false;
                }
                
                if (cmdvector.size() < 2) {
                    perror("syntax error: incomplete input of test command\n");
                    return false; //BUG: it seems that the command still executes? Always prints an extra "No such file or directory".
                }
                
                string flag;
                string path;
                
                if (cmdvector.size() == 3 || cmdvector.size() == 4) { //This mean it is "test" or a complete symbolic test
                    flag = cmdvector.at(1); //It is assumed this is correct thanks to parser. 
                    path = cmdvector.at(2);
                }
                else if (cmdvector.size() == 2) {
                    flag = "-e";
                    path = cmdvector.at(1);
                }
                
                struct stat buf;
                
                if (flag == "-e") { //Check for existence of a file.
                    //printf("do -e\n");
                    
                    if (stat(path.c_str(), &buf) == 0) {
                        printf("(True)\n");
                        return true;
                    }
                }
                else if (flag == "-f") { //Check for existence and if it is a normal file.
                    //printf("do -f\n");
                    
                    if ( (stat(path.c_str(), &buf) == 0) && (S_ISREG(buf.st_mode) == 1) ) {
                        printf("(True)\n");
                        return true;
                    }
                }
                else if (flag == "-d") { //Check for existence and if it is a directory.
                    //printf("do -d\n");
                    if ( (stat(path.c_str(), &buf) == 0) && (S_ISDIR(buf.st_mode) == 1) ) {
                        printf("(True)\n");
                        return true;
                    }
                }
                else { //Defaulting on -e
                    if (stat(path.c_str(), &buf) == 0) {
                        printf("(True)\n");
                        return true;
                    }
                }
                
                printf("(False)\n");
                return false;
                
            }
            else {
                // char *cmd[cmdvector.size() + 1];//&cmdvector[0];
            
                // for(int i = 0; i < cmdvector.size(); i++) { //Converting strings into char*.
                //     cmd[i] = (char*) cmdvector[i].c_str();
                // }
            
                // cmd[cmdvector.size()] = '\0'; //Adds null char.

    		    pid_t pid = fork();
    		    pid_t wait;
    		    int abc;
    
        		if (pid > 0) {
    	    		wait = waitpid(pid, &abc, 0);
    		    	if (wait == -1) {
    			    	perror("wait error");
    				    exit(EXIT_FAILURE);
    			    }
    
        			if (WEXITSTATUS(abc) == 0) 
    	    			return true;
    		    }
        		else if (pid == 0) {
                    bool specialcase = false;
                    for (int i = 0; i < cmdvector.size(); ++i) {
                        if ((cmdvector.at(i) == ">") || (cmdvector.at(i) == "<") || (cmdvector.at(i) == ">>")) {
                            specialcase = true;
                        }
                    }

                    // could also use vector<string>::iterator it = find(cmdvector.begin(), cmdvector.end(), ">/</>>")
                    // 3 separate iterators?

                    if (!specialcase) {
                        char* cmd[cmdvector.size() + 1];
                        
                        for (int i = 0; i < cmdvector.size(); ++i) {
                            cmd[i] = (char*) cmdvector[i].c_str();
                        }

                        cmd[cmdvector.size()] = '\0';

                        dup2(indir, 0);
                        dup2(outdir, 1);

                        if (indir != 0) {
                            close(indir);
                        }
                        else if (outdir != 1) {
                            close(outdir);
                        }
                        if (execvp(*cmd, cmd) < 0) {
                            perror("failed to execute command");
                            exit(1);
                        }
                    }
                    else {
                        vector<string> tempvec;
                        string input = "";
                        string output1 = "";
                        string output2 = "";
                        for (int i = 0; i < cmdvector.size(); ++i) {
                            if ((cmdvector.at(i) != ">") && (cmdvector.at(i) != "<") && (cmdvector.at(i) != ">>")) {
                                tempvec.push_back(cmdvector.at(i));
                            }
                            else {
                                if (cmdvector.at(i) == "<") {
                                    input = cmdvector.at(i + 1);
                                }
                                else if (cmdvector.at(i) == ">") {
                                    output1 = cmdvector.at(i + 1);
                                }
                                else if (cmdvector.at(i) == ">>") {
                                    output2 = cmdvector.at(i + 1);
                                }
                                break;
                            }
                        }
                        char* cmd[tempvec.size() + 1];

                        for (int i = 0; i < tempvec.size(); ++i) {
                            cmd[i] = (char*) tempvec[i].c_str();
                        }
                        
                        cmd[tempvec.size()] = '\0';

                        if (input != "") {
                            if (output2 != "") {
                                int file1 = open(input.c_str(), O_RDWR);
                                int file2 = open(output2.c_str(), O_CREAT | O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG);
                                if (file1 == -1) {
                                    perror("file opening error");
                                    return false;
                                }
                                if (file2 == -1) {
                                    perror("file opening error");
                                    return false;
                                }
                                dup2(file1, 0);
                                dup2(file2, 1);
                                close(file1);                        
                                close(file2);
                            }
                            else if (output1 != "") {
                                int file1 = open(input.c_str(), O_RDWR);
                                int file2 = open(output1.c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRWXG);
                                if (file1 == -1) {
                                    perror("file opening error");
                                    return false;
                                }
                                if (file2 == -1) {
                                    perror("file opening error");
                                    return false;
                                }
                                dup2(file1, 0);
                                dup2(file2, 1);
                                close(file1);                        
                                close(file2);
                            }
                            else {
                                int file1 = open(input.c_str(), O_RDWR);
                                if (file1 == -1) {
                                    perror("file opening error");
                                    return false;
                                }
                                dup2(file1, 0);
                                close(file1);
                            }
                        }
                        else if (output2 != "") {
                            int file2;
                            file2 = open(output2.c_str(), O_CREAT | O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG);
                            
                            if (file2 < 0) {
                                perror("file opening error");
                                return false;
                            }
                            dup2(file2, 1);
                            close(file2);
                        }
                        else if (output1 != "") {
                            int file2;
                            file2 = open(output1.c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRWXG);
                            
                            if (file2 < 0) {
                                perror("file opening error");
                                return false;
                            }
                            dup2(file2, 1);
                            close(file2);
                        }
                        else {
                            perror("this shouldnt happen\n");
                            exit(1);
                        }
                        dup2(indir, 0);
                        dup2(outdir, 1);

                        if (indir != 0) {
                            close(indir);
                        }
                        else if (outdir != 1) {
                            close(outdir);
                        }
                        if (execvp(*cmd, cmd) < 0) {
                            perror("failed to execute command");
                            exit(1);
                        }
                    }
    		    }
                else {
    	    		perror("Should not happen but eh?");
    		    	exit(1);
    	    	}
            }
            
            return false;
        };
};
#endif //___COMMAND_H__