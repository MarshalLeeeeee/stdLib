#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
private:
    T **ptr;
    size_t maxSize;
    size_t currentSize;
    void resize(void)
    {
        T **tmp;
        tmp = new T *[2 * maxSize];
        size_t index;
        for(index = 0; index < currentSize; index++)
        {
            tmp[index] = new T(*(ptr[index]));
            delete ptr[index];
        }
        delete []ptr;
        ptr = tmp;
        maxSize *= 2;
    }
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		 T **itr;
		 size_t pos;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
        iterator(T **other_itr = nullptr, size_t other_pos = 0):itr(other_itr), pos(other_pos){}
        iterator &operator=(iterator &other)
        {
            itr = other.itr;
            pos = other.pos;
            return *this;
        }
		iterator operator+(const int &n) const
		{
			iterator ans;
			ans.itr = itr + n;
			ans.pos = pos + n;
			return ans;
		}
		iterator operator-(const int &n) const
		{
			iterator ans;
			ans.itr = itr - n;
			ans.pos = pos - n;
			return ans;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invalid_iterator.
		int operator-(const iterator &rhs) const
		{
			int ans = itr - rhs.itr;
			if(ans != pos - rhs.pos) throw invalid_iterator();
			else return ans;
		}
		iterator operator+=(const int &n)
		{
			itr += n;
			pos += n;
			return *this;
		}
		iterator operator-=(const int &n)
		{
			itr -= n;
			pos -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
		    iterator ans(itr, pos);
		    itr += 1;
		    pos += 1;
		    return ans;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++()
		{
		    itr += 1;
		    pos += 1;
		    return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
		    iterator ans(itr, pos);
		    itr -= 1;
		    pos -= 1;
		    return ans;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--()
		{
		    itr += 1;
		    pos += 1;
		    return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{return **itr;}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {return ((itr == rhs.itr)?1:0);}
		bool operator==(const const_iterator &rhs) const {return ((itr == rhs.itr)?1:0);}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return ((itr == rhs.itr)?0:1);}
		bool operator!=(const const_iterator &rhs) const {return ((itr == rhs.itr)?0:1);}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {
    private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		 T *const*itr;
		 size_t pos;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
        const_iterator(T *const*other_itr = nullptr, size_t other_pos = 0){itr=other_itr; pos=other_pos;}
        const_iterator &operator=(const_iterator &other)
        {
            itr = other.itr;
            pos = other.pos;
            return *this;
        }
		const_iterator operator+(const int &n) const
		{
			const_iterator ans;
			ans.itr = itr + n;
			ans.pos = pos + n;
			return ans;
		}
		const_iterator operator-(const int &n) const
		{
			const_iterator ans;
			ans.itr = itr - n;
			ans.pos = pos - n;
			return ans;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invalid_iterator.
		int operator-(const const_iterator &rhs) const
		{
			int ans = itr - rhs.itr;
			if(ans != pos - rhs.pos) throw invalid_iterator();
			else return ans;
		}
		const_iterator operator+=(const int &n)
		{
			itr += n;
			pos += n;
			return *this;
		}
		const_iterator operator-=(const int &n)
		{
			itr -= n;
			pos -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int)
		{
		    const_iterator ans(itr, pos);
		    itr += 1;
		    pos += 1;
		    return ans;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++()
		{
		    itr += 1;
		    pos += 1;
		    return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int)
		{
		    const_iterator ans(itr, pos);
		    itr -= 1;
		    pos -= 1;
		    return ans;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--()
		{
		    itr += 1;
		    pos += 1;
		    return *this;
		}
		/**
		 * TODO *it
		 */
		const T& operator*() const{return **itr;}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {return ((itr == rhs.itr)?1:0);}
		bool operator==(const const_iterator &rhs) const {return ((itr == rhs.itr)?1:0);}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return ((itr == rhs.itr)?0:1);}
		bool operator!=(const const_iterator &rhs) const {return ((itr == rhs.itr)?0:1);}

	};
	/**
	 * TODO Constructs
	 * Atleast three: default constructor, copy constructor and a constructor for std::vector
	 */
	vector()
	{
	    maxSize = 100;
	    currentSize = 0;
	    ptr = new T*[maxSize];
	}
	vector(const vector &other)
	{
	    maxSize = other.maxSize;
	    currentSize = other.currentSize;
	    ptr = new T*[maxSize];
	    size_t index;
	    for(index = 0; index < currentSize; index++)
        {
            (ptr[index]) = new T(*(other.ptr[index]));
        }
	}
	/**
	 * TODO Destructor
	 */
	~vector()
	{
	    size_t index;
	    for(index = 0; index < currentSize; index++)
        {
            delete ptr[index];
        }
        delete []ptr;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other)
	{
	    if(this == &other) return *this;
	    else
        {
            size_t index;
            for(index = 0; index < currentSize; index++)
            {
                delete ptr[index];
            }
            delete []ptr;
            maxSize = other.maxSize;
            currentSize = other.currentSize;
            ptr = new T*[maxSize];
            for(index = 0; index < currentSize; index++)
            {
                ptr[index] = new T(*(other.ptr[index]));
            }
            return *this;
        }
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos)
	{
	    if(pos < 0 || pos >= currentSize) throw index_out_of_bound();
	    else return *ptr[pos];
	}
	const T & at(const size_t &pos) const
	{
	    if(pos < 0 || pos >= currentSize) throw index_out_of_bound();
	    else return *ptr[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos)
	{
	    if(pos < 0 || pos >= currentSize) throw index_out_of_bound();
	    else return *ptr[pos];
	}
	const T & operator[](const size_t &pos) const
	{
	    if(pos < 0 || pos >= currentSize) throw index_out_of_bound();
	    else return *ptr[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const
	{
	    if(!currentSize) throw container_is_empty();
	    else return *ptr[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const
	{
	    if(!currentSize) throw container_is_empty();
	    else return *ptr[currentSize - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {iterator ans(ptr, 0); return ans;}
	const_iterator cbegin() const {const_iterator ans(ptr, 0); return ans;}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {iterator ans(ptr + currentSize, currentSize); return ans;}
	const_iterator cend() const {const_iterator ans(ptr + currentSize, currentSize); return ans;}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {return ((!currentSize)?1:0);}
	/**
	 * returns the number of elements
	 */
	size_t size() const {return currentSize;}
	/**
	 * returns the number of elements that can be held in currently allocated storage.
	 */
	size_t capacity() const {return maxSize;}
	/**
	 * clears the contents
	 */
	void clear()
	{
	    size_t index;
	    for(index = 0; index < currentSize; index++)
        {
            delete ptr[index];
        }
	    currentSize = 0;
    }
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value)
	{
	    if(currentSize == maxSize)resize();
        iterator itr = end();
        ptr[currentSize] = new T(value);
        for(;itr != pos; itr--)
        {
            *itr = *(itr-1);
        }
        *pos = value;
        currentSize++;
        return pos;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value is true
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value)
	{
	    if(ind < 0 || ind > currentSize) throw index_out_of_bound();
	    else
        {
            if(currentSize == maxSize) resize();
            if(ind == currentSize)
            {
                ptr[currentSize] = new T(value);
                currentSize++;
            }
            else
            {
                size_t index;
                ptr[currentSize] = new T(value);
                for(index = currentSize; index != ind; index--)
                {
                    *ptr[index] = *ptr[index-1];
                }
                *ptr[ind] = value;
                currentSize++;
            }
            iterator ans(ptr + ind, ind);
            return ans;
        }
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos)
	{
	    if(pos == end() - 1)
        {
            delete ptr[--currentSize];
            return end()-1;
        }
        else
        {
            iterator itr;
            iterator itre = end() - 1;
            for(itr = pos; itr != itre; itr++)
            {
                *itr = *(itr+1);
            }
            delete ptr[--currentSize];
            return pos;
        }
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind)
	{
	    if(ind < 0 || ind >= currentSize) throw index_out_of_bound();
	    else
	    {
	        if(ind == currentSize - 1)
            {
                delete ptr[--currentSize];
                return end() - 1;
            }
            else
            {
                size_t index;
                for(index = ind; index != currentSize - 1; index++)
                {
                    *ptr[index] = *ptr[index+1];
                }
                iterator ans(ptr+ind, ind);
                delete ptr[--currentSize];
                return ans;
            }
	    }
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value)
	{
	    if(currentSize == maxSize) resize();
	    ptr[currentSize++] = new T(value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back()
	{
	    if(!currentSize) throw container_is_empty();
	    else delete ptr[--currentSize];
	}
};


}

#endif
