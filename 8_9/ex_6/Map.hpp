#include <vector>

namespace tree_map {

	template <class K, class V> class Mapiter;
	template <class K, class V> class Map;

	template<class K, class V>
	class Link {
		friend class Map<K, V>;
		friend class Mapiter<K, V>;
	private:
		const K key;
		V value;
		Link *llink;
		Link *rlink;
		int B;		// mark of node balance
		Link(const K& k, const V& v) : key(k), value(v) { }
		~Link()
		{ 
			delete llink; 
			delete rlink;
		}
	};

	template<class K, class V>
	class Map {
		friend class Mapiter<K, V>;
		Link<K, V> *current;
		Link<K, V> *head;
		int sz;
		int height;
		V def_val;
		K def_key;
		void find(const K&);

		void init() 
		{ 
			head = new Link<K, V>(def_key, def_val); 
			head->rlink = head->llink = NULL;
			head->B = 0;
			height = 0;
			sz = 0;
		}

		inline Link<K, V>* &link(int a, Link<K, V> *P)
		{
			return (a == -1) ? P->llink : P->rlink;
		}
	public:
		
		Map() { init(); }
		Map(const K& k, const V& d)
			: def_key(k), def_val(d) { init(); }
		~Map() { delete head; }
		Map(const Map&);
		Map& operator=(const Map&);
		V& operator[](const K& k);
		int size() const { return sz; }
		void clear() { delete head; init(); }
		void remove(const K& k);

		Mapiter<K, V> element(const K& k)
		{
			(void)operator[](k);
			return Mapiter<K, V>(this, current);
		}
		Mapiter<K, V> first();
		Mapiter<K, V> last();
	};

	template<class K, class V>
	V& Map<K, V>::operator[](const K& k)
	{
		Link<K, V> *S, *P, *T, *Q, *R;
		if (head->rlink == 0) {
			head->rlink = new Link<K, V>(k, def_val);
			sz += 1;
			height += 1;
			current = head->rlink;
			current->rlink = current->llink = NULL;
			current->B = 0;
			return current->value;
		}
		P = S = head->rlink;
		T = head;

		// seek for a node
		for (;;) {
			if (k == P->key) {
				current = P;
				return current->value; 
			}
			if (k < P->key) {
				Q = P->llink;
				if (Q == NULL) {
					Q = new Link<K, V>(k, def_val);
					sz += 1;
					P->llink = Q;
					break;
				} 
			} else {
				Q = P->rlink;
				if (Q == NULL) {
					Q = new Link<K, V>(k, def_val);
					sz += 1;
					P->rlink = Q;
					break;
				} 
			}
			if (Q->B != 0) {
				T = P;
				S = Q;
			} 
			P = Q;
		}

		// setup new node
		Q->rlink = Q->llink = NULL;
		Q->B = 0;

		R = P = (k < S->key) ? S->llink : S->rlink;
		while (P != Q) {
			if (k < P->key) {
				P->B = -1;
				P = P->llink;
			} else {
				P->B = 1;
				P = P->rlink;
			}
		}

		int a = (k < S->key) ? -1 : 1;
		// tree became higher
		if (S->B == 0) {
			S->B = a;
			height += 1;
			current = Q;
			return current->value;
		// tree became more balanced
		} else if (S->B == -a) {
			S->B = 0;
			current = Q;
			return current->value;
		// tree stop being balanced
		} else {
			if (R->B == a) {
				P = R;
				link(a, S) = link(-a, R);
				link(-a, R) = S;
				S->B = R->B = 0;
			} else {
				P = link(-a, R);
				link(-a, R) = link(a, P);
				link(a, P) = R;
				link(a, S) = link(-a, P);
				link(-a, P) = S;

				if (P->B == a) {
					S->B = -a;
					R->B = 0;
				} else if (P->B == 0) {
					S->B = 0;
					R->B = 0;
				} else {
					S->B = 0;
					R->B = a;
				}
				P->B = 0;
			}

			if (S == T->rlink)
				T->rlink = P;
			else
				T->llink = P;
			current = Q;
			return current->value;
		}
	}

	template<class K, class V>
	Mapiter<K, V> Map<K, V>::first()
	{
		Mapiter<K, V> mp(*this);
		if (sz > 0)
			mp.current = 0;	
		return mp;
	}

	template<class K, class V>
	Mapiter<K, V> Map<K, V>::last()
	{
		Mapiter<K, V> mp(*this);
		if (sz > 0)
			mp.current = sz - 1;	
		return mp;
	}
	
	template<class K, class V>
	class Mapiter {
		friend class Map<K, V>;
		Map<K, V> *m;
		std::vector<Link<K, V> *> parray;
		int current;
		Mapiter(Map<K, V> *mm, Link<K, V> *pp);
		void tree_to_vec(Link<K, V> *node);
	public:
		Mapiter() { m = 0; current = -1; }
		Mapiter(Map<K, V> &mm);
		operator void*() 
		{ 
			if (current >= 0 && current < m->sz) {
				return parray[current];
			} else
				return NULL;
		}
		const K& key();
		V& value();
		Mapiter& operator--();
		void operator--(int);
		Mapiter& operator++();
		void operator++(int);
	};

	template<class K, class V>
	void Mapiter<K, V>::tree_to_vec(Link<K, V> *node)
	{
		if (node == NULL)
			return; 
		tree_to_vec(node->llink);
		parray.push_back(node);
		tree_to_vec(node->rlink);
	}

	template<class K, class V>
	Mapiter<K, V>::Mapiter(Map<K, V> *mm, Link<K, V> *pp)
	{
		m = mm;
		tree_to_vec(m->head->rlink);	
		current = -1;
		for (auto node : parray) {
			if (pp == node)
				break;
			current += 1;
		}
	}

	template<class K, class V>
	Mapiter<K, V>::Mapiter(Map<K, V> &mm)
	{
		m = &mm;
		tree_to_vec(m->head->rlink);
		current = -1;
	}

	template<class K, class V> V& Mapiter<K, V>::value()
	{ 
		if (current != -1)
			return parray[current]->value;
		else
			return m->def_val;
	}

	template<class K, class V>
	const K& Mapiter<K, V>::key()
	{
		if (current != -1)
			return parray[current]->key;
		else
			return m->def_key;
	}

	template<class K, class V>
	Mapiter<K, V>& Mapiter<K, V>::operator++()
	{
		if (current != -1)
			current += 1;
		if (current >= m.sz)
			current = -1;
		return *this;
	}

	template<class K, class V>
	void Mapiter<K, V>::operator++(int)
	{
		if (current != -1)
			current += 1;
		if (current >= m->sz)
			current = -1;
	}

}
