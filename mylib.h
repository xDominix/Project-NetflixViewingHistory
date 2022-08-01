#ifndef MYLIB_H 
#define MYLIB_H

#include <map>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <list>
#include <tuple>

#include <utility>
#include <functional>

#include "csvread.h"
#include "factory.h" 
#include "csvread.h" 
#include "builder.h" 

using namespace std;

enum VideoType{MOVIE,ONESERIES,MULTISERIES,UNKNOWN};
inline const char* to_string(VideoType v)
{
    switch (v)
    {
        case MOVIE:               return "MOVIE";
        case ONESERIES:       return "ONE-SEASON SERIES";
        case MULTISERIES:   return "MULTI-SEASON SERIES";
        default:                       return "UNKNOWN";
    }
};

struct aSettings
{
    string title;
    string fileName;
    int screenSize;
    int movieLen;
    int seriesLen;
};

struct bSettings //look at builder.h
{
    int watchBegin;
    int watchEnd;
    int most;
    int dislike;
    int onRepeat;
    int shortest;
    int fastest;
    int longest;
    int binge;
};

struct dDate
{
    string startDate;
    string endDate;
    int cnt;
    dDate(string date):startDate(date),endDate(date),cnt(1){}
};

class Video 
{
    public:
        Video(string ntitle,string strDate):title(ntitle),bestSession(dDate("")),firstDate(strDate){}

        void addRecord(Record record);

        void setTypeAuto();
        void setBestSessionAuto();

        void setType(enum VideoType manualType)  { videoType = manualType; }
       
        VideoType getType() const { return videoType; }
        string getTitle() const {return title; }
        int getEpisodesLength() const {return episodes.size(); }
        vector<string> getEpisodes() const{return episodes;}
        vector<int> getBreaks() const{return breaks; }
        dDate getBestSession() const {return bestSession;}
        string getFirstDate()  const{return firstDate;}
        string getLastDate() const {return lastDate;}
    
    private:
        string title;
        vector<string> episodes;
        vector<int> breaks; // 1 1 1 21 2 2 219 19
        string firstDate, lastDate;

        //after configurating
        VideoType videoType;
        dDate bestSession; //longest
};

void loading(int seconds);//visuals

void start(aSettings a,bSettings b);

#endif