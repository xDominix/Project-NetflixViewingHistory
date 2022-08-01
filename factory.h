#ifndef FACTORY_H 
#define FACTORY_H

#include <iostream>
#include <unistd.h>
#include <string> 
#include <vector>
#include <algorithm>

#define MINSCREENSIZE 51
#define TEXTGAP 2 //pomiedzy textami osobnymi

using namespace std;

class Factory
{
    public:
        Factory(int newScreenSize);
        void head(string text=""); //uppercase , center
        void body(string left,string right="",string center=""); //cutter
        void lbody(vector<string> list,string title=""); //list left body , pagination
        void margin();
    private:
        int screenSize;
};

#endif