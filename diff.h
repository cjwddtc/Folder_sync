#ifndef DIFF_H_INCLUDED
#define DIFF_H_INCLUDED
#define DELFILE 0
#define DELDIR 1
#define NEWFILE 2
#define NEWDIR 3
class change
{
public:
    unsigned char changetype;
    std::string path;
};
class conflict
{
    node *sameodd;
    node *newa;
    node *newb;
};
std::list<change> *cmp(node *newroot,node *oldroot,std::list<change> *li=0);
std::list<conflict> getconflict(std::list<change> a,std::list<change> b);
void rmdirall(std::string path);
void copyFile(std::string source,std::string des);
void copyDir(std::string source,std::string des);

#endif // DIFF_H_INCLUDED
