#ifndef ___AND_H__
#define ___AND_H__

#include "connector.h"

using namespace std;

class And : public Connector {
    public:
        And() : Connector() {};
        And(Base* b, Base* c) : Connector(b,c) {};

        bool execute_command(int a, int b) {
            if (current -> execute_command(0, 1))
                return next -> execute_command(0, 1);
            
            return false;    
        };
};

#endif //___AND_H__