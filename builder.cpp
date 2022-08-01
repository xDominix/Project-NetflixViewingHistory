#include "builder.h" 

string toupper(string text)
{
    for (auto & c: text) c = toupper(c);
    return text;
};

void Builder::title(string title,string date1,string date2)
{
    factory.head();
    factory.head(title);
    factory.head(date1 +" - "+date2);
    factory.head();
}
void Builder::titleDown(int period,int watch)
{
    double ratio = (double)watch/period * 100;
    factory.body("REPORT PERIOD",to_string(period)+" DAYS");
    factory.body("WATCHING - DAYS",to_string(watch)+" DAYS ("+
    to_string(ratio).substr(0,2)+"%)");
    factory.head();
    factory.margin();
}
void Builder::total(int total,int movie, int one,int multi)
{
    factory.head("TOTAL WATCHED");
    factory.body("ALL" , to_string(total));
    factory.body("> MOVIES",to_string(movie));
    factory.body("> ONE-SEASON SERIES",to_string(one));
    factory.body("> MULTI-SEASON SERIES",to_string(multi));
    factory.head();
}
void Builder::totalEpisodes(int epi,double day)
{
    factory.body("EPISODES WATCHED",to_string(epi));
    factory.body("IT IS","PER DAY!",to_string(day).substr(0, 5));
      factory.body("IT IS","PER MONTH!",to_string(day*30).substr(0, 5));
    factory.head();
}
void Builder::totalHours(int r1,int r2 ,int hours,double day)
{
    if(r1 <= r2) factory.body("WATCH TIME",to_string(r1)+"H - "+to_string(r2)+"H");
    else factory.body("WATCH TIME",to_string(r2)+"H - "+to_string(r1)+"H");
    factory.body("AVG","~ "+to_string(hours)+"H");
    int h = (int)day;
    int m = (int)(60 * day) % 60;
    factory.body("IT IS","PER DAY!",to_string(h) + "H "+to_string(m)+ "MIN" );
    factory.head();
    factory.margin();

}

void Builder::years(vector<pair<string,int>> years)
{
    if(years.size()>1)
    {
        factory.head("OVER THE YEARS");
        for(auto& year :years)
            factory.body(year.first,to_string(year.second)+" EPISODES");
        factory.head();
        factory.margin();
    }
    
}

void Builder::months(vector<pair<string,int>> months)
{
    if(months.size()>1)
    {
        factory.head("OVER THE MONTHS");
        for(auto& month :months)
            factory.body(month.first,to_string(month.second)+" EPISODES");
        factory.head();
        factory.margin();
    }
    
}

void Builder::watch(vector<tuple<string,string>> titles1,vector<tuple<string,string>> titles2)
{
    factory.head("WATCH HISTORY");
    for (auto title = titles1.begin(); title != titles1.end();title++) factory.body(get<0>(*title),get<1>(*title));
    factory.body("...");
    for (auto title = titles2.begin(); title != titles2.end();title++) factory.body(get<0>(*title),get<1>(*title));
    factory.body("","","^ remember them ? ^");
    factory.head();
    factory.margin();
}
void Builder::most(vector<pair<string ,int>> titles)
{
    factory.head("MOST WATCHED");
    for (auto title = titles.begin(); title != titles.end();title++) factory.body((*title).first,to_string((*title).second)+" EPISODES");
    factory.head();
    factory.margin();
}
void Builder::dislike(int totalSeries,int totalDislikes,vector<string> titles)
{
    double percent = (double)totalDislikes/totalSeries*100;
    factory.head("DISLIKED SERIES ("+to_string(percent).substr(0, 4) +"%)");
    factory.body("TITLES YOU BARELY STARTED:");
    if(titles.size() != totalDislikes) titles.push_back(" ...");
    factory.lbody(titles);
    factory.body("","TOTAL: "+to_string(totalDislikes));
    factory.head();
    factory.margin();
}

void Builder::fastest(vector<tuple<string,string,int,int>> sessions)
{
    int cnt =0;

    factory.head("THE FASTEST SESSIONS");
    for(auto &session : sessions) 
    {
        factory.body("TOP "+to_string(++cnt)+" - "+get<0>(session),get<1>(session));
        factory.body("> You watched: "+to_string(get<2>(session))+" episodes in "+to_string(get<3>(session))+" days!");
        factory.body("> It is " +to_string((double)get<2>(session)/get<3>(session)).substr(0,5)+" per day.");
        factory.margin();
    }
}

void Builder::longest(vector<tuple<string,string,int,int>> sessions)
{
    int cnt =0;

    factory.head("THE LONGEST SESSIONS");
    for(auto & session:sessions) 
    {
        factory.body("TOP "+to_string(++cnt)+" - "+get<0>(session),get<1>(session));
        factory.body("> You watched: "+to_string(get<2>(session))+" episodes in "+to_string(get<3>(session))+" days!");
        factory.margin();
    }
    factory.head();
    factory.margin();
}
void Builder::strike(int days,string date1,string date2,vector<string> titles,int total)
{
    factory.head("IT'S A STRIKE!");
    factory.body("WATCH STREAK",to_string(days)+" DAYS IN A ROW!");
    factory.margin();
    factory.body("PERIOD: "+date1+" - "+ date2);
    factory.lbody(titles,"WATCHED TITLES: ");
    factory.body("TOTAL EPISODES: "+ to_string(total));
    factory.head();
    factory.margin();
}
void Builder::binge(vector<tuple<string,vector<string>>> dates)
{
    factory.head("BINGE-WATCHING OR INDECISIVE DAY ?");
    for(auto& date:dates)
    {
        factory.body("On "+ get<0>(date)+" you watched:");
        for (auto& episode: get<1>(date)) factory.body("> "+episode);
        factory.margin();
    }
    factory.head();
    factory.margin();
}

void Builder::gap(string date1,string date2,int days, string name1,string name2)
{
    factory.head("THE LONGEST GAP");
    factory.body("You didn't watch anything on dates: ");
    factory.body("> "+date1+" - "+ date2);
    factory.body("Ahh "+to_string(days)+" days of relax...");
    factory.body("What did you do then ?");
    factory.margin();
    factory.body("Before the gap you watched:");
    factory.body("> "+ name1);
    factory.body("After the gap you watched:");
    factory.body("> "+ name2);
    factory.head();
    factory.margin();
}

void Builder::aboutSeries(string title,string type, int epi,string date1,string date2,
vector<string> epis,vector<int> breaks, tuple<string,int,int> bestSession)
{
    int cnt = -1;

    factory.margin();
    factory.head("ABOUT: "+ toupper(title));
    factory.body("TYPE",type);
    factory.body("EPISODES WATCHED",to_string(epi));
    factory.body("FIRST EPISODE WATCHED",date1);
    factory.body("LAST EPISODE WATCHED",date2);
    factory.margin();
    factory.body("","","LIST OF EPISODES");
    factory.body("NAME","DAYS DELTA");
    for(auto& ept : epis)
    {        
        if(cnt==-1) factory.body(ept);
        else factory.body(ept,"+"+to_string(breaks[cnt]));
        cnt++;
    }
    factory.margin();
    factory.body("",get<0>(bestSession),"BEST SESSION");
    factory.body("You watched "+to_string(get<2>(bestSession))+" episodes in "+ to_string(get<1>(bestSession))+" days!" );
    factory.head(); 
    factory.margin();
}
void Builder::aboutMovie(string title,string type,string date)
{
    factory.margin();
    factory.head("ABOUT: "+ toupper(title));
    factory.body("TYPE",type);
    factory.body("WATCHED DATE",date);
    factory.head();
    factory.margin();
}
