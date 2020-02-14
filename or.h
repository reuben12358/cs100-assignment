#ifndef ___OR_H__
#define ___OR_H__

#include "connector.h"

using namespace std;

class Or : public Connector { 
    public:
        Or() : Connector() {};
        Or(Base* b, Base* c) : Connector(b,c) {};
    
        bool execute_command(int a, int b) {
            if (current -> execute_command(0, 1)) 
                return true;
            
            return next -> execute_command(0, 1);
        };
};

#endif //___OR_H__