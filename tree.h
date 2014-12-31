#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

class node;
//目录和文件不同的属性
union asdf
{
    long n;
    std::list<node*> *sonlist;
};

//目录节点
class node
{
public:
    bool is_dir;
    union asdf value;
    std::string name;
    node *father;
    node(const char *name,node *father=0)
    {
        this->name=name;
        this->is_dir=true;
        this->value.sonlist=new std::list<node*>();
        this->father=father;
    }
    node(const char *name,long n,node *father=0)
    {
        this->name=name;
        this->is_dir=false;
        this->value.n=n;
        this->father=father;
    }
    bool operator<(node &a)
    {
        return this->name.compare(a.name)<0;
    }
    bool operator>(node &a)
    {
        return this->name.compare(a.name)>0;
    }
    bool operator==(node &a)
    {
        if(this->is_dir==a.is_dir)
            if(this->name.compare(a.name)==0)
            {
                if(!this->is_dir)
                {
                    if(this->value.n==a.value.n)
                        return true;
                }
                else return true;
            }
        return false;
    }
    bool operator!=(node &a)
    {
        return !(*this==a);
    }
};
std::string getpath(node *root);
std::string getxdpath(node *root);
void catchToTree(node *root);
void writeToFile(node *root,FILE *fp);
void readFromFile(node *root,FILE *fp);
void delall(node *root);
void print(node *root,int n=0);
#endif // TREE_H_INCLUDED
