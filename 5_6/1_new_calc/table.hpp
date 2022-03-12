#include <string>

struct name {
	char* string;
	name* next;	
	union {
                double (*fptr)(double);
                double value;
                std::string *UserFunc;
        };			
	bool isFunc;
	bool isUserDef;
};

class table {
	name** tbl;
	int size;
public: 
	table(int sz = 15);
	~table();
	name* look(char*, int = 0);
	name* insert(char* s);
	void remove_name(const char *p);
};