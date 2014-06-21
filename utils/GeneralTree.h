#include <iostream>
#include <vector>

template <class T> class GeneralTree
{
private:
	struct node_GeneralTree {
		T info;
		std::vector<node_GeneralTree*> next;
	};
	node_GeneralTree* first_node;

	
	static node_GeneralTree* copy_node_GeneralTree(node_GeneralTree* m)
	/* Pre: True */
	/* Post: if m==NULL -> n=Null; else -> returns a copi of m*/
	{
		node_GeneralTree* n;
		if (m==NULL) n=NULL;
		else {
			n = new node_GeneralTree;
			n->info = m->info;
			int N = m->next.size();
			n->next = std::vector<node_GeneralTree*>(N);
			for (int i=0; i<N; ++i)
				n->next[i] = copy_node_GeneralTree(m->next[i]);
			}
		return n;
	}

	static void  delete_node_GeneralTree(node_GeneralTree* m)
	/* Pre: True */
	/* Post: if m==NULL -> ; else -> deletes all nodes of m */
	{
		if (m != NULL) {
			int N = m->next.size();
			for (int i=0; i<N; ++i)
				delete_node_GeneralTree(m->next[i]);
			delete m;
		}
	}



public:

	GeneralTree() {
	/* Pre: True */
	/* Post: p.i. is an empty GeneralTree */
		first_node= NULL;
	}

	GeneralTree(const T &x) {
	/* Pre: True */
	/* Post: p.i. is an GeneralTree with root x and no sones */
		first_node= new node_GeneralTree;
		first_node->info = x;
	}

	GeneralTree(const GeneralTree& original) {
	/* Pre: True */
	/* Post: p.i. is a copy of original */
		first_node = copy_node_GeneralTree(original.first_node);
	}

	~GeneralTree() {
	/* Pre: True */
	/* Post: deletes the tree */
		delete_node_GeneralTree(first_node);
	}

	GeneralTree& operator=(const GeneralTree& original) {
	/* Pre: True */
	/* Post: p.i. is a copy of original */
		if (this != &original) {
			delete_node_GeneralTree(first_node);
			first_node = copy_node_GeneralTree(original.first_node);
		}
		return *this;
	}
	void tree_empty() {
	/* Pre: True */
	/* Post: p.i. is an empty GeneralTree */
		delete_node_GeneralTree(first_node);
		first_node= NULL;
	}
	void plant(const T &x) {
	/* Pre: p.i. is empty*/
	/* Post: p.i. root = x and has 0 sons  */
		first_node= new node_GeneralTree;
		first_node->info= x;
	}

	void plant(const T &x, std::vector<GeneralTree> &v) {
	/* Pre: p.i. is empty and has 0 sons*/
	/* Post: p.i. root = x soons = v, v = vector of NULL GeneralTrees */
		node_GeneralTree* aux= new node_GeneralTree;
		aux->info= x;
		int n = v.size();
		aux->next = std::vector<node_GeneralTree*>(n);
		for (int i=0; i<n; ++i) {
			aux->next[i] = v[i].first_node;
			v[i].first_node = NULL;
		}
		first_node= aux;
	}

	void plant_values(const T &x, std::vector<T> &v) {
	/* Pre: p.i. is empty and has 0 sons*/
	/* Post: p.i. root = x soons = v */
		node_GeneralTree* aux = new node_GeneralTree;
		aux->info= x;
		int n = v.size();
		aux->next = std::vector<node_GeneralTree*>(n);
		for (int i=0; i<n; ++i) {
			node_GeneralTree* aux_node= new node_GeneralTree;
			aux_node->info= v[i];
			aux->next[i] = aux_node;
		}
		first_node= aux;
	}

	void add_son(const GeneralTree &a) {
	/* Pre: p.i. has no empty sons */
	/* Post: p.i. has a copy of a as a new son (last)*/
		(first_node->next).push_back(copy_node_GeneralTree(a.first_node));
	}
	void add_son(const GeneralTree &a, int i) {
	/* Pre: p.i. is empty, a is not empty, i is between 1 and the number of sons of a */
	/* Post: p.i is a copy of the n-son of a */
		first_node = copy_node_GeneralTree((a.first_node)->next[i-1]);
	}

	void add_sons(const std::vector<T> &v) {
	/* Pre: p.i. has 0 sons*/
	/* Post: p.i. sons = v */
		node_GeneralTree* aux= new node_GeneralTree;
		aux->info= first_node->info;
		int n = v.size();
		aux->next = std::vector<node_GeneralTree*>(n);
		for (int i=0; i<n; ++i) {
			node_GeneralTree* aux_node= new node_GeneralTree;
			aux_node->info= v[i];
			aux->next[i] = aux_node;
		}
		first_node= aux;
	}

	void sons(vector<GeneralTree> &v) {
	/* Pre: p.i. is not empty, v is an empty vector */
	/* Post: v has a copy of the sons of p.i. */
		node_GeneralTree* aux= first_node;
		int n = aux->next.size();
		v = std::vector<GeneralTree> (n);
		for (int i=0; i<n; ++i)
			v[i].first_node = copy_node_GeneralTree(aux->next[i]);
		delete aux;
	}

	T root() const {
	/* Pre: p.i. is not empty */
	/* Post: returns p.i.'s root */
		return first_node->info;
	}
	bool is_empty() const {
	/* Pre: True */
	/* Post: returns if p.i. is empty */
		return (first_node==NULL);
	}

	int num_sons() const {
	/* Pre: p.i. is not empty */
	/* Post: returns number of sons of p.i. */
		return (first_node->next).size();
	}

	void print() {
		if (not is_empty()){
			std::cout << root() << endl;
			std::vector<GeneralTree<T> > v;
			sons(v);
			for (int i=0; i<v.size();i++)
				v[i].print();
		} else std::cout << "Empty tree" << endl;
	}
};

