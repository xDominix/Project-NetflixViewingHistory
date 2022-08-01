#include <iostream>

#include "mylib.h" 

//settings 1 - main
#define TITLE "NETFLIX WATCH HISTORY SUMMARY"
#define SCREENSIZE 50
#define MOVIELEN 100 //in minutes
#define SERIESLEN 40 //in minutes

//settings 2
#define WATCHBEGIN 5 //watch history begin
#define WATCHEND 5 //watch history end
#define MOST 15 //most watched
#define DISLIKE 3
#define FASTEST 5
#define LONGEST 5
#define BINGE 2

//ogolne zalozenie: nie ogladales seriali od srodka... xd

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2) {cout<<"Wrong no arguments..."<<argc<<endl; exit(1);}

    aSettings a;
    a.fileName =  argv[1];
    a.title = TITLE;
    a.screenSize = SCREENSIZE;
    a.movieLen = MOVIELEN;
    a.seriesLen = SERIESLEN;
    
    bSettings b;
    b.watchBegin = WATCHBEGIN;
    b.watchEnd = WATCHEND;
    b.most = MOST;
    b.dislike = DISLIKE;
    b.fastest = FASTEST;
    b.longest = LONGEST;
    b.binge = BINGE;

    start(a,b);

    return 0;
}