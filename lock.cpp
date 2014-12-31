#include "all_use.h"

using namespace std;

lock::lock(std::list<change> *li,string path)
{
    for(change &a:*li)
    {
        typemap.insert({a.path,a.changetype});
    }
    this->path=path;
}

void lock::writechange(change *li,string source)
{
    auto a=typemap.count(li->path);
    if(a)
    {
        throw typemap.equal_range(li->path);
    }
    else
    {
        typemap.insert({li->path,li->changetype});
        string fromp=source+"\\";
        fromp+=li->path;
        string top=path+"\\";
        top+=li->path;
        switch(li->changetype)
        {
        case DELFILE:
            remove(top.c_str());
            break;
        case DELDIR:
            rmdirall(top);
            break;
        case NEWFILE:
            copyFile(fromp,top);
        case NEWDIR:
            copyDir(fromp,top);
        }
    }
}

void lock::operator=(lock &a)
{
    for(change b:a.typemap){

    }
}
