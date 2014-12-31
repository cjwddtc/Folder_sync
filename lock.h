#ifndef LOCK_H
#define LOCK_H

class lock
{
public:
    std::string path;
    std::unordered_multimap<std::string,unsigned char> typemap;
    lock(std::list<change> *li,std::string path);
    void writechange(change *li,std::string source);
    lock &operator<<(std::pair< std::list<change> *,std::string> pa);
};

class syncmgr : public std::unordered_set<long>
{
public:
    syncmgr(std::string path);
    syncmgr();
};
#endif // LOCK_H
