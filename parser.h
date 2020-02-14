#ifndef PARSER_H
#define PARSER_H

#include <boost/tokenizer.hpp>
#include <string>
#include <vector>
using namespace std;
using namespace boost;

struct Parser {
    vector<string> parsedCommand;
    
    void parse(string inputStr) {
        typedef tokenizer<char_separator<char> > tokenizer; //Tokenizer char_seperator whcih will define delimiters.
        
        char_separator<char> sep(""," <>()&;|#"); //Defining delimiters. No tokens are dropped. And empty tokens aren't kept.
        
        tokenizer tokens(inputStr, sep); //We input the inputStr and the tokens to be split.          
        
        //Utilizes boost tokenizer to split the string into several tokens based off of the delimiters.
    
        for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
            if ((*tok_iter != " ") && (*tok_iter != "#")) { 
                // parsedCommand.push_back(*tok_iter);
                // if ((*tok_iter == "&") || (*tok_iter == "|")) //Skips extra connector symbol.
                //     ++tok_iter;

                if (*tok_iter == "&") {
                    parsedCommand.push_back(*tok_iter);
                    ++tok_iter;
                }
                else if (*tok_iter == "|") {
                    if (*(++tok_iter) == "|") {
                        parsedCommand.push_back(*tok_iter);
                        ++tok_iter;
                    }
                    else {
                        parsedCommand.push_back("@");
                    }
                }  
                else if (*tok_iter == ">") {
                    if (*(++tok_iter) == ">") {
                        parsedCommand.push_back(">>");
                        ++tok_iter;
                    }
                    else {
                        parsedCommand.push_back(">");
                    }
                }  
                else {
                    parsedCommand.push_back(*tok_iter);
                }
            }
            else if (*tok_iter == "#") //Stops everything past # symbol so that nothing is parsed past it.
                break;    
        }
    }
    
    void clearparser(){
        while(!parsedCommand.empty()) {
            parsedCommand.pop_back();
        }
    }
};

#endif 