#ifndef BUILDER_H 
#define BUILDER_H

#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <map>

#include "factory.h" 

using namespace std;

string toupper(string text);

class Builder
{
    public:
        Builder(Factory fac):factory(fac){}

        void title(string title,string date1,string date2);
        void titleDown(int period,int watch);
        void total(int total,int movie, int one,int multi);
        void totalEpisodes(int epi,double day);
        void totalHours(int r1,int r2,int hours,double day);
        void years(vector<pair<string,int>> months);
        void months(vector<pair<string,int>> months);
        void watch(vector<tuple<string,string>> titles1,vector<tuple<string,string>> titles2);
        void most(vector<pair<string ,int>> titles);
        void dislike(int totalSeries,int totalDislikes,vector<string> titles);
        void fastest(vector<tuple<string,string,int,int>> sessions);
        void longest(vector<tuple<string,string,int,int>> sessions);
        void strike(int days,string date1,string date2,vector<string> titles,int total);
        void binge(vector<tuple<string,vector<string>>> dates);
        void gap(string date1,string date2,int days, string name1,string name2);

        void aboutSeries(string title,string type, int epi,string date1,string date2,
vector<string> epis,vector<int> breaks, tuple<string,int,int> bestSession);
        void aboutMovie(string title,string type,string date);

    private:
        Factory factory;
};

#endif