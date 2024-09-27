#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>

#include <iostream>
#include <string>
#include <utility>
#include <optional>

/* 
 * Iterable binary search tree based on doubly-linked list	
*/
template<class K, class V>  
class SearchingTree 
{
private:
	struct ListNode 
	{
		std::optional<std::pair<K, V>> element;
		ListNode* prev;
		ListNode* next;

		ListNode() : element(), prev(nullptr), next(nullptr) {}
		ListNode(K k, V v): element(std::pair<K, V>(k, v)), prev(nullptr), next(nullptr) {}
	};

	struct TreeNode 
	{
		ListNode* list_node;
		TreeNode* right;
		TreeNode* left;

		TreeNode() : list_node(nullptr), right(nullptr), left(nullptr) {};
	};

	size_t size_;

	// List node, that points to the first element
	ListNode* begin_;

	// Utility list node, that points to the element after the last element
	// begin == end means empty containers
	ListNode* end_;

	TreeNode* root_;

	void free_node(TreeNode* node);
	TreeNode* find_min_right_parent(TreeNode* subtree_root);

	
public:
	SearchingTree();
	SearchingTree(const SearchingTree &other);
	SearchingTree(SearchingTree &&other);
	~SearchingTree();

	class iterator 
	{
	private:
		friend class SearchingTree;
		ListNode* node_;

		// Utility pointers for iterating in specific range
		ListNode* begin_;
		ListNode* end_;

        iterator(ListNode *node,
                 ListNode *begin, ListNode *end)
            : node_(node), begin_(begin),
              end_(end) { }
	public:
		iterator() : node_(nullptr), begin_(nullptr), end_(nullptr) {}
		iterator(const iterator &other) = default; // copy constructor
		iterator(iterator &&other) = default;      // move constructor
		iterator operator ++();          // iterate forward
		iterator operator --();		     // iterate backward
		bool   	 operator== (const iterator &other); // compare two iterators
		bool 	 operator!= (const iterator &other); // compare two iterators
		std::pair<K, V>& operator* (); // dereference
		std::pair<K, V>* operator-> (); // dereference
	};

	class Range 
	{
	private:
		friend class SearchingTree;
		iterator begin_;
		iterator end_;

		Range() = delete;
		Range(
			iterator begin,
			iterator end) : 
		begin_(begin), end_(end) {
		}

	public:
		iterator begin();
		iterator end();
	};


	iterator 
	begin();

	iterator 
	end();

	iterator 
	begin() const;

	iterator 
	end() const;

    void
    insert(const K &key, const V &value);

    void 
    erase(const K &key);
    
    iterator
    find(const K &key) const;

    Range 
    range(const K &min, const K& max) const;

    V&
    at(const K &key);

    V& 
    operator[] (const K &key);
};

// ===============================
// ===== Tree Implementation =====
// ===============================

template<class K, class V>
SearchingTree<K, V>::SearchingTree() : size_(0) {
	root_ = new TreeNode();
	end_ = new ListNode();
	begin_ = end_;

	root_->list_node = begin_;
}

template<class K, class V>
void
SearchingTree<K, V>::free_node(TreeNode* node) {
	if (!node)
		return;

	if (node->right)
		free_node(node->right);

	if (node->left)
		free_node(node->left);

	delete node->list_node;
	delete node;
}

template<class K, class V>
SearchingTree<K, V>::~SearchingTree() {
	free_node(root_);
}

template<class K, class V>
SearchingTree<K, V>::iterator
SearchingTree<K, V>::begin() {
	return iterator(begin_, begin_, end_);
}

template<class K, class V>
SearchingTree<K, V>::iterator
SearchingTree<K, V>::end() {
	return iterator(end_, begin_, end_);
}

template<class K, class V>
SearchingTree<K, V>::iterator
SearchingTree<K, V>::begin() const {
	return iterator(begin_, begin_, end_);
}

template<class K, class V>
SearchingTree<K, V>::iterator
SearchingTree<K, V>::end() const {
	return iterator(end_, begin_, end_);
}

template<class K, class V>
SearchingTree<K, V>::iterator
SearchingTree<K, V>::find(const K &key) const {
	TreeNode* dummy = root_;
	while (dummy) {
		if (key == dummy->list_node->element.value().first)
			return iterator(dummy->list_node, begin_, end_);

		if (key < dummy->list_node->element.value().first){
			dummy = dummy->left;
			continue;
		}

		if (dummy->list_node->element.value().first < key){
			dummy = dummy->right;
			continue;
		}

	}

	return iterator(end_, begin_, end_);
}

template<class K, class V>
SearchingTree<K, V>::Range
SearchingTree<K, V>::range(const K &min, const K& max) const {
	if (max < min)
		return Range(iterator(end_, begin_, end_), iterator(end_, begin_, end_));
	if (!size_)
		return Range(iterator(end_, begin_, end_), iterator(end_, begin_, end_));

	TreeNode* low = root_;

	// find lower bound
	while (low) {
		if (low->list_node->element.value().first == min)
			break;

		if (min < low->list_node->element.value().first) {
			if (!low->left)
				break;
			low = low->left;
			continue;
		}

		if (!low->right) {
			low = nullptr;
			break;
		}

		low = low->right;
	}

	TreeNode* up = root_;

	// find upper bound
	while (up) {
		if (up->list_node->element.value().first == max)
			break;

		if (up->list_node->element.value().first < max) {
			if (!up->right)
				break;
			up = up->right;
			continue;
		}

		if (!up->left) {
			up = nullptr;
			break;
		}

		up = up->left;
	}

	if (!up || !low)
		return Range(iterator(end_, begin_, end_), iterator(end_, begin_, end_));

	iterator end = iterator(up->list_node, low->list_node, up->list_node->next);
	++end;

	return Range(iterator(low->list_node, low->list_node, up->list_node->next), end);
}


template<class K, class V>
void
SearchingTree<K, V>::insert(const K &key, const V &value) {
	// if empty, assign root
	if (size_ == 0) {
		root_->list_node = new ListNode(key, value);

		begin_ = root_->list_node;

		root_->list_node->next = end_;
		end_->prev = root_->list_node;
		size_++;
		return;
	}

	TreeNode* dummy = root_;

	while (dummy) {
		if (key == dummy->list_node->element.value().first)
			return;

		if (key < dummy->list_node->element.value().first) {
			if (dummy->left) {
				dummy = dummy->left;
				continue;
			}
			// Allocate new nodes
			dummy->left = new TreeNode();
			dummy->left->list_node = new ListNode(key, value);
			ListNode* current = dummy->left->list_node;

			// Link next pointers in the list
			current->next = dummy->list_node;
			current->prev = dummy->list_node->prev;
			dummy->list_node->prev = current;

			if (current->prev)
				current->prev->next = current;

			if (dummy->list_node == begin_)
				begin_ = current;

			size_++;
			return;
		}
		else {
			if (dummy->right) {
				dummy = dummy->right;
				continue;
			}
			// Allocate new nodes
			dummy->right = new TreeNode();
			dummy->right->list_node = new ListNode(key, value);
			ListNode* current = dummy->right->list_node;

			// Link next pointers in the list
			current->prev = dummy->list_node;
			current->next = dummy->list_node->next;
			dummy->list_node->next = current;

			if (current->next)
				current->next->prev = current;

			size_++;
			return;
		}
	}
}

template<class K, class V>
void
SearchingTree<K, V>::erase(const K &key) {
	if (size_ == 0)
		return;

	if (size_ == 1) {
		delete root_->list_node;
		begin_ = end_;
		size_--;
		root_->list_node = begin_;
		return;
	}

	TreeNode* dummy = root_;
	TreeNode* parent = nullptr;

	while(dummy) {
		// if less, search in left subtree
		if (key < dummy->list_node->element.value().first) {
			parent = dummy;
			dummy = dummy->left;
			continue;
		}
		
		// if greater, search in right subtree
		if (dummy->list_node->element.value().first < key) {
			parent = dummy;
			dummy = dummy->right;
			continue;
		}

		// relink the list
		if (dummy->list_node->prev)
			dummy->list_node->prev->next = dummy->list_node->next;
		if (dummy->list_node->next)
			dummy->list_node->next->prev = dummy->list_node->prev;

		if (dummy->list_node == begin_)
			begin_ = dummy->list_node->next;
		delete dummy->list_node;


		// if both children are empty, just erase node
		if (!dummy->right && !dummy->left) {
			delete dummy;
			size_--;
			return;
		}

		// only right child is empty
		if (!dummy->right) {
			TreeNode* left = dummy->left;
			delete dummy;

			// replace it with left subtree
			if (!parent) {
				root_ = left;
				size_--;
				return;
			}
			
			if (parent->list_node->element.value().first > left->list_node->element.value().first)
				parent->left = left;
			else
				parent->right = left;
			size_--;
			return;
		}
		// only left child is empty
		if (!dummy->left) {
			TreeNode* right = dummy->right;
			delete dummy;

			if (!parent) {
				root_ = right;
				size_--;
				return;
			}

			// replace it with the right subtree
			if (parent->list_node->element.value().first < right->list_node->element.value().first)
				parent->right = right;
			else
				parent->left = right;
			size_--;
			return;
		}

		// both children are non-empty

		// find the parent of min element in the right subtree
		TreeNode* min_right_parent = find_min_right_parent(dummy);
		TreeNode* min_right = min_right_parent->left;

		TreeNode* right_subtree = dummy->right;
		
		min_right_parent->left = nullptr;
		min_right->right = right_subtree;
		delete dummy;

		// replace it with min_right
		if (!parent) {
			root_ = min_right;
			size_--;
			return;
		}

		if (parent->list_node->element.value().first > min_right->list_node->element.value().first)
			parent->left = min_right;
		else
			parent->right = min_right;
		size_--;
		return;
	}
}
 
template<class K, class V>
SearchingTree<K, V>::TreeNode*
SearchingTree<K, V>::find_min_right_parent(TreeNode* subtree_root) {
	TreeNode* parent = subtree_root;

	assert(parent->right);

	TreeNode* dummy = subtree_root->right;

	while (dummy) {
		if (!dummy->left)
			return parent;
		parent = dummy;
		dummy = dummy->left;
	}
	return dummy;
}

// ===================================
// ===== Iterator Implementation =====
// ===================================


template<class K, class V>
SearchingTree<K, V>::iterator
SearchingTree<K, V>::iterator::operator ++() {
	assert(node_ != end_);
	assert(node_->next != nullptr);

	node_ = node_->next;
	return *this;
}

template<class K, class V>
SearchingTree<K, V>::iterator
SearchingTree<K, V>::iterator::operator --() {
	assert(node_ != begin_);
	assert(node_->prev != nullptr);
	
	node_ = node_->prev;
	return *this;
}

template<class K, class V>
bool
SearchingTree<K, V>::iterator::operator==(const SearchingTree<K, V>::iterator &other) {
	return (node_ == other.node_);
}

template<class K, class V>
bool
SearchingTree<K, V>::iterator::operator!=(const SearchingTree<K, V>::iterator &other) {
	return (node_ != other.node_);
}

template<class K, class V>
std::pair<K, V>&
SearchingTree<K, V>::iterator::operator* () {
	return node_->element.value();
}

template<class K, class V>
std::pair<K, V>*
SearchingTree<K, V>::iterator::operator-> () {
	return &node_->element.value();
}

// ================================
// ===== Range Implementation =====
// ================================

template<class K, class V>
SearchingTree<K, V>::iterator
SearchingTree<K, V>::Range::begin() {
	return begin_;
}

template<class K, class V>
SearchingTree<K, V>::iterator
SearchingTree<K, V>::Range::end() {
	return end_;
}

#define LOCAL

#ifdef LOCAL
// #define private public

int main(void) {

	auto tree = SearchingTree<int, int>();
	tree.insert(3, 3);
	tree.insert(2, 2);
	tree.insert(1, 1);
	tree.erase(3);

	for (auto &[k, v] : tree.range(-10, 10)) {
		std::cout << v << ' ';
	}
	return 0;
}

#endif