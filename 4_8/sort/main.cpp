#include <iostream>
#include <cstring>

typedef int (*CFT) (const void*, const void *);

class heap {
	
public:

	heap(void *pbase, unsigned base_elem_count, unsigned sz, CFT cmp);		
				
	void heapsort();
	
private:

	void *elements;
	CFT cmp;
	unsigned heap_size;
	unsigned element_size;
	
	void swap(unsigned elem_1, unsigned elem_2);
	
	inline unsigned left(unsigned i);
	
	inline unsigned right(unsigned i);
	
	inline int compare(unsigned elem_1, unsigned elem_2);	
	
	void max_heapify(unsigned i);
	
	void build_max_heap();
	
};

heap::heap(void *pbase, unsigned base_elem_count, unsigned sz, CFT cmp)
{
	this->elements = pbase;
	this->cmp = cmp;
	this->heap_size = base_elem_count;
	this->element_size = sz;
}

inline unsigned heap::left(unsigned i) { return 2*i; }

inline unsigned heap::right(unsigned i) { return 2*i+1; }

inline heap::compare(unsigned elem_1, unsigned elem_2)
{
	return (*cmp)((char *)elements+(elem_1 - 1)*element_size, 
	(char *)elements+(elem_2 - 1)*element_size);
}

void heap::swap(unsigned elem_1, unsigned elem_2)
{
	char *p_elem_1 = (char *)elements + (elem_1-1)*element_size;
	char *p_elem_2 = (char *)elements + (elem_2-1)*element_size;
	
	for (int i = 0; i < element_size; i++) {
		char temp = p_elem_1[i];
		p_elem_1[i] = p_elem_2[i];
		p_elem_2[i] = temp;
	}
}

void heap::max_heapify(unsigned i) {
	unsigned l = left(i);
	unsigned r = right(i);
	
	unsigned largest = (l <= heap_size && compare(l, i) > 0) ? l : i;
	
	if (r <= heap_size && compare(r, largest) > 0)
		largest = r;
	
	if (largest != i) {
		swap(i, largest);
		max_heapify(largest);
	}	
}

void heap::build_max_heap()
{
	for (unsigned i = heap_size / 2; i > 0; i--)
		max_heapify(i);	
}

void heap::heapsort()
{
	unsigned save_size = heap_size;
	build_max_heap();
	for (unsigned i = heap_size; i > 1; i--) {
		swap(1, i);
		heap_size -= 1;
		max_heapify(1);
	}
	heap_size = save_size;
}

void sort(void *base, unsigned n, unsigned int sz, CFT cmp)
{
	heap Heap(base, n, sz, cmp);
	
	Heap.heapsort();	
}

struct user {
	const char *name;
	const char *id;
	int dept;
};
typedef user *Puser;
user heads[] = {
	"Ritchie D.M.",	"dmr", 11271,
	"Sethi R.", "ravi", 11272,
	"SZYmanski T.G.", "tgs", 11273,
	"Schryer N.L.", "nls", 11274,
	"Schryer N.L.", "nls", 11275,
	"Kernighan B.W.", "bwk", 11276
};

void print_id(Puser v, int n)
{
	for (int i = 0; i < n; i++)
		std::cout << v[i].name << '\t'
		<< v[i].id << '\t'
		<< v[i].dept << '\n';
}

int cmp1(const void *p, const void *q)
{
	return std::strcmp(Puser(p)->name, Puser(q)->name);
}

int cmp2(const void *p, const void *q)
{
	return Puser(p)->dept - Puser(q)->dept;
}

int main()
{
	sort(heads, 6, sizeof(user), cmp1);
	print_id(heads, 6);
	std::cout << std::endl;
	sort(heads, 6, sizeof(user), cmp2);
	print_id(heads, 6);
}