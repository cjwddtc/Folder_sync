#include "all_use.h"

using namespace std;

//获取当前目录下的完整路径
string getpath(node *root)
{
    string fullpath="";
    node *a=root;
    std::stack<node *> sta;
    while(a->father!=0)
    {
        sta.push(a);
        a=a->father;
    }
    fullpath+=a->name;
    while(!sta.empty())
    {
        fullpath+="\\"+sta.top()->name;
        sta.pop();
    }
    return fullpath;
}
//获取相对路径
string getxdpath(node *root)
{
    string fullpath="";
    node *a=root;
    std::stack<node *> sta;
    while(a->father!=0)
    {
        sta.push(a);
        a=a->father;
    }
    while(!sta.empty())
    {
        fullpath+=sta.top()->name+"\\";
        sta.pop();
    }
    fullpath.erase(fullpath.size()-1);
    return fullpath;
}

//利用递归调用以root为根节点建立对应目录结构的树
void catchToTree(node *root)

{
    struct _finddata_t filefind;
    string fullpath=getpath(root);
    fullpath+="\\*";
    int n=_findfirst(fullpath.c_str(),&filefind);
    if(n==-1)
    {
        return;
    }
    while(!_findnext(n,&filefind))
    {
        if(!strcmp(filefind.name,".."))continue;
        if(!strcmp(filefind.name,".sync"))continue;
        if(filefind.attrib==_A_SUBDIR)
        {
            node *a=new node(filefind.name,root);
            root->value.sonlist->push_back(a);
            catchToTree(a);
            a->value.sonlist->sort([](node *a,node *b)
            {
                return a->name.compare(a->name);
            });
        }
        else
        {
            node *a=new node(filefind.name,(long)(filefind.time_write),root);
            root->value.sonlist->push_back(a);
        }
    }
}
//将书写入文件
void writeToFile(node *root,FILE *fp)
{
    if(root->is_dir)
    {
        //标志表明是目录
        putc(1,fp);
        fprintf(fp,"%s\n",root->name.c_str());
        //写入目录包含文件数量
        int n=root->value.sonlist->size();
        fwrite(&n,sizeof(n),1,fp);
        for(node *a:*(root->value.sonlist))
        {
            writeToFile(a,fp);
        }
    }
    else
    {
        //标志是文件
        putc(0,fp);
        fprintf(fp,"%s\n",root->name.c_str());
        //写入修改时间
        fwrite(&(root->value.n),sizeof(root->value.n),1,fp);
    }
}
//从文件读取目录树
void readFromFile(node *root,FILE *fp)
{
    int n=fgetc(fp);
    char a[1024];
    if(n==1)
    {
        fgets(a,1024,fp);
        a[strlen(a)-1]=0;
        node *b=new node(a,root);
        root->value.sonlist->push_back(b);
        fread(&n,sizeof(int),1,fp);
        for(int i=0; i<n; i++)
        {
            readFromFile(b,fp);
        }
    }
    else
    {
        long n;
        fgets(a,1024,fp);
        a[strlen(a)-1]=0;
        fread(&n,sizeof(n),1,fp);
        node *b=new node(a,n,root);
        root->value.sonlist->push_back(b);
    }
}
//删除目录树
void delall(node *root)
{
    if(root->is_dir)
    {
        for(node *a:*(root->value.sonlist))
        {
            delall(a);
        }
        delete root;
    }
    else
    {
        delete root;
    }
}
//打印目录树
void print(node *root,int n)
{
    for(int i=0; i<n; i++)
    {
        printf(" ");
    }
    if(root->is_dir)
    {
        printf("%s\n",root->name.c_str());
        n++;
        for(node *a:*(root->value.sonlist))
        {
            print(a,n);
        }
    }
    else
    {
        printf("%s\n",root->name.c_str());
    }
}
