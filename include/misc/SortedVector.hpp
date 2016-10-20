#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <functional>

#include "../typetraits/CallTraits.hpp"

namespace util
{
    template<typename Key,
             bool     AllowDuplicates = true,
             typename Compare         = std::less<Key>,
             typename Allocator       = std::allocator<Key>>
    class sorted_vector
    {
    public:
        typedef sorted_vector<Key,AllowDuplicates,Compare,Allocator> sorted_vector_type;

        typedef std::vector<Key,Allocator>            vector_type;
        typedef typename vector_type::allocator_type  allocator_type;
        typedef typename vector_type::size_type		  size_type;
        typedef typename vector_type::difference_type difference_type;
        typedef typename vector_type::reference       reference;
        typedef typename vector_type::const_reference const_reference;
        typedef typename vector_type::value_type      value_type;
        typedef best_param<value_type>                value_param;
        typedef Key                                   key_type;
        typedef best_param<Key>                       key_param;
        typedef Compare                               key_compare_type;
        typedef Compare                               value_compare_type;

        typedef typename vector_type::iterator			     iterator;
        typedef typename vector_type::const_iterator	     const_iterator;
        typedef typename vector_type::reverse_iterator	     reverse_iterator;
        typedef typename vector_type::const_reverse_iterator const_reverse_iterator;

        typedef std::pair<iterator, iterator>             iterator_iterator_pair_type;
        typedef std::pair<const_iterator, const_iterator> citerator_citerator_pair_type;
        typedef std::pair<iterator, bool>                 iterator_bool_pair_type;

        explicit sorted_vector(Compare const& pred = Compare(), Allocator const& al = Allocator())
            : key_compare(pred), vec(al)
        {}

        sorted_vector(std::initializer_list<value_type> const& il, Compare const& pred = Compare(), Allocator const& al = Allocator())
            : key_compare(pred), vec(al)
        {
            for (value_param x : il)
                insert(x);
        }

        template<class It>
        sorted_vector(It const begin, It const end, Compare const& pred = Compare(), Allocator const& al = Allocator())
            : key_compare(pred),vec(begin,end,al)
        { stable_sort(); }

        sorted_vector(sorted_vector_type&& x)
            : key_compare(std::forward<key_compare_type>(x.key_compare)), vec(std::forward<vector_type>(x.vec))
        {}

        sorted_vector(sorted_vector_type const& x)
            : key_compare(x.key_compare), vec(x.vec)
        {}

        ~sorted_vector()
        {}

        sorted_vector_type& operator= (std::initializer_list<value_type> const& il)
        {
            vec.clear();
            return *new (this) vector_type(il);
        }

        sorted_vector_type& operator= (sorted_vector_type&& x)
        {
            key_compare = std::forward<key_compare_type>(x.key_compare);
            vec = std::forward<vector_type>(x.vec);
            return *this;
        }

        sorted_vector_type& operator= (sorted_vector_type const& x)
        {
            key_compare = x.key_compare;
            vec = x.vec;
            return *this;
        }

        sorted_vector_type& operator= (vector_type const& x)
        {
            vec.operator=(x);
            sort();
            return *this;
        }

        inline void reserve(size_type const n)	{ vec.reserve(n); }
        inline void shrink_to_fit() { vec.shrink_to_fit(); }

        inline iterator                 begin()       { return vec.begin(); }
        inline const_iterator           begin() const { return vec.begin(); }
        inline const_iterator          cbegin() const { return vec.cbegin(); }
        inline reverse_iterator        rbegin()       { return vec.rbegin(); }
        inline const_reverse_iterator crbegin() const { return vec.crbegin(); }

        inline iterator                 end()       { return vec.end(); }
        inline const_iterator           end() const { return vec.end(); }
        inline const_iterator          cend() const { return vec.cend(); }
        inline reverse_iterator        rend()       { return vec.rend(); }
        inline const_reverse_iterator crend() const { return vec.crend(); }

        inline size_type size    () const { return vec.size(); }
        inline size_type max_size() const { return vec.max_size(); }

        inline bool empty() const              { return vec.empty(); }
        inline Allocator get_allocator() const { return vec.get_allocator(); }

        inline const_reference at(size_type const p) const { return vec.at(p); }
        inline reference       at(size_type const p)       { return vec.at(p); }

        inline const_reference operator[](size_type const p) const { return vec.operator[](p); }
        inline reference       operator[](size_type const p)       { return vec.operator[](p); }

        inline value_type*       data()       { return vec.data(); }
        inline value_type const* data() const { return vec.data(); }

        inline reference       front()       { return vec.front(); }
        inline const_reference front() const { return vec.front(); }

        inline reference       back()       { return vec.back(); }
        inline const_reference back() const { return vec.back(); }

        inline void pop_back() { vec.pop_back(); }

        inline void assign(const_iterator first, const_iterator beyond) { vec.assign(first,beyond); }
        inline void assign(size_type const n, key_param x = Key())      { vec.assign(n,x); }

        iterator_bool_pair_type insert(value_param x)
		{
            if(!AllowDuplicates)
            {
                iterator const p = lower_bound(x);
                if(p == end() || key_compare(x,*p))
                    return iterator_bool_pair_type(insertImpl(p,x),true);
                else
                    return iterator_bool_pair_type(p,false);
            }
            else
            {
                iterator const p = upper_bound(x);
                return iterator_bool_pair_type(insertImpl(p,x),true);
            }
        }

        iterator insert(iterator it, value_param x)
        {
           if(it != end())
           {
               if(!AllowDuplicates)
               {
                    if(key_compare(*it,x))
                    {
                        if((it+1) == end() || keyCompareGt(*(it+1),x))
                            return insertImpl(it+1,x);
                        else if(keyCompareGeq(*(it+1),x))
                           return end();
                    }
               }
               else
               {
                   if(keyCompareLeq(*it,x) && ((it+1) == end() || keyCompareGeq(*(it+1),x)))
                       return insertImpl(it+1,x);
               }
           }
           return insert(x).first;
        }

        template<class It> void insert(It begin, It end)
        {
            size_type const n = std::distance(begin, end);
            reserve(size() + n);
            for(; begin != end; ++begin)
                insert(*begin);
        }

        inline iterator erase(iterator const p) { return vec.erase(p); }
        inline iterator erase(iterator const begin, iterator end) { return vec.erase(begin,end); }
        size_type erase(key_param key)
        {
            iterator_iterator_pair_type const begEnd = equal_range(key);
            erase(begEnd.first,begEnd.second);
            return std::distance(begEnd.first, begEnd.second);
        }

        inline void clear() { return vec.clear(); }

        inline bool equal(sorted_vector_type const& x) const { return size() == x.size() && std::equal(begin(), end(), x.begin()); }
        inline bool lexo (sorted_vector_type const& x) const { return std::lexicographical_compare(begin(), end(), x.begin(), x.end()); }

        void swap(sorted_vector_type& x)
        {
            vec.swap(x.vec);
            std::swap(key_compare,x.key_compare);
        }

        friend void swap(sorted_vector_type& x, sorted_vector_type& y)
        {
            x.swap(y);
        }

        inline key_compare_type key_comp() const { return key_compare; }
        inline value_compare_type value_comp() const { return key_comp(); }

        iterator find(key_param k)
        {
            iterator const p = lower_bound(k);
            return (p == end() || key_compare(k, *p)) ? end() : p;
        }

        const_iterator find(key_param k) const
        {
            const_iterator const p = lower_bound(k);
            return (p==end()||key_compare(k,*p))?end():p;
        }

        size_type count(key_param k) const
        {
            citerator_citerator_pair_type const p = equal_range(k);
            return std::distance(p.first, p.second);
        }

        inline iterator       lower_bound(key_param k)       { return std::lower_bound(begin(), end(), k, key_compare); }
        inline const_iterator lower_bound(key_param k) const { return std::lower_bound(begin(), end(), k, key_compare); }

        inline iterator       upper_bound(key_param k)       { return std::upper_bound(begin(), end(), k, key_compare); }
        inline const_iterator upper_bound(key_param k) const { return std::upper_bound(begin(), end(), k, key_compare); }

        inline iterator_iterator_pair_type   equal_range(key_param k)       { return std::equal_range(begin(), end(), k, key_compare); }
        inline citerator_citerator_pair_type equal_range(key_param k) const { return std::equal_range(begin(), end(), k, key_compare); }

        inline vector_type& get_vector() { return vec; }

        void sort()
        {
            std::sort(vec.begin(), vec.end(), key_compare);
            if(!AllowDuplicates)
                vec.erase(moveDuplicates(),vec.end());
        }

        void qsort()
        {
            std::qsort(vec.begin(), vec.end(), key_compare);
            if (!AllowDuplicates)
                vec.erase(moveDuplicates(), vec.end());
        }

        void stable_sort()
        {
            std::stable_sort(vec.begin(), vec.end(), key_compare);
            if(!AllowDuplicates)
                erase(moveDuplicates(),end());
        }

        inline bool operator== (sorted_vector_type const& x) const { return equal(x); }
        inline bool operator!= (sorted_vector_type const& x) const { return !(*this == x); }
        inline bool operator<  (sorted_vector_type const& x) const { return lexo(x); }
        inline bool operator>  (sorted_vector_type const& x) const { return *this < x; }
        inline bool operator<= (sorted_vector_type const& x) const { return !(*this < x); }
        inline bool operator>= (sorted_vector_type const& x) const { return (!(x < *this)); }

    protected:
        iterator moveDuplicates()
        {
            iterator begin = vec.begin();
            iterator out   = vec.end();
            iterator end   = vec.end();

            bool bCopy_ = false;

            for(iterator previous; (previous = begin) != end && ++begin != end;)
            {
                if(key_compare(*previous, *begin))
                {
                    if(bCopy_)
                    {
                        *out = *begin;
                        ++out;
                    }
                }
                else
                {
                    if(!bCopy_)
                    {
                        out=begin;
                        bCopy_=true;
                    }
                }
            }
            return out;
        }

        inline iterator insertImpl(iterator const p, value_param x) { return vec.insert(p,x);}

        inline bool keyCompareLeq(key_param a, key_param b) { return !key_compare(a,b); }
        inline bool keyCompareGeq(key_param a, key_param b) { return !key_compare(a,b); }
        inline bool keyCompareGt (key_param a, key_param b) { return  key_compare(a,b); }

        key_compare_type key_compare;
        vector_type      vec;
    };
}
