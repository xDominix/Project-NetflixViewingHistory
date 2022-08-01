#ifndef CSVREAD_H 
#define CSVREAD_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <istream>
#include <time.h>
#include <string.h>
#include <algorithm>

using namespace std;

struct Record{ string name; string strDate;  string getTitle(); string getEpisode();};
vector<Record> convertCsv(string filename, char delimiter);
int daysDiff(string dateFrom,string dateTo);
int daysDiffInc(string dateFrom,string dateTo);
int getWatchingDays(vector<Record> records);
string getMonthFormat(string date);
string getYearFormat(string date);

#endif