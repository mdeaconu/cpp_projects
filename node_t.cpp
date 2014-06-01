#include <iostream>
#include <chrono>

static const int gN = 100000;

typedef struct node {
	int value;
	struct node* next;
} node_t;

static void Add(node_t*& nodes, int value)
{
	if (nodes==nullptr) {
		nodes = new node_t;
		nodes->value = value;
		nodes->next = nullptr;
	} else {
		node_t* nod = new node_t;
		nod->value = value;
		nod->next=nodes;
		nodes = nod;
	}
}

static void print(node_t*& node) 
{
	node_t* p = node;
	while (p!=nullptr) {
		std::cout << p->value << " ";
		p = p->next;
	}
	std::endl(std::cout);
}

static void reverse(node_t*& root)
{
	node_t* current = root;
	node_t* prev = nullptr;
	node_t* next = nullptr;

	while (current != nullptr) {
		next = current->next; 
		current->next = prev;  
		prev = current;
		current = next;
	}
	root = prev;
}
static void rreverse(node_t*& root) 
{
	node_t* first;
	node_t* rest;

	if (root == nullptr)
		return;  

	first = root; 
	rest  = first->next;

	if (rest == NULL)
		return;  

	rreverse(rest);
	first->next->next  = first; 
	first->next  = NULL;         

	// set the root
	root = rest; 
}

int main()
{
	node_t* NOD = nullptr;
	for (int i=0; i<gN; ++i) {
		Add(NOD, i);
	}

	print(NOD);

	reverse(NOD);
	print(NOD);

	return EXIT_SUCCESS;
}