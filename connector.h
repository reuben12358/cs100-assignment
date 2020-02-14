#ifndef ___CONNECTOR_H__
#define ___CONNECTOR_H__

#include "base.h"

using namespace std;

class Connector : public Base {
    protected: 
        Base* current; //Current command to be executed.
        Base* next; //The next command to be executed. 

    public: 
        /* Constructors */
        Connector() : current(0), next(0) {};
        Connector(Base* b, Base* c) : current(b), next(c) {};

        /* pure virtual function */
        virtual bool execute_command(int a, int b) = 0;
};

#endif //___CONNECTOR_H__