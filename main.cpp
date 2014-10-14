#include<iostream>
#include "tree.h"

using namespace std;
int main()
{
    char a[1024];
    gets(a);
    node root(a);
    catchToTree(&root);
    root.value.sonlist->sort([](node *a,node *b){return a->name.compare(a->name);});
    print(&root);
    return 0;
}
