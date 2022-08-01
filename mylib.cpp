#include "mylib.h"

void loading(int seconds)
{
    for (int i =0;i<seconds;i++)
    {
        cout<<"LOADING |"<<endl;
        usleep(1000*250);
        system("clear");
        cout<<"LOADING \\"<<endl;
        usleep(1000*250);
        system("clear");
        cout<<"LOADING -"<<endl;
        usleep(1000*250);
        system("clear");
        cout<<"LOADING /"<<endl;
        usleep(1000*250);
        system("clear");
    }
    system("clear");
}

void Video::addRecord(Record record)
{
    episodes.push_back( record.getEpisode() );
    lastDate = record.strDate;
    if(getEpisodesLength() >1)
        breaks.push_back(daysDiff(firstDate,lastDate));
}

void Video::setTypeAuto()
{
    if(episodes.back().empty())  {videoType = MOVIE;   return;  } 

    size_t cnt = count(episodes.back().begin(), episodes.back().end(), ':'); //last episode : cnt
    if(cnt >0) videoType = MULTISERIES;
    else 
    {
        if(episodes.size()>1) videoType = ONESERIES;
        else videoType = UNKNOWN;
    }            
}

void Video::setBestSessionAuto()
{
    bestSession.startDate = firstDate;
    bestSession.endDate = lastDate;
    bestSession.cnt = getEpisodesLength();
}

map<string, Video> toVideoMap(vector<Record> &records)
{
    map<string, Video> vMap; //title video

    string title;
    for(auto & rec : records)
    {
        title = rec.getTitle();
        if(vMap.find(title) != vMap.end()) vMap.at(title).addRecord(rec);
        else
        {
            Video newVideo(title,rec.strDate);
            newVideo.addRecord(rec);
            vMap.insert({title, newVideo});
        }
    }
    return vMap;
}

void setVideoTypesAndCount(map<string, Video> &vMap,int cnts[4])
{
    for (auto& video : vMap) 
    {
        video.second.setTypeAuto();
        if(video.second.getType()==UNKNOWN)
            video.second.setType(ONESERIES); // bledy pomijalne
        
        if(video.second.getType() == MOVIE) cnts[0]++;
        else if(video.second.getType() ==ONESERIES) cnts[1]++;
        else if(video.second.getType() == MULTISERIES) cnts[2]++;
        else cnts[3]++;
    }
}

vector<pair<string, int >> getYears(vector<Record> records)
{
    string lastYear,temp;
    int cnt = 1;
    vector<pair<string,int>> res;

    for(auto& rec : records)
    {
        if(lastYear.empty()) lastYear = getYearFormat(rec.strDate);
        else
        {
            temp = getYearFormat(rec.strDate);
            if(lastYear == temp)
            {
                cnt++;
            }
            else
            {
                res.push_back(make_pair(lastYear,cnt));
                lastYear = temp;
                cnt=1;
            }
        }
    }
    res.push_back(make_pair(lastYear,cnt));

    return res;
}

vector<pair<string, int >> getMonths(vector<Record> records)
{
    string lastMonth,temp;
    int cnt = 1;
    vector<pair<string,int>> res;

    for(auto& rec : records)
    {
        if(lastMonth.empty()) lastMonth = getMonthFormat(rec.strDate);
        else
        {
            temp = getMonthFormat(rec.strDate);
            if(lastMonth == temp)
            {
                cnt++;
            }
            else
            {
                res.push_back(make_pair(lastMonth,cnt));
                lastMonth = temp;
                cnt=1;
            }
        }
    }
    res.push_back(make_pair(lastMonth,cnt));

    return res;
}

void getWatch(vector<tuple<string,string>> &titles1,vector<tuple<string,string>> &titles2,vector<Record> &records,map<string,Video> &vMap,int watchB,int watchE)
{
    vector<string> helper;
    string title;

    auto it = records.begin();
    while(titles2.size() != watchE)
    {
        auto title = it->getTitle();
        if(find(helper.begin(),helper.end(),title) == helper.end())
        {
            helper.push_back(title);
            titles2.push_back(make_tuple(title,to_string(vMap.at(title).getType())));
        }
        it++;
    }
    reverse(titles2.begin(),titles2.end());

    helper.clear();
    it = records.end();
    while(titles1.size() != watchB)
    {
        it--;
        title = it->getTitle();
        if(find(helper.begin(),helper.end(),title) == helper.end())
        {
            helper.push_back(title);
            titles1.push_back(make_tuple(title,to_string(vMap.at(title).getType())));
        }
    }
}

map<string, int> getEpisodesWithCnt(string title,vector<string> episodes, vector<Record> &records)
{
    map<string,int> res;
    for(auto& epi:episodes) res.insert(make_pair(epi,0));

    for(auto & rec : records)
    {
        if(rec.getTitle() == title) res[rec.getEpisode()]++;
    }

    return res;
}

vector<pair<string ,int>> getMost(map<string,Video> &entries, int size)
{
    vector<pair<string, Video>> sorted;
    Video newVideo("","");
    sorted.resize(size,make_pair("",newVideo));
    
    partial_sort_copy(entries.begin(), entries.end(),
                      sorted.begin(), sorted.end(),
                      [](const pair<string, Video> &a, const pair<string, Video> &b)
    {
        return !(a.second.getEpisodesLength() < b.second.getEpisodesLength());
    });
    vector<pair<string,int>> conv;
    for(pair<string,Video> item : sorted)
    {
        conv.push_back(make_pair(item.first,item.second.getEpisodesLength()));
    }
    return conv;
}

int getDislike(map<string,Video> &vMap,int ratio,vector<string> &titles) //return length + fill titles
{
    int cnt = 0;
    for (const auto& video : vMap) 
    {
        if(video.second.getType()==ONESERIES || video.second.getType()==MULTISERIES)
        {
            if(video.second.getEpisodesLength() <= ratio)
            {
                if(cnt <= 10) titles.push_back(video.first);
                cnt++;
            }
              
        }
    }
    return cnt;
}

vector<tuple<string,string,int,int>>getEst(map<string,Video> &vMap,int size,int mode)
{
    vector<pair<string, Video>> sorted;
    Video newVideo("","");
    sorted.resize(size,make_pair("",newVideo));

    if(mode == 0) //fastest
    {
        partial_sort_copy(vMap.begin(), vMap.end(),
        sorted.begin(), sorted.end(),
        [](const pair<string, Video> &a, const pair<string, Video> &b)
        {
            dDate temp1 = a.second.getBestSession();
            dDate temp2 = b.second.getBestSession();
            int delta1 = daysDiffInc(temp1.startDate,temp1.endDate);
            if(delta1 < 5) return false; //define minimum days

            int delta2 = daysDiffInc(temp2.startDate,temp2.endDate);
            if(delta2 < 5) return true;

            return !(temp1.cnt/delta1< temp2.cnt/delta2);
        });
    }
    else //longest
    {
        partial_sort_copy(vMap.begin(), vMap.end(),
        sorted.begin(), sorted.end(),
        [](const pair<string, Video> &a, const pair<string, Video> &b)
        {
            dDate temp1 = a.second.getBestSession();
            dDate temp2 = b.second.getBestSession();

            return !(daysDiffInc(temp1.startDate,temp1.endDate) <  daysDiffInc(temp2.startDate,temp2.endDate));
        });
    }
   
    vector<tuple<string,string,int,int>> conv;
    for(pair<string,Video> item : sorted)
    {
        string title = item.first;
        string startDate = item.second.getBestSession().startDate;
        int cnt = item.second.getBestSession().cnt;
        int days = daysDiffInc(startDate,item.second.getBestSession().endDate);
        conv.push_back(make_tuple(title,startDate,cnt,days));
    }
    return conv;
}

tuple<int,string,string,vector<string>,int> getStrike(vector<Record> &records)
{
    int bestStrike = 0; //in days
    int bestStartIndex;
    string bestLastDate;

    int strike = 1; //in days
    int startIndex=0; //start index of strike
    int cnt=0;
    string firstDate="";
    for(auto& record : records)
    {
        if(firstDate.empty()) firstDate = record.strDate;
        else
        {
            if(daysDiff(firstDate,record.strDate)==1)
            {
                strike++;
                firstDate = record.strDate;
            }
            else if(firstDate != record.strDate) //<=>  daysDiff > 1 
            {
                if(bestStrike <= strike)
                {
                    bestStrike = strike;
                    bestStartIndex = startIndex;
                    bestLastDate = firstDate;
                }
                startIndex = cnt;
                strike = 1;
                firstDate = record.strDate;
            }
        }
        cnt++;
    }
    
    //
    vector<string> titles;

    string tempTitle;
    cnt=0;
    while (records[bestStartIndex+cnt].strDate != bestLastDate)
    {
        tempTitle = records[bestStartIndex+cnt].getTitle();
        if (find(titles.begin(), titles.end(), tempTitle) == titles.end()) titles.push_back(tempTitle);
        cnt++;
    }

    return make_tuple(bestStrike,records[bestStartIndex].strDate,bestLastDate,titles,cnt);
}

vector<tuple<string,vector<string>>> getBinge(vector<Record> &records,int size)
{
    vector<tuple<string,vector<string>>> res;
    vector<string> temp;

    int bestCnt = 0;
    string bestDate="";

    string date="";
    int cnt=1;
    for (int i =0; i < size;i++)
    {
        for (auto& record : records)
        {
            if(find(temp.begin(), temp.end(), record.strDate)!=temp.end()) continue; //omija daty ktore sa best

            if(date.empty()) date= record.strDate;
            else
            {
                if(date == record.strDate)
                {
                    cnt++;
                }
                else
                {
                    if(bestCnt <= cnt)
                    {
                        bestCnt = cnt;
                        bestDate = date;
                    }
                    date = record.strDate;
                    cnt = 1;
                }
            }
        }
        if(bestCnt <= cnt) { bestCnt = cnt;  bestDate = date; } //last one
        
        if(!bestDate.empty()) //jesli bedzie za malo recordow zeby znalezc kolejne
        {
            vector<string> newTemp;
            for (auto& record : records)
            {
                if(record.strDate == bestDate) newTemp.push_back(record.name);
                else if(!newTemp.empty()) break;
            }
            
            res.push_back(make_tuple(bestDate,newTemp));
            temp.push_back(bestDate);
        }
        
        bestCnt =0;
        bestDate ="";
        date ="";
        cnt = 1;
    }
    return res;
}

tuple<pair<string,string>,int,pair<string,string>> getGap(vector<Record> &records)
{
    pair<string,string> dates,names;

    int cnt,maxCnt=0;
    string date = "";//last date
    string name;//last name
    for(auto& rec : records)
    {
        if(date.empty()) {date = rec.strDate; name = rec.name;}
        else
        {
            cnt = daysDiffInc(date, rec.strDate);
            if(cnt >= maxCnt)
            {
                dates.first = date;
                dates.second = rec.strDate;
                names.first = name;
                names.second = rec.name;

                maxCnt = cnt;
            }
            date = rec.strDate;
            name = rec.name;
        }
    }
    return make_tuple(dates,maxCnt,names);
}

void start(aSettings a,bSettings b)
{
    //wizualne
    loading(5);

    //przygotowanie
    vector<Record> records = convertCsv(a.fileName,',');
    map<string, Video> vMap = toVideoMap(records);

    //set video types + count
    int cnts[4] = {0,0,0,0}; // movie ,one , multi, unknown
    setVideoTypesAndCount(vMap,cnts);

    //builder + factory
    Factory factory(a.screenSize);
    Builder builder(factory);

    //1
    string dateFrom = records.front().strDate;
    string dateTo = records.back().strDate;
    int period = daysDiffInc(dateFrom,dateTo); //csvread function
    int watch = getWatchingDays(records); //csvread function

    builder.title(a.title,dateFrom,dateTo);
    builder.titleDown(period,watch);

    //2
    int totalTitles = cnts[0]+ cnts[1] + cnts[2]+ cnts[3];
    int totalEpisodes = records.size();
    int totalMinutes = a.movieLen * cnts[0] + a.seriesLen * (cnts[1] + cnts[2])+ (a.movieLen+a.seriesLen)/2 * cnts[3];

    builder.total(totalTitles,cnts[0],cnts[1],cnts[2]);
    builder.totalEpisodes(totalEpisodes,(double)totalEpisodes/period);
    int fixedMinutes = watch*totalMinutes/totalTitles;
    int avgMinutes = (fixedMinutes+totalMinutes) / 2;
    builder.totalHours(fixedMinutes/60,totalMinutes/60,avgMinutes/60,(double)avgMinutes/60/period);
    
    //2.3
    builder.years(getYears(records));

    //2.5 optional
   //builder.months(getMonths(records));
    
    //3
    vector<tuple<string,string>> titles1,titles2;
    getWatch(titles1,titles2,records,vMap,b.watchBegin,b.watchEnd);
    builder.watch(titles1,titles2);
   
   //4
    builder.most(getMost(vMap,b.most));

    //5
    vector<string> temp;
    int cnt2 = getDislike(vMap,b.dislike,temp);
    builder.dislike(cnts[1] + cnts[2],cnt2,temp);

    //7
    for (auto& video : vMap) video.second.setBestSessionAuto();

    builder.fastest(getEst(vMap,b.fastest,0));
    builder.longest(getEst(vMap,b.longest,1));
    //8
    tuple<int,string,string,vector<string>,int> res = getStrike(records);
    builder.strike(get<0>(res),get<1>(res),get<2>(res),get<3>(res),get<4>(res));
   
    //9
    vector<tuple<string,vector<string >>> temp2 = getBinge(records,b.binge);
    builder.binge(temp2);

    //10
    tuple<pair<string,string>,int,pair<string,string>> gapRes = getGap(records);
    builder.gap(get<0>(gapRes).first,get<0>(gapRes).second, get<1>(gapRes),get<2>(gapRes).first,get<2>(gapRes).second);

    string command;
    cout<<"Type title for more info or quit (Q) :" << endl;
    cout<<">> "; getline(cin, command);
    
    while (command != "Q" || command != "q")
    {
        if(vMap.find(command) != vMap.end())
        {
            VideoType type =  vMap.at(command).getType();
            if(type == ONESERIES || type == MULTISERIES)
            {
                Video vid = vMap.at(command);
                dDate bestSession = vid.getBestSession();

                builder.aboutSeries(vid.getTitle(),to_string(vid.getType()),vid.getEpisodesLength(),
                vid.getFirstDate(),vid.getLastDate(),vid.getEpisodes(),vid.getBreaks(),
                make_tuple(bestSession.startDate, daysDiffInc(bestSession.startDate,bestSession.endDate),
                bestSession.cnt));
  
                cout<<"Another one ?" << endl;
            }
            else if( type == MOVIE)
            {
                Video vid = vMap.at(command);

                builder.aboutMovie(vid.getTitle(),to_string(vid.getType()),vid.getFirstDate());

                cout<<"Another one ?" << endl;
            }
            else cout<< "Type of this title is unknown. :<";
        }
        else cout<<"Title not found... try again: "<<endl;

        cout<<">> "; getline(cin, command);
    }
    
}
