
namespace listmap {

	template<class K, class V> class Mapiter;
	template<class K, class V> class Link;

	template<class K, class V>
	class Map {
		friend class Mapiter<K, V>;
		Link<K,V> *head;
		Link<K,V> *current;
		V def_val;
		K def_key;
		int sz;
		void find(const K&); 
		void init() { sz = 0; head = 0; current = 0; }
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
	Mapiter<K, V> Map<K, V>::first()
	{
		return Mapiter<K, V>(this, head);
	}

	template<class K, class V>
	Mapiter<K, V> Map<K, V>::last()
	{
		return Mapiter<K, V>(this, head->pre);
	}

	template<class K, class V>
	V& Map<K, V>::operator[](const K &k)
	{
		if (head == 0) {
			current = head = new Link<K, V>(k, def_val);
			current->pre = current->suc = 0;
			return current->value;
		}

		Link<K, V> *p = head;
		for (;;) {
			if (p->key == k) {
				current = p;
				return current->value;
			}
			if (k < p->key) {
				current = new Link<K, V>(k, def_val);
				current->pre = p->pre;
				current->suc = p;
				if (p == head)
					head = current;
				else 
					p->pre->suc = current;
				p->pre = current;
				return current->value;
			}

			Link<K, V> *s = p->suc;
			if (s == 0) {
				current = new Link<K, V>(k, def_val);
				current->pre = p;
				current->suc = 0;
				p->suc = current;
				return current->value;
			}
			p = s;
		}
	}

	template<class K, class V>
	void Map<K, V>::remove(const K& k)
	{
		Link<K, V> *p = head;
		for (;;) {
			if (p->key == k) {
				break;
			}
			if (k < p->key) {
				return;
			}

			Link<K, V> *s = p->suc;
			if (s == 0) {
				return;	
			}
			p = s;
		}
		if (p == head)
			head = current = 0;
		if (p->pre != 0)
			p->pre->suc = p->suc;
		if (p->suc != 0)
			p->suc->pre = p->pre;
		delete p;
	}

	template<class K, class V>
	Map<K, V>::Map(const Map<K, V>& m)
	{
	}

	template<class K, class V>
	Map<K, V>& Map<K, V>::operator=(const Map<K, V>& m)
	{
		return *this;
	}

	template<class K, class V>
	class Mapiter {
		friend class Map<K, V>;
		Map<K, V> *m;
		Link<K, V> *p;
		Mapiter(Map<K, V> *mm, Link<K, V> *pp)
			: m(mm), p(pp) {}
	public:
		Mapiter() { m = 0; p = 0; }
		Mapiter(Map<K, V> &mm);
		operator void*() { return p; }
		const K& key();
		V& value();
		Mapiter& operator--();
		void operator--(int);
		Mapiter& operator++();
		void operator++(int);
	};

	template<class K, class V>
	const K& Mapiter<K, V>::key()
	{
		if (p) 
			return p->key;
		else
			return m->def_key;
	}

	template<class K, class V>
	V& Mapiter<K, V>::value()
	{
		if (p)
			return p->value;
		else
			return m->def_val;
	}

	template<class K, class V>
	Mapiter<K, V>& Mapiter<K, V>::operator--()
	{
		if (p)
			p = p->pre;
		return *this;
	}

	template<class K, class V>
	void Mapiter<K, V>::operator--(int)
	{
		if (p)
			p = p->pre;
	}

	template<class K, class V>
	Mapiter<K, V>& Mapiter<K, V>::operator++()
	{
		if (p)
			p = p->suc;
		return *this;
	}

	template<class K, class V>
	void Mapiter<K, V>::operator++(int)
	{
		if (p)
			p = p->suc;
	}

	template<class K, class V>
	Mapiter<K, V>::Mapiter(Map<K, V> &mm)
	{
		m = &mm;
		p = m->head;
	}

	template<class K, class V>
	class Link {
		friend class Map<K, V>;
		friend class Mapiter<K, V>;
	private:
		const K key;
		V value;
		Link *pre;
		Link *suc;
		Link(const K& k, const V& v) : key(k), value(v) { }
		~Link() { delete suc; }
	};

}
