#include "all_use.h"

using namespace std;

//比较两书，返回值需释放内存
list<change> *cmp(node *newroot,node *oldroot,list<change> *li)
{
    change di;
    bool flag=false;
    if(li==0)
    {
        li=new list<change>();
        flag=true;
    }
    auto ito=oldroot->value.sonlist->begin();
    auto itoe=oldroot->value.sonlist->end();
    auto itn=newroot->value.sonlist->begin();
    auto itne=newroot->value.sonlist->end();
    while(ito!=itoe && itn!=itne)
    {
        if(**itn>**ito)
        {
            di.changetype=(*ito)->is_dir?DELDIR:DELFILE;
            di.path=getxdpath(*ito);
            li->push_back(di);
            ito++;
        }
        else if(**itn<**ito)
        {
            di.changetype=(*itn)->is_dir?NEWDIR:NEWFILE;
            di.path=getxdpath(*itn);
            li->push_back(di);
            itn++;
        }
        else
        {
            if((*itn)->is_dir)
            {
                if((*ito)->is_dir)
                {
                    cmp(*itn,*ito,li);
                }
                else
                {
                    di.changetype=DELFILE;
                    di.path=getxdpath(*ito);
                    li->push_back(di);
                    di.changetype=NEWDIR;
                    di.path=getxdpath(*itn);
                    li->push_back(di);
                }
            }
            else if((*ito)->is_dir)
            {
                di.changetype=DELDIR;
                di.path=getxdpath(*ito);
                li->push_back(di);
                di.changetype=NEWFILE;
                di.path=getxdpath(*itn);
                li->push_back(di);
            }
            else
            {
                if((*itn)->value.n>(*ito)->value.n)
                {
                    di.changetype=DELFILE;
                    di.path=getxdpath(*itn);
                    li->push_back(di);
                    di.changetype=NEWFILE;
                    di.path=getxdpath(*ito);
                    li->push_back(di);
                }
            }
            ito++;
            itn++;
        }
    }
    while(itn!=itne)
    {
        di.changetype=(*itn)->is_dir?NEWDIR:NEWFILE;
        di.path=getxdpath(*itn);
        li->push_back(di);
        itn++;
    }
    while(ito!=itoe)
    {
        di.changetype=(*ito)->is_dir?DELDIR:DELFILE;
        di.path=getxdpath(*ito);
        li->push_back(di);
        ito++;
    }
    if(flag)
    {
        string a;
        li->sort([](change &a1,change &a2)
        {
            if(a1.changetype==a2.changetype)
                return a1.path.compare(a2.path)<0;
            else
                return a1.changetype<a2.changetype;
        });
    }
    return li;
}

void rmdirall(string path)
{
    struct _finddata_t filefind;
    int n=_findfirst(path.c_str(),&filefind);
    while(!_findnext(n,&filefind))
    {
        string f=path+"\\"+filefind.name;
        if(filefind.attrib==_A_SUBDIR)
        {
            rmdirall(f);
        }
        else
        {
            remove(f.c_str());
        }
    }
}

void copyFile(string source,string des)
{
    const int BS=1000000;
    void *ptr=malloc(BS);
    FILE *in=fopen(source.c_str(),"rb");
    FILE *out=fopen(des.c_str(),"wb");
    fseek(in,0,2);
    long n=ftell(in);
    rewind(in);
    int i=n/BS;
    printf("copying \"%s\" to \"%s\"\n",source.c_str(),des.c_str());
    for(int j=0; j<i; j++)
    {
        fread(ptr,BS,1,in);
        fwrite(ptr,BS,1,out);
        printf("%dm\r",j);
    }
    fread(ptr,n%BS,1,in);
    fwrite(ptr,n%BS,1,out);
    fclose(out);
    fclose(in);
}

void copyDir(string source,string des)
{
    struct _finddata_t filefind;
    string full_path=source+"\\*";
    int n=_findfirst(full_path.c_str(),&filefind);
    mkdir(des.c_str());
    while(!_findnext(n,&filefind))
    {
        if(!strcmp(filefind.name,".."))continue;
        string f=source+"\\"+filefind.name;
        string t=des+"\\"+filefind.name;
        if(filefind.attrib==_A_SUBDIR)
        {
            copyDir(f,t);
        }
        else
        {
            copyFile(f,t);
        }
    }
}
