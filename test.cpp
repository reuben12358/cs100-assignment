#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "../src/base.h"
#include "../src/rshell.h"
#include "../src/parser.h" //Parser with modified output loop for testing.
#include "../src/connector.h"
#include "../src/command.h"
#include "../src/and.h"
#include "../src/or.h"
#include <boost/tokenizer.hpp>
#include <unistd.h>

using namespace std;
using namespace boost;

#include "gtest/gtest.h"

TEST(ParserTests, SingleCommand1) {
    Parser p;
    string userInput = "ls";
    
    p.parse(userInput);
    
    string output = p.parsedCommand.at(0);
    
    EXPECT_EQ("ls", output);
   
}

TEST(ParserTests, SingleCommand2) {
   
    Parser p;
    string userInput = " ";

    p.parse(userInput);

    bool output = p.parsedCommand.empty();

    EXPECT_EQ(true, output);
   
}

TEST(ParserTests, SingleCommand3) {
   
    Parser p;
    string userInput = ";";

    p.parse(userInput);

    string output = p.parsedCommand.at(0);

    EXPECT_EQ(";", output);
   
}

TEST(ParserTests, SingleCommand4) {

    Parser p;
    string userInput = "||";

    p.parse(userInput);

    string output = p.parsedCommand.at(0);

    EXPECT_EQ("|", output); //Only receive a single '|' so that it can be passed into rShell.

}

TEST(ParserTests, SingleCommand5) {

    Parser p;
    string userInput = "echo \"Hi\"";

    p.parse(userInput);

    string output = p.parsedCommand.at(0);
    output += " ";
    output += p.parsedCommand.at(1);

    EXPECT_EQ("echo \"Hi\"", output);

}

TEST(ParserTests, SingleCommand6) {

    Parser p;
    string userInput = "";

    p.parse(userInput);

    bool output = p.parsedCommand.empty();

    EXPECT_EQ(true, output);

}

TEST(ParserTests, SingleCommand7) {

    Parser p;
    string userInput = "# this shouldn't be passed in";

    p.parse(userInput);

    bool output = p.parsedCommand.empty();

    EXPECT_EQ(true, output);
}


TEST(ParserTests, MultiCommand1) {
    
    Parser p;
    string userInput = "ls ; mkdir folder ; cd .. ;";

    p.parse(userInput);

    string output;

    for (int i = 0; i < p.parsedCommand.size(); ++i) {
         output += p.parsedCommand.at(i);
         output += " ";

    }

    EXPECT_EQ("ls ; mkdir folder ; cd .. ; ", output);
    
}

TEST(ParserTests, MultiCommand2) {

    Parser p;
    string userInput = "                      ls ; touch ab ; echo \"Hello World\" ;";
    p.parse(userInput);

    string output;

    for (int i = 0; i < p.parsedCommand.size(); ++i) {
       output += p.parsedCommand.at(i);
       output += " ";
    }

    EXPECT_EQ("ls ; touch ab ; echo \"Hello World\" ; ", output);

}

TEST(ParserTests, MultiCommand3) {

    Parser p;
    string userInput = "|| && ;";

    p.parse(userInput);

    string output;

    for (int i = 0; i < p.parsedCommand.size(); ++i) {
       output += p.parsedCommand.at(i);
       output += " ";
    }

    EXPECT_EQ("| & ; ", output);

}

TEST(ParserTests, MultiCommand4) {

    Parser p;
    string userInput = "&&&&&&&&&&&&&&";

    p.parse(userInput);

    string output;

    for (int i = 0; i < p.parsedCommand.size(); ++i) {
       output += p.parsedCommand.at(i);
       output += " ";
    }

    EXPECT_EQ("& & & & & & & ", output);
}

TEST(ParserTests, MultiCommand5) {

    Parser p;
    string userInput = "; ; || && echo \"Hello\" ; || ";

    p.parse(userInput);

    string output;

    for (int i = 0; i < p.parsedCommand.size(); ++i) {
       output += p.parsedCommand.at(i);
       output += " ";
    }

    EXPECT_EQ("; ; | & echo \"Hello\" ; | ", output);

}

TEST(ParserTests, Parenthesis1) {
    Parser p;
    string in = "(ls && (echo A && echo B))";
    p.parse(in);

    string output;

    for (int i = 0; i < p.parsedCommand.size(); ++i) {
       output += p.parsedCommand.at(i);
       output += " ";
    }

    EXPECT_EQ("( ls & ( echo A & echo B ) ) ", output);

}

TEST(ParserTests, Parenthesis2) {
    Parser p;
    string in = "(ls)";
    p.parse(in);

    string output;

    for (int i = 0; i < p.parsedCommand.size(); ++i) {
        output += p.parsedCommand.at(i);
        output += " ";
    }

    EXPECT_EQ("( ls ) ", output);
}

TEST(ParserTests, Parenthesis3) {
    Parser p;
    string in = "(((((((";
    p.parse(in);

    string output;

    for (int i = 0; i < p.parsedCommand.size(); ++i) {
        output += p.parsedCommand.at(i);
        output += " ";
    }

    EXPECT_EQ("( ( ( ( ( ( ( ", output);

}



TEST(SemiTests, Semi1) {

    vector<string> cmds{"ls"};

    Command* ca = new Command(cmds);

    bool output = ca->execute_command();

    EXPECT_EQ(true, output);


}

TEST(SemiTests, Semi2) {

    vector<string> cmds{"echo", "Hi"};

    Command* ca = new Command(cmds);

    bool output = ca->execute_command();

    EXPECT_EQ(true, output);

}


TEST(SemiTests, Semi3) {

    vector<string> cmd1{"ls"};
    vector<string> cmd2{"echo", "Hi"};
    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);

    bool output1 = ca->execute_command();

    bool output2 = cb->execute_command();

    EXPECT_EQ(true, output1);
    EXPECT_EQ(true, output2);

}

TEST (AndTests, And1) {
    vector<string> cmd1{"ls"};
    vector<string> cmd2{"echo", "Hi"};
    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
   
    And* a = new And(ca, cb);

    bool output = a->execute_command();

    EXPECT_EQ(true, output);

}

TEST(AndTests, And2) {

    vector<string> cmd1{"potato"};
    vector<string> cmd2{"echo", "Hi"};

    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
    And* a = new And(ca, cb);
    bool output = a->execute_command();

    EXPECT_EQ(false, output);

   
}

TEST(AndTests, And3) {

    vector<string> cmd1 {"ls"};
    vector<string> cmd2 {"potato Yes"};
    vector<string> cmd3 {"echo No"};

    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
    Command* cc = new Command(cmd3);
    And* a = new And(ca, cb);
    And* b = new And(a, cc);
   
    bool output1 = b->execute_command();
    bool output2 = a->execute_command();

    EXPECT_EQ(false, output1);
    EXPECT_EQ(false, output2);
}

TEST(OrTests, Or1) {

    vector<string> cmd1 {"ls"};
    vector<string> cmd2 {"echo", "Hi"};

    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
    Or* o = new Or(ca, cb);

    bool output = o->execute_command();

    EXPECT_EQ(true, output);

}

TEST(OrTests, Or2) {

    vector<string> cmd1 {"lss"};
    vector<string> cmd2 {"echo", "Hi"};

    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
    Or* o = new Or(ca, cb);

    bool output = o->execute_command();

    EXPECT_EQ(true, output);

}

TEST(OrTests, Or3) {

    vector<string> cmd1 {"lss"};
    vector<string> cmd2 {"echohoho Hey"};
    vector<string> cmd3 {"ls"};

    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
    Command* cc = new Command(cmd3);
    Or* or1 = new Or(ca, cb);
    Or* or2 = new Or(or1, cc);

    bool output = or2->execute_command();
    bool output2 = or1->execute_command();

    EXPECT_EQ(true, output);
    EXPECT_EQ(false, output2);
}

TEST(MultiTests, Mult1) {
    //And-Or
    vector<string> cmd1 {"ls"};
    vector<string> cmd2 {"echo Hey"};
    vector<string> cmd3 {"corn"};

    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
    Command* cc = new Command(cmd3);
    
    And* a = new And(ca, cc);
    Or*  b = new Or(a, cb);

    bool output = a->execute_command();
    bool output2 = b->execute_command();
    
    EXPECT_EQ(false, output);
    EXPECT_EQ(false, output2);
}

TEST(MultiTests, Mult2) {
    //Or-And
    vector<string> cmd1 {"ls"};
    vector<string> cmd2 {"echo Hello"};
    vector<string> cmd3 {"YEA"};

    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
    Command* cc = new Command(cmd3);
    
    Or* a = new Or(cc, cb);
    And* b = new And(a, ca);
    
    bool output = a->execute_command();
    bool output2 = b->execute_command();
    
    EXPECT_EQ(false, output);
    EXPECT_EQ(false, output2);
}

TEST(MultiTests, Mult3) {
    //And-And Pass
    vector<string> cmd1 {"ls"};
    vector<string> cmd2 {"echo Hey"};
    vector<string> cmd3 {"ls -a"};

    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
    Command* cc = new Command(cmd3);

    And* a = new And(ca, cb);
    And* b = new And(a, cc);
    
    bool output = b->execute_command();
    bool output2 = a->execute_command();
    
    EXPECT_EQ(false, output2);
    EXPECT_EQ(false, output);
}

TEST(MultiTests, Mult4) {
    //Or-Or
    vector<string> cmd1 {"ls"};
    vector<string> cmd2 {"what"};
    vector<string> cmd3 {"echo Hi"};
    
    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
    Command* cc = new Command(cmd3);

    Or* a = new Or(ca, cb);
    Or* b = new Or(a, cc);
    
    bool output = a->execute_command();
    bool output2 = b->execute_command();

    EXPECT_EQ(true, output);
    EXPECT_EQ(true, output2);
}

TEST(MultiTests, Mult5) {
     //Or-And
    vector<string> cmd1 {"ls"};
    vector<string> cmd2 {"what"};
    vector<string> cmd3 {"echo Hi"};

    Command* ca = new Command(cmd1);
    Command* cb = new Command(cmd2);
    Command* cc = new Command(cmd3);

    Or* a = new Or(ca, cb);
    And* b = new And(a, cc);

    bool output = a->execute_command();
    bool output2 = b->execute_command();

    EXPECT_EQ(true, output);
    EXPECT_EQ(false, output2);
}

TEST(TestCmd, T1) {
    vector<string> cmd1 {"test", "-e", "tests/test.cpp"};
    Command* ca = new Command(cmd1);
    bool output = ca->execute_command();

    EXPECT_EQ(true, output);
}

TEST(TestCmd, T2) { 
    vector<string> cmd1 {"test", "-f", "tests/test.cpp"};
    Command* ca = new Command(cmd1);
    bool output = ca->execute_command();

    EXPECT_EQ(true, output);
}

TEST(TestCmd, T3) {
    vector<string> cmd1 {"test", "-d", "tests"};
    Command* ca = new Command(cmd1);
    bool output = ca->execute_command();

    EXPECT_EQ(true, output);
}

TEST(TestCmd, T4) {
    vector<string> cmd1 {"test", "-e", "non-existent.wav"};
    Command* ca = new Command(cmd1);
    bool output = ca->execute_command();

    EXPECT_EQ(false, output);
}

TEST(TestCmd, T5) {
    vector<string> cmd1 {"test", "tests/test.cpp"};
    Command* ca = new Command(cmd1);
    bool output = ca->execute_command();

    EXPECT_EQ(true, output);
}

TEST(TestCmd, T6) {
    vector<string> cmd1 {"test", "-f", "tests"};
    Command* ca = new Command(cmd1);
    bool output = ca->execute_command();

    EXPECT_EQ(false, output);
}

TEST(TestCmd, T7) {
    vector<string> cmd1 {"test", "-d", "tests/test.cpp"};
    Command* ca = new Command(cmd1);
    bool output = ca->execute_command();
   
    EXPECT_EQ(false, output);
}

TEST(TestCmd, Tsymbol) {
    vector<string> cmd1 {"[", "test" "-e", "tests/test.cpp", "]"};
    Command* ca = new Command(cmd1);
    bool output = ca->execute_command();

    EXPECT_EQ(true, output);
}  


int main(int argc, char **argv){
        ::testing::InitGoogleTest(&argc,argv);
        return RUN_ALL_TESTS();
}

