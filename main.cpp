#include "all_use.h"

using namespace std;

bool getfile(string a,string b,string &c,string d)
{
    struct _finddata_t filefind;
    struct _finddata_t filefindi;
    string ai=a+"\\.sync";
    string aname,bname;
    if(access(ai.c_str(),0))
    {
        if(mkdir(ai.c_str()))
        {
            throw a;
        }
    }
    string bi=b+"\\.sync";
    if(access(bi.c_str(),0))
    {
        if(mkdir(bi.c_str()))
        {
            throw b;
        }
    }
    int n=_findfirst(bi.c_str(),&filefind);
    if(n==-1)
    {
        return false;
    }
    while(!_findnext(n,&filefind))
    {
        if(!strcmp(filefind.name,".."))continue;
        if(!strcmp(filefind.name,".sync"))continue;
        c=ai+"\\"+filefind.name;
        int n=_findfirst(c.c_str(),&filefindi);
        if(!n)
        {
            return true;
        }
    }
    return false;
}

void sync(string a,string b)
{
    string name;
    node *anode=new node(a.c_str());
    node *bnode=new node(b.c_str());
    catchToTree(anode);
    catchToTree(bnode);
    node *file;
    if(getfile(a,b,name)){
        node tmp("");
        FILE *fp=fopen(name.c_str(),"rb");
        readFromFile(&tmp,fp);
        file=tmp.value.sonlist->front();
        file->father=0;
        fclose(fp);
    }else{
        file=new node("");
    }
    list<change> *achli=cmp(anode,file);
    list<change> *bchli=cmp(bnode,file);
    lock alo(achli,a);
    lock blo(bchli,b);
    alo<<make_pair(bchli,b);
}

int main()
{
    sync("D:\\chydb\\word","D:\\word");
    return 0;
}
