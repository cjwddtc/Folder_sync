#include<iostream>
#include<string>
#include<io.h>
#include<vector>
#include<stack>

using namespace std;
class node;
//目录和文件不同的属性
union asdf {
    long n;
    std::vector<node*> *sonlist;
};

//目录节点
class node{
public:
    bool is_dir;
    union asdf value;
    string name;
    node *father;
    node(const char *name,node *father=0){
        this->name=name;
        this->is_dir=true;
        this->value.sonlist=new std::vector<node*>();
        this->father=father;
    }
    node(const char *name,long n,node *father=0){
        this->name=name;
        this->is_dir=false;
        this->value.n=n;
        this->father=father;
    }
};
//获取当前目录下的完整路径
string getpath(node *root){
    string fullpath="";
    node *a=root;
    std::stack<node *> sta;
    while(a->father!=0){
        sta.push(a);
        a=a->father;
    }
    fullpath+=a->name+"\\";
    while(!sta.empty()){
        fullpath+=sta.top()->name+"\\";
        sta.pop();
    }
    return fullpath;
}

//利用递归调用以root为根节点建立对应目录结构的树
void differToTree(node *root){
    struct _finddata_t filefind;
    string fullpath=getpath(root);
    fullpath+="*";
    int n=_findfirst(fullpath.c_str(),&filefind);
    if(n==-1){
        return;
    }
    while(!_findnext(n,&filefind)){
        if(!strcmp(filefind.name,".."))continue;
        if(filefind.attrib==_A_SUBDIR){
            node *a=new node(filefind.name,root);
            root->value.sonlist->push_back(a);
            differToTree(a);
        }else{
            node *a=new node(filefind.name,(long)(filefind.time_write),root);
            root->value.sonlist->push_back(a);
        }
    }
}
//将书写入文件
void writeToFile(node *root,FILE *fp){
    if(root->is_dir){
        //标志表明是目录
        putc(1,fp);
        fprintf(fp,"%s\n",root->name.c_str());
        //写入目录包含文件数量
        int n=root->value.sonlist->size();
        fwrite(&n,sizeof(n),1,fp);
        for(node *a:*(root->value.sonlist)){
            writeToFile(a,fp);
        }
    }else{
        //标志是文件
        putc(0,fp);
        fprintf(fp,"%s\n",root->name.c_str());
        //写入修改时间
        fwrite(&(root->value.n),sizeof(root->value.n),1,fp);
    }
}
//从文件读取目录树
void readFromFile(node *root,FILE *fp){
    int n=fgetc(fp);
    char a[1024];
    if(n==1){
        fgets(a,1024,fp);
        node *b=new node(a,root);
        root->value.sonlist->push_back(b);
        fread(&n,sizeof(int),1,fp);
        for(int i=0;i<n;i++){
            readFromFile(b,fp);
        }
    }else{
        long n;
        fgets(a,1024,fp);
        fread(&n,sizeof(n),1,fp);
        new node(a,n,root);
    }
}
//删除目录树
void delall(node *root){
    if(root->is_dir){
        for(node *a:*(root->value.sonlist)){
            delall(a);
        }
        delete root;
    }else{
        delete root;
    }
}
//打印目录树
void print(node *root,int n=0){
    for(int i=0;i<n;i++){
        printf(" ");
    }
    if(root->is_dir){
        printf("%s\n",root->name.c_str());
        n++;
        for(node *a:*(root->value.sonlist)){
            print(a,n);
        }
    }else{
        printf("%s\n",root->name.c_str());
    }
}

int main()
{
    node root("D:\\下载");
    differToTree(&root);
    print(&root);
    return 0;
}
