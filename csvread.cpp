#include "csvread.h" 

string getMonthFormat(string date) //20.06.22 -> 06.22 
{
    int index;

    index = date.find(".");
    if(index != string::npos) //20.06.2022
    {
        return date.substr(index+1,-1);
    }
    
    index = date.find("/");
    if(index != string::npos) //6/22/22 
    {
        return date.substr(0,index)+"/"+date.substr(date.length()-2,2);
    }

    return "0/00";
}

string getYearFormat(string date) //20.06.2022 -> 2022
{
    int index;

    index = date.find(".");
    if(index != string::npos) //20.06.2022
    {
        return date.substr(date.length()-4,4);
    }
    
    index = date.find("/");
    if(index != string::npos) //6/22/22 
    {
        return "20" + date.substr(date.length()-2,2);
    }

    return "00";
}

Record csv_read_row(std::istream &in, char delimiter)
{
    Record res;

    stringstream ss;
    bool inquotes = false;
    while(in.good())
    {
        char c = in.get();
        if (!inquotes && c=='"')
        {
            inquotes=true;
        }
        else if (inquotes && c=='"')
        {
            if ( in.peek() == '"') ss << (char)in.get();
            else inquotes=false;
        }
        else if (!inquotes && c==delimiter)
        {
            res.name = ss.str();
            ss.str("");
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            res.strDate = ss.str();
            if(in.peek()=='\n') { in.get(); }
            return res;
        }
        else  ss << c;
    }
    return res;
}

vector<Record> convertCsv(string filename, char delimiter) //return reversed csv file as vector of records
{   
    vector<Record> res;
    ifstream in(filename);
    if (in.fail()) {cout << "File not found" << endl; exit(1);}

    csv_read_row(in, delimiter); //header

    while(in.good())
    {
        Record rec = csv_read_row(in, delimiter);
        if(!rec.strDate.empty()) res.push_back(rec); // ekhme
    }
    in.close();

    reverse(res.begin(), res.end());
    return res;
}

void getDate(string strDate,int arr[3])
{
    if(strDate.find('.') != string::npos) //20.06.2022
    {
        int len = strDate.length();
        int index = strDate.find(".");
        arr[0] =   stoi(strDate.substr(0,index));
        int index2 =index+1+ strDate.substr(index+1,len-index-1).find(".");
        arr[1] =   stoi(strDate.substr(index+1,-1));
        arr[2] = stoi(strDate.substr(index2+1,-1));
    }
    else //6/22/22
    {
        int len = strDate.length();
        int index = strDate.find("/");
        arr[1] =   stoi(strDate.substr(0,index));
        int index2 =index+1+ strDate.substr(index+1,len-index-1).find("/");
        arr[0] =   stoi(strDate.substr(index+1,-1));
        arr[2] =2000+ stoi(strDate.substr(index2+1,-1));
    }
}

time_t stringToDate(string strDate)
{
    struct tm * date;

    int temp[3]; 
    getDate(strDate,temp);
    int day=temp[0], month = temp[1],year = temp[2];

    time_t rawtime;
    time ( &rawtime );
    date = localtime ( &rawtime );
    date->tm_year = year - 1900;
    date->tm_mon = month - 1;
    date->tm_mday = day;
    
    return mktime ( date );   
}

int daysDiff(string dateFrom,string dateTo)
{
    return (int)(difftime(stringToDate(dateTo), stringToDate(dateFrom)))/60/60/24;
}

int daysDiffInc(string dateFrom,string dateTo) //take last date inclusive
{
    return daysDiff(dateFrom,dateTo)+1;
}

int getWatchingDays(vector<Record> records)
{
    int cnt=0;
    string lastDate="";
    for (auto &record : records)
    {
        if(!lastDate.empty() && lastDate != record.strDate)
        {
            cnt++;
        }
        lastDate=record.strDate;
    }
    return cnt;
}

string Record::getTitle()
{
    int index = name.find(':');
    if (index == string::npos) return name;
    return name.substr(0,index);
}

string Record::getEpisode()
{
    int index = name.find(':');
    if (index != string::npos)  return name.substr(index+2); 
    return "";
}