#ifndef ___SEMI_H__
#define ___SEMI_H__

#include "connector.h"

using namespace std;

class Semi : public Connector { 
    public:
        Semi() : Connector() {};
        Semi(Base* b, Base* c) : Connector(b,c) {};
    
        bool execute_command(int a, int b) {
            current -> execute_command(0, 1);
            return next -> execute_command(0, 1);
        };
};

#endif //___SEMI_H__