#include "factory.h" 

string toTitle(string text)
{
    for (auto & c: text) c = toupper(c);
    return " " + text+ " ";
}

string cutt(string text, int length)
{
    if(text.length() <= length) return text;

    return text.substr(0, length-3)+ "...";
}

Factory::Factory(int newScreenSize)
{
    if(newScreenSize< MINSCREENSIZE)  screenSize=MINSCREENSIZE;
    else
    {
        if(screenSize%2 ==1) screenSize = newScreenSize;
        else screenSize = newScreenSize+1; 
    }
}

void Factory::head(string text)
{
    string res(this->screenSize , '-');

    if(text.empty()) cout<<res<<endl;
    else
    {
        text = toTitle(text);
        int len = text.length();
        int base = screenSize/2 - len/2;
        for(int i =0;i<len;i++)
        {
            res[base+ i] = text[i];
        }
        cout << res << endl;
    }
}

void Factory::body(string left,string right,string center) //cutter
{
    string res(this->screenSize , ' ');

    //cutt left if too long
    int lmaxlen,len,base;
    if(!center.empty()) lmaxlen = screenSize/2 - center.length()/2;
    else lmaxlen = screenSize - right.length();
    left = cutt(left,lmaxlen -TEXTGAP); 

    //cutt right if too long
    //SOON

    len = left.length();
    base = 0;
    for(int i =0;i<len;i++) res[base + i] = left[i];

    len = center.length();
    base = screenSize/2 - len/2;
    for(int i =0;i<len;i++) res[base + i] = center[i];

    len = right.length();
    base = screenSize - len;
    for(int i =0;i<len;i++) res[base + i] = right[i];

    cout<< res << endl;
}

void Factory::lbody(vector<string> list,string title) //list left body , pagination
{
    string res(this->screenSize , ' ');

    int len,cnt=0;

    len = title.length();
    for (int i =0;i<len;i++) res[cnt++] = title[i];

    for (auto it = list.begin(); it != list.end();it++) 
    {
        len = (*it).length();
        if(screenSize - cnt-2 <= len) //is not enogh space
        {
            cout<<res<<endl;
            fill(res.begin(), res.begin()+this->screenSize, ' ');
            cnt=0;
        }

        for(int j=0;j<len;j++) res[cnt++] = (*it)[j];
        if(it+1 !=list.end())
        {
            res[cnt++] = ','; 
            res[cnt++] = ' ';
        }
    }
    cout<<res<<endl;
}

void Factory::margin()
{
    cout<<endl;
}