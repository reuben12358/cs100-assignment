#ifndef __BASE_H__
#define __BASE_H__

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

using namespace std;

class Base {
    public:
        /* Constructors */
        Base() {};

        /* Pure virtual functions */
        virtual bool execute_command(int a, int b) = 0;
};

#endif //__BASE_H__
