

struct dlink {
	dlink *next;
	dlink *prev;
	dlink() { next = 0; prev = 0; }
	dlink(dlink *p) { next = p; }
};

template<class T>
struct Tlink : public dlink {
	T info;
	Tlink(const T& a) : info(a) { }
};

class dlist_base {
	dlink *last;
public:
	void insert(dlink *a);
	void append(dlink *a);
	dlink *get();

	void clear() { last = 0; }
	dlist_base() { last = 0; }
	dlist_base(dlink *a) 
	{ 
		last = a->next = a;
		a->prev = a;
       	}
	friend class dlist_base_iter;
};

template<class T>
class dlist : private dlist_base {
public:
	void insert(const T& a)
	{
		dlist_base::insert(new Tlink<T>(a));
	}
	void append(const T& a)
	{
		dlist_base::append(new Tlink<T>(a));
	}
	T get()
	{
		Tlink<T>* lnk = (Tlink<T> *) dlist_base::get();
		T i = lnk->info;
		delete lnk;
		return i;
	}
};

void dlist_base::insert(dlink *a)
{
	if (last) {
		last->next->prev = a;
		a->next = last->next;
		a->prev = last;
		last->next = a;
	} else {
		last = a;
		last->next = a;
		last->prev = a;
	}
}

void dlist_base::append(dlink *a)
{
	if (last) {
		last->next->prev = a;
		a->next = last->next;
		a->prev = last;
		last->next = a;
		last = a;
	} else 
		last = a->prev = a->next = a;
}

dlist* dlist_base::get()
{
	if (last == 0)
		std::logic_error("Inposible to get from empty list");
	dlist* f = last->next;
	if (f == last)
		last = 0;
	else {
		last->next = f->next;
		f->next->prev = last;
	}
	return f;
}

main()
{
	slist<char> lst;
}
