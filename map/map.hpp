/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include <string>
#include "utility.hpp"
#include "exceptions.hpp"
#define RED 0
#define BLACK 1
#define LEFT 0
#define RIGHT 1

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;

private:
	struct Node
	{
		value_type data;
		bool color;
		Node *left, *right, *parent;
		Node(const value_type &other) :data(other), left(NULL), right(NULL), parent(NULL), color(RED){}
	};
	Node *root;
	Node *end_node;
	size_t node_size;
	Compare cmp;

	Node *copy_node(Node *&node, Node *parent_node, Node *other_node)
	{
		if (!other_node) {node = NULL; return NULL;}
		node = new Node(other_node->data);
		node->parent = parent_node;
		node->color = other_node->color;
		node->left = copy_node(node->left, node, other_node->left);
		node->right = copy_node(node->right, node, other_node->right);
		return node;
	}

	void destroy_node(Node *&node)
	{
		if (!node) return;
		destroy_node(node->left);
		destroy_node(node->right);
		delete node;
		node = NULL;
		node_size--;
	}

	Node *find_node(const Key &key, Node *node)
	{
		if (!node) return NULL;
		if (!cmp(node->data.first, key) && !cmp(key, node->data.first))
            return node;
		else
		{
			if (!cmp(key, node->data.first))
				return find_node(key, node->right);
			else
				return find_node(key, node->left);
		}
	}

	const Node *find_node(const Key &key, Node *node) const
	{
		if (!node) return NULL;
		if (!cmp(node->data.first, key) && !cmp(key, node->data.first)) return node;
		else
		{
			if (!cmp(key, node->data.first))
				return find_node(key, node->right);
			else
				return find_node(key, node->left);
		}
	}

	Node *find_insert_parent(const Key &key, Node *node)
	{
		if (cmp(node->data.first, key))
		{
			if (node->right) return find_insert_parent(key, node->right);
			else return node;
		}
		else
		{
			if (node->left) return find_insert_parent(key, node->left);
			else return node;
		}
	}

	void LLb(Node *node)
	{
		Node *tmp1, *tmp2, *tmp3, *tmp4;
		tmp1 = node; tmp2 = node->left; tmp3 = node->right; tmp4 = node->left->left;
		tmp2->parent = tmp1->parent;
		if (tmp1->parent)
		{
			if (return_identity(tmp1) == LEFT) tmp2->parent->left = tmp2;
			else tmp2->parent->right = tmp2;
		}
		else root = tmp2;
		tmp1->left = tmp2->right;
		if (tmp2->right) tmp2->right->parent = tmp1;
		tmp2->right = tmp1;
		tmp1->parent = tmp2;
	}

	void RRb(Node *node)
	{
		Node *tmp1, *tmp2, *tmp3, *tmp4;
		tmp1 = node; tmp2 = node->left; tmp3 = node->right; tmp4 = node->right->right;
		tmp3->parent = tmp1->parent;
		if (tmp1->parent)
		{
			if (return_identity(tmp1) == LEFT) tmp3->parent->left = tmp3;
			else tmp3->parent->right = tmp3;
		}
		else root = tmp3;
		tmp1->right = tmp3->left;
		if (tmp3->left) tmp3->left->parent = tmp1;
		tmp3->left = tmp1;
		tmp1->parent = tmp3;
	}

	void LRb(Node *node)
	{
		RRb(node->left);
		LLb(node);
	}

	void RLb(Node *node)
	{
		LLb(node->right);
		RRb(node);
	}

	void LL(Node *node)
	{
		LLb(node);
		node->color = RED;
		node->parent->color = BLACK;
	}

	void RR(Node *node)
	{
		RRb(node);
		node->color = RED;
		node->parent->color = BLACK;
	}

	void LR(Node *node)
	{
		LRb(node);
		node->color = RED;
		node->parent->color = BLACK;
	}

	void RL(Node *node)
	{
		RLb(node);
		node->color = RED;
		node->parent->color = BLACK;
	}

	void recolor(Node *node)
	{
		node->color ^= 1;
		node->left->color ^= 1;
		node->right->color ^= 1;
	}

	bool return_color(Node *node)
	{
		if (!node || node->color == BLACK) return BLACK;
		else return RED;
	}

	Node *return_brother(Node *node)
	{
		if (node->parent->left == node) return node->parent->right;
		else return node->parent->left;
	}

	bool return_identity(Node *node)
	{
		if (node->parent->left == node) return LEFT;
		else return RIGHT;
	}

	void adjust(Node *pos)
	{
		if (pos && pos->color == BLACK && pos != root && return_brother(pos)->color == BLACK)
		{
			Node *brother = return_brother(pos);
			bool r_color = pos->parent->color;
			if (return_color(brother->left) == RED && return_color(brother->right) == RED)// brother has two red sons
			{
				if (return_identity(pos) == LEFT)
				{
					brother->parent->color = BLACK;
					brother->left->color = r_color;
					RLb(pos->parent);
				}
				else
				{
					brother->parent->color = BLACK;
					brother->right->color = r_color;
					LRb(pos->parent);
				}
			}
			else if (return_color(brother->left) == RED || return_color(brother->right) == RED)// brother has one red son
			{
				if (return_identity(pos) == LEFT)
				{
					if (return_color(brother->right) == RED) { brother->color = r_color; brother->parent->color = BLACK; brother->right->color = BLACK; RRb(pos->parent); }
					else { brother->parent->color = BLACK; brother->left->color = r_color; RLb(pos->parent); }
				}
				else
				{
					if (return_color(brother->left) == RED) { brother->color = r_color; brother->parent->color = BLACK; brother->left->color = BLACK; LLb(pos->parent); }
					else { brother->parent->color = BLACK; brother->right->color = r_color; LRb(pos->parent); }
				}
			}
			else// brother has no red sons
			{
				if (pos->parent->color == RED)
				{
					brother->color = RED;
					pos->parent->color = BLACK;
				}
				else if (pos->parent == root)
				{
					brother->color = RED;
				}
				else
				{
					brother->color = RED;
					Node *tmp = pos->parent;
					while (tmp->color == BLACK && tmp != root && return_color(return_brother(tmp)) == BLACK && return_color(return_brother(tmp)->left) == BLACK && return_color(return_brother(tmp)->right) == BLACK)
					{
						return_brother(tmp)->color = RED;
						tmp = tmp->parent;
					}
					if (tmp == root);
					else if(tmp->color == RED) tmp->color = BLACK;
					else if (return_color(return_brother(tmp)) == RED)
					{
						if (return_identity(tmp) == LEFT)
						{
							RRb(tmp->parent);
							tmp->parent->parent->color = BLACK;
							tmp->parent->color = RED;
							adjust(tmp);
						}
						else
						{
							LLb(tmp->parent);
							tmp->parent->parent->color = BLACK;
							tmp->parent->color = RED;
							adjust(tmp);
						}
					}
					else
					{
						adjust(tmp);
					}
				}
			}
		}
	}
public:
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	public:
		/**
		* TODO add data members
		*   just add whatever you want.
		*/
		Node *itr;
		Node *root_itr;
		Node *end_itr;
		Node *add(void)
		{
			Node *ans;
			if(!itr) return end_itr;
			else if (itr->right)
			{
				ans = itr->right;
				while (ans->left) ans = ans->left;
				return ans;
			}
			else
			{
				if (!itr->parent) return end_itr;// no bigger one
				if (itr->parent->left == itr) return itr->parent;
				else
				{
					ans = itr;
					while (ans->parent->right == ans)
					{
						ans = ans->parent;
						if (!ans->parent) return end_itr;// no bigger one
					}
					return ans->parent;
				}
			}
		}
		Node *subtract(void)
		{
			Node *ans;
			if(!itr)
            {
                ans = root_itr;
                if(!ans) return end_itr;
                while(ans->right) ans = ans->right;
                return ans;
            }
            else if(itr == end_itr)
            {
                ans = root_itr;
                if(!ans) return end_itr;
                while(ans->right) ans = ans->right;
                return ans;
            }
			else if (itr->left)
			{
				ans = itr->left;
				while (ans->right) {ans = ans->right;}
				return ans;
			}
			else
			{
				if (!itr->parent) return NULL;// no smaller one
				if (itr->parent->right == itr) {return itr->parent;}
				else
				{
					ans = itr;
					while (ans->parent->left == ans)
					{
						ans = ans->parent;
						if (!ans->parent) return NULL;// no bigger one
					}
					return ans->parent;
				}
			}
		}
	public:
		iterator() :itr(NULL), root_itr(NULL), end_itr(NULL) {}
		iterator(const iterator &other) { itr = other.itr; root_itr = other.root_itr; end_itr = other.end_itr; }
		iterator(Node *node, Node *r, Node *e) { itr = node; root_itr = r; end_itr = e;}
		/**
		* return a new iterator which pointer n-next elements
		*   even if there are not enough elements, just return the answer.
		* as well as operator-
		*/
		/**
		* TODO iter++
		*/
		iterator operator++(int)
		{
			iterator ans(*this);
			itr = add();
			return ans;
		}
		/**
		* TODO ++iter
		*/
		iterator & operator++()
		{
			itr = add();
			return *this;
		}
		/**
		* TODO iter--
		*/
		iterator operator--(int)
		{
			iterator ans(*this);
			itr = subtract();
			return ans;
		}
		/**
		* TODO --iter
		*/
		iterator & operator--()
		{
			itr = subtract();
			return *this;
		}
		/**
		* a operator to check whether two iterators are same (pointing to the same memory).
		*/
		value_type & operator*() const { return itr->data; }
		bool operator==(const iterator &rhs) const { return (itr == rhs.itr ? 1 : 0); }
		bool operator==(const const_iterator &rhs) const { return (itr == rhs.itr ? 1 : 0); }
		/**
		* some other operator for iterator.
		*/
		bool operator!=(const iterator &rhs) const { return (itr != rhs.itr ? 1 : 0); }
		bool operator!=(const const_iterator &rhs) const { return (itr != rhs.itr ? 1 : 0); }
		/**
		*assignment function
		*/
		iterator &operator=(Node *node){ itr = node; return *this; }
		/**
		* for the support of it->first.
		* See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		*/
		value_type* operator->() const noexcept{ return &(itr->data); }
		Node *return_node(void){ return itr; }
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
	public:
		const Node *itr;
		const Node *root_itr;
		const Node *end_itr;
		const Node *add(void)
		{
			const Node *ans;
			if(!itr) return end_itr;
			else if (itr->right)
			{
				ans = itr->right;
				while (ans->left) ans = ans->left;
				return ans;
			}
			else
			{
				if (!itr->parent) return end_itr;// no bigger one
				if (itr->parent->left == itr) return itr->parent;
				else
				{
					ans = itr;
					while (ans->parent->right == ans)
					{
						ans = ans->parent;
						if (!ans->parent) return end_itr;// no bigger one
					}
					return ans->parent;
				}
			}
		}
		const Node *subtract(void)
		{
			const Node *ans;
			if(!itr)
            {
                ans = root_itr;
                if(!ans) return end_itr;
                while(ans->right) ans = ans->right;
                return ans;
            }
            else if(itr == end_itr)
            {
                ans = root_itr;
                if(!ans) return end_itr;
                while(ans->right) ans = ans->right;
                return ans;
            }
			else if (itr->left)
			{
				ans = itr->left;
				while (ans->right) ans = ans->right;
				return ans;
			}
			else
			{
				if (!itr->parent) return NULL;// no bigger one
				if (itr->parent->right == itr) return itr->parent;
				else
				{
					ans = itr;
					while (ans->parent->left == ans)
					{
						ans = ans->parent;
						if (!ans->parent) return NULL;// no bigger one
					}
					return ans->parent;
				}
			}
		}
	public:
		const_iterator() :itr(NULL){}
		const_iterator(const const_iterator &other) { itr = other.itr; root_itr = other.root_itr; end_itr = other.end_itr;}
		const_iterator(const iterator &other) { itr = other.itr; root_itr = other.root_itr; end_itr = other.end_itr; }
		const_iterator(const Node *node, const Node *r, const Node *e) { itr = node; root_itr = r; end_itr = e;}
		/**
		* return a new iterator which pointer n-next elements
		*   even if there are not enough elements, just return the answer.
		* as well as operator-
		*/
		/**
		* TODO iter++
		*/
		const_iterator operator++(int)
		{
			const_iterator ans(*this);
			itr = add();
			return ans;
		}
		/**
		* TODO ++iter
		*/
		const_iterator & operator++()
		{
			itr = add();
			return *this;
		}
		/**
		* TODO iter--
		*/
		const_iterator operator--(int)
		{
			const_iterator ans(*this);
			itr = subtract();
			return ans;
		}
		/**
		* TODO --iter
		*/
		const_iterator & operator--()
		{
			itr = subtract();
			return *this;
		}
		/**
		* a operator to check whether two iterators are same (pointing to the same memory).
		*/
		const value_type & operator*() const { return itr->data; }
		bool operator==(const iterator &rhs) const { return (itr == rhs.itr ? 1 : 0); }
		bool operator==(const const_iterator &rhs) const { return (itr == rhs.itr ? 1 : 0); }
		/**
		* some other operator for iterator.
		*/
		bool operator!=(const iterator &rhs) const { return (itr != rhs.itr ? 1 : 0); }
		bool operator!=(const const_iterator &rhs) const { return (itr != rhs.itr ? 1 : 0); }
		/**
		*assignment function
		*/
		const_iterator &operator=(Node *node){ itr = node; return *this; }
		/**
		* for the support of it->first.
		* See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		*/
		const value_type* operator->() const noexcept{ return &(itr->data); }
		const Node *return_node(void){ return itr; }
	};
	/**
	 * TODO two constructors
	 */
	map() :root(NULL), end_node(NULL), node_size(0){}
	map(const map &other)
	{
	    copy_node(root, NULL, other.root); node_size = other.node_size;
	    if(root) end_node = new Node(root->data);
	    else end_node = NULL;
    }
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other)
	{
		if (this == &other) return *this;
		if(root) delete end_node;
		destroy_node(root);
		copy_node(root, NULL, other.root);
		node_size = other.node_size;
		if(root) end_node = new Node(root->data);
		else end_node = NULL;
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map()
	{
	    if(root) delete end_node;
	    destroy_node(root);
    }
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key)
	{
		Node *target = find_node(key, root);
		if (!target) throw index_out_of_bound();
		return target->data.second;
	}
	const T & at(const Key &key) const
	{
		const Node *target = find_node(key, root);
		if (!target) throw index_out_of_bound();
		return target->data.second;
	}
	/**
	 * TODO
	 * access specified element
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key)
	{
		Node *target = find_node(key, root);
		if (!target)
		{
			T t;
			value_type value(key, t);
			pair<iterator, bool> ans = insert(value);
			return ans.first->second;
		}
		else return target->data.second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const
	{
		const Node *target = find_node(key, root);
		if (!target) throw index_out_of_bound();
		else return target->data.second;
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin()
	{
		Node *target = root;
		if (!target)
		{
			iterator ans(end_node, root, end_node);
			return ans;
		}
		else
		{
			while (target->left) target = target->left;
			iterator ans(target, root, end_node);
			return ans;
		}
	}
	const_iterator cbegin() const
	{
		Node *target = root;
		if (!target)
		{
			const_iterator ans(target, root, end_node);
			return ans;
		}
		else
		{
			while (target->left) target = target->left;
			const_iterator ans(target, root, end_node);
			return ans;
		}
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end()
	{
		iterator ans(end_node, root, end_node);
		return ans;
	}
	const_iterator cend() const
	{
		const_iterator ans(end_node, root, end_node);
		return ans;
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const { return (node_size == 0);}
	/**
	 * returns the number of elements.
	 */
	size_t size() const { return node_size; }
	/**
	 * clears the contents
	 */
	void clear()
	{
	    if(root) {delete end_node; end_node = NULL;}
		destroy_node(root);
		root = NULL;
		node_size = 0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion),
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value)
	{
		pair<iterator, bool> ans;
		if (!root)
		{
			Node *new_node = new Node(value);
			root = new_node;
			ans.first = root;
			ans.second = true;
			node_size++;
			end_node = new Node(root->data);
		}
		else
		{
			Node *target = find_node(value.first, root);
			if (target)
            {
                ans.first = target;
                ans.second = false;
            }
			else
			{
				Node *father = find_insert_parent(value.first, root);
				Node *new_node = new Node(value);
				new_node->parent = father;
				ans.first = new_node;
				ans.second = true;
				node_size++;
				if (cmp(father->data.first, new_node->data.first)) father->right = new_node;
				else father->left = new_node;
				if (return_color(father) == RED)
				{
					if (return_color(return_brother(father)) == BLACK)
					{
						if (return_identity(father) == LEFT && return_identity(new_node) == LEFT) LL(father->parent);
						else if (return_identity(father) == LEFT && return_identity(new_node) == RIGHT) LR(father->parent);
						else if (return_identity(father) == RIGHT && return_identity(new_node) == LEFT) RL(father->parent);
						else RR(father->parent);
					}
					else
					{
						recolor(father->parent);
						Node *check_node = father->parent;
						while (check_node->parent && check_node->parent->color == RED)
						{
							if (return_brother(check_node->parent)->color == RED)
							{
								recolor(check_node->parent->parent);
								check_node = check_node->parent->parent;
							}
							else
							{
								if (return_identity(check_node->parent) == LEFT && return_identity(check_node) == LEFT) LL(check_node->parent->parent);
								else if (return_identity(check_node->parent) == LEFT && return_identity(check_node) == RIGHT) LR(check_node->parent->parent);
								else if (return_identity(check_node->parent) == RIGHT && return_identity(check_node) == LEFT) RL(check_node->parent->parent);
								else RR(check_node->parent->parent);
								break;
							}
						}
					}
				}
			}
		}
		root->color = BLACK;
		return ans;
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos)
	{
		if (!pos.return_node() || pos.return_node() == end_node || pos.root_itr != root) throw index_out_of_bound();
		else
		{
			node_size--;
			Node *target = pos.return_node();
			if (target->left && target->right)
			{
				iterator tmp = --pos;
				Node *change = tmp.return_node();
				Node *change_parent = change->parent;// change->parent cannot be NULL
				Node *change_left = change->left;
				Node *change_right = change->right;
				bool change_color = change->color;

				Node *new_node = new Node(*tmp);
				if(return_identity(change) == LEFT) change_parent->left = new_node;
				else change_parent->right = new_node;
				new_node->color = change_color;
				new_node->left = change_left;if(new_node->left) new_node->left->parent = new_node;
				new_node->right = change_right;if(new_node->right) new_node->right->parent = new_node;
				new_node->parent = change_parent;

				if(target->parent)
                {
                    if(return_identity(target) == LEFT) target->parent->left = change;
                    else target->parent->right = change;
                }
                else root = change;
                change->left = target->left;if(change->left) change->left->parent = change;
                change->right = target->right;if(change->right) change->right->parent = change;
                change->color = target->color;
                change->parent = target->parent;

                delete target;
                target = new_node;
			}
			if (!target->left && !target->right)
			{
				if (target->color == RED)
				{
					if (return_identity(target) == LEFT) target->parent->left = NULL;
					else target->parent->right = NULL;
					delete target;
				}
				else if (target == root)
				{
					delete root;
					root = NULL;
					delete end_node;
					end_node = NULL;
					return ;
				}
				else
				{
					Node *brother = return_brother(target);
					bool r_color = target->parent->color;
					if (return_color(brother) == BLACK)// brother node is black
					{
						adjust(target);
						if (return_identity(target) == LEFT) target->parent->left = NULL;
						else target->parent->right = NULL;
						delete target;
					}
					else
					{
						if (return_identity(target) == LEFT)
						{
							target->parent->color = RED;
							brother->color = BLACK;
							RRb(target->parent);
							adjust(target);
							if (return_identity(target) == LEFT) target->parent->left = NULL;
							else target->parent->right = NULL;
							delete target;
						}
						else
						{
							target->parent->color = RED;
							brother->color = BLACK;
							LLb(target->parent);
							adjust(target);
							if (return_identity(target) == LEFT) target->parent->left = NULL;
							else target->parent->right = NULL;
							delete target;
						}
					}
				}
			}
			else
			{
                if(target->left)
                {
                    if(target == root) {root = target->left;target->left->parent = NULL;delete target;}
                    else
                    {
                        Node *tmp = target->left;
                        if(return_identity(target) == LEFT) {target->parent->left = target->left; target->left->parent = target->parent; target->left->color = BLACK; delete target;}
                        else {target->parent->right = target->left; target->left->parent = target->parent; target->left->color = BLACK; delete target;}
                    }
                }
                else
                {
                    if(target == root) {root = target->right;target->right->parent = NULL;delete target;}
                    else
                    {
                        Node *tmp = target->right;
                        if(return_identity(target) == LEFT) {target->parent->left = target->right; target->right->parent = target->parent; target->right->color = BLACK; delete target;}
                        else {target->parent->right = target->right; target->right->parent = target->parent; target->right->color = BLACK; delete target;}
                    }
                }
            }
		}
		root->color = BLACK;
	}
	/**
	 * Returns the number of elements with key
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const
	{
		const Node *target = find_node(key, root);
		if (target) return 1;
		else return 0;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key)
	{
		Node *target = find_node(key, root);
		if(target) {iterator ans(target, root, end_node);return ans;}
		else {iterator ans(end_node, root, end_node);return ans;}
	}
	const_iterator find(const Key &key) const
	{
		const Node *target = find_node(key, root);
		if(target) {const_iterator ans(target, root, end_node);return ans;}
		else {const_iterator ans(end_node, root, end_node);return ans;}
	}
	void mid_Order(Node *node)
	{
	    if(node)
        {
            if(node->left) mid_Order(node->left);
            std::cout << node->data.first.val << ' ' << node->data.second << ' ' << node->color << std::endl;
            if(node->right) mid_Order(node->right);
        }
	}
	void mid(void){mid_Order(root);}
	Node *return_root(void){return root;}
};

}

#endif
