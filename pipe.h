#ifndef ___PIPE_H__
#define ___PIPE_H__

#include "connector.h"

using namespace std;

class Pipe : public Connector { 
    public:
        Pipe() : Connector() {};
        Pipe(Base* b, Base* c) : Connector(b,c) {};
    
        bool execute_command(int a, int b) {
            int pipearray[2];
            
            pipe(pipearray);

            current -> execute_command(a, pipearray[1]);
            close(pipearray[1]);
            
            next -> execute_command(pipearray[0], b);
            close(pipearray[0]);

            return true;
        };
};

#endif //___SEMI_H__