#include <string>
#include <cstring>

#include "table.hpp"
#include "main.hpp"

name* table::look(char *p, int ins)
{
	using namespace std;
	
	int ii = 0;
	char *pp = p;
	while (*pp) ii = ii<<1 ^ *pp++;
	if (ii < 0) ii = -ii;
	ii %= size;	
	
	for (name *n = tbl[ii]; n; n=n->next)
		if (strcmp(p, n->string) == 0) return n;
	if (ins == 0) error("Name not found");
	name *nn = new name();
	nn->string = new char[strlen(p)+1];
	strcpy(nn->string, p);
	nn->value = 1;
	nn->next = tbl[ii];
	tbl[ii] = nn;
	return nn;
}

name* table::insert(char* s)
{
	return look(s, 1);
}

void table::remove_name(const char *p)
{
        int ii = 0;
        const char *pp = p;
        while (*pp) ii = ii << 1 ^ *pp++;
        if (ii < 0)
                ii = -ii;
        ii %= size;
        for (name *n=tbl[ii]; n; n=n->next) {
                if (strcmp(p,n->string) == 0) {
                        for (int i = 0; i < size; i++) {
                                if (tbl[i] == n)
                                        tbl[i] = n->next;                                                                        
                        }
                        delete[] n->string;
                        if (n->isUserDef)
                                delete n->UserFunc;
                        delete n;
                        return;
                }
        }
}

table::table(int sz)
{
	if (sz < 0) error("Less zero teble size");
	tbl = new name*[size = sz];
	for (int i = 0; i < sz; i++)
		tbl[i] = 0;
}

table::~table()
{
	for (int i = 0; i < size; i++) {
		name *nx;
		for (name *n = tbl[i]; n; n=nx) {
			nx = n->next;
			delete n->string;
			delete n;
		}
	}
}