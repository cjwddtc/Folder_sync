#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include<iostream>
#include<string>
#include<io.h>
#include<list>
#include<stack>

class node;
//目录和文件不同的属性
union asdf {
    long n;
    std::list<node*> *sonlist;
};

//目录节点
class node{
public:
    bool is_dir;
    union asdf value;
    std::string name;
    node *father;
    node(const char *name,node *father=0){
        this->name=name;
        this->is_dir=true;
        this->value.sonlist=new std::list<node*>();
        this->father=father;
    }
    node(const char *name,long n,node *father=0){
        this->name=name;
        this->is_dir=false;
        this->value.n=n;
        this->father=father;
    }
};
std::string getpath(node *root);
void catchToTree(node *root);
void writeToFile(node *root,FILE *fp);
void readFromFile(node *root,FILE *fp);
void delall(node *root);
void print(node *root,int n=0);

#endif // TREE_H_INCLUDED
