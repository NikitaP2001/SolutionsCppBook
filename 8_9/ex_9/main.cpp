#include <iostream>
#include <vector>
#include <map>

struct node {
	node() : count(0), top(NULL) {}
	~node()
	{ 
		if (top != NULL)
			delete top;
	}
	union {
		int count;
		int suc;
	};
	union {			
		node *top;
		node *next;
	};
};

void print_map(std::map<int, node> &nodes, int n)
{		
	bool not_empty = true;
	for (int cl = 0; not_empty; cl++) {
		not_empty = false;
		for (int i = 1; i <= n; i++) {
			node *F = &nodes[i];
			for (int l = 0; l < cl && F != NULL; l++)
				F = F->next;						
			if (F != NULL) {
				not_empty = true;
				std::cout << F->count << " ";
			} else
				std::cout << "  ";
		}
		std::cout << std::endl;
	}
}

std::vector<int> top_sort(int n,
const std::vector<std::vector<int>> &relations)
{	
	int N = n;	
	std::vector<int> sorted;
	std::map<int, node> nodes;
#ifdef DEBUG	
	std::cout << "Map on start" << std::endl;
	print_map(nodes, n);
#endif
	
	for (auto &rel : relations) {
		int j = rel[0], k = rel[1];
		nodes[k].count += 1;
		node *P = new node;
		P->suc = k;
		P->next = nodes[j].top;
		nodes[j].top = P;
	}
#ifdef DEBUG	
	std::cout << "stage 1" << std::endl;
	print_map(nodes, n);
#endif
	
	int R = 0;
	for (int i = 1; i <= n; i++) {
		if (nodes[i].count == 0) {
			nodes[R].count = i;
			R = i;
		}
	}
	int F = nodes[0].count;		
#ifdef DEBUG	
	std::cout << "stage 3" << std::endl;
	print_map(nodes, n);
#endif
	
	while (F != 0) {
		sorted.push_back(F);
		N -= 1;
		node *P = nodes[F].top;
		while (P != NULL) {			
			nodes[P->suc].count -= 1;
			if (nodes[P->suc].count == 0) {
				nodes[R].count = P->suc;
				R = P->suc;			
			}			
			P = P->next;
		}
		F = nodes[F].count;
		print_map(nodes, n);
	}
	if (N != 0)
		throw std::logic_error("loop detected");
	
	return sorted;
}

int main()
{
	int n;
	std::cin >> n >> n;	// count of objs also max obj
	std::vector<std::vector<int>> relations;
	for (int j, k; std::cin >> j >> k; ) {				
		std::vector<int> relation;
		relation.push_back(j);
		relation.push_back(k);		
		relations.push_back(relation);
	}
	
	std::vector<int> sorted;
	try {
		sorted = top_sort(n, relations);
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		exit(1);
	}
	
	for (int elem : sorted) {
		std::cout << elem << " ";
	}
	
	exit(0);
}