//
//  SkipList.hpp
//  CS540Project2
//
//  Created by James Kizer on 4/15/13.
//  Copyright (c) 2013 James Kizer. All rights reserved.
//

#ifndef __CS540Project2__SkipList__
#define __CS540Project2__SkipList__

#include <iostream>
#include <iterator>

namespace cs540 {
 
    
    
    
    
    
    template <typename Key_T, typename Mapped_T, size_t MaxLevel> class SkipList {
    
    public:
        typedef std::pair<const Key_T, Mapped_T> ValueType;
        
        class SkipListNode {
            
        public:
            //SkipListNode() : keyVal(nullptr), prev(nullptr), next(nullptr) {}
            SkipListNode(ValueType *kv, SkipListNode *p, SkipListNode *n) : keyVal(kv), prev(p), next(n) {}
            //default copy constructor should be fine
            SkipListNode(const SkipListNode &);
            
            SkipListNode *getNext() const {return next;}
            SkipListNode *getPrev() const {return prev;}
            void setNext(SkipListNode *n) {next = n;}
            void setPrev(SkipListNode *p) {prev = p;}
            
            ValueType &getPairRef() const {return *keyVal;}
            ValueType *getPairPtr() const {return keyVal;}
            
            const ValueType &getConstPairRef() const {return const_cast<const ValueType &>(*keyVal);}
            const ValueType *getConstPairPtr() const {return const_cast<const ValueType *>(keyVal);}
           
            bool isHead() const {
                return (keyVal == &SkipList::headValue);
            }
            
            bool isTail() const {
                return (keyVal == &SkipList::tailValue);
            }
            
            const Key_T &key() const {
                return keyVal->first;
            }
            
            //returns '<' of keys based on key's '<' operator
            bool lessThan(const Key_T &k) const {
                return (key() < k);
            }
            
            //returns '<' of keys based on key's '<' operator
            bool lessThan(const SkipListNode &sln) const {
                return (key() < sln.key());
            }
            
            //returns true if the keys match based on '<' operator
            bool keyMatch(const Key_T &k) const {
                bool b1 = (key() < k);
                bool b2 = (k < key());
                return !(b1||b2);
            }
            
            //returns true if the keys match based on '<' operator
            bool keyMatch(const SkipListNode &sln) const {
                return match(sln.key());
            }
            
            
        private:
            ValueType       *keyVal;
            SkipListNode    *prev;
            SkipListNode    *next;
        };
        
    public:
        class Iterator : public std::iterator<std::bidirectional_iterator_tag, SkipListNode>
        {
        public:
            //Delete Default Constructor
            Iterator() = delete;
            
            Iterator(SkipListNode *n) : node(n) {}
            
            //Copy Constructor and Assignment Operator
            //Implicit functions should be sufficient
            Iterator(const Iterator &it) : node(it.node) {}
            Iterator& operator=(const Iterator &it) {
                if (&it == this)
                    return (*this);
                
                node = it.node;
                return *this;
            }
            
            //increment
            Iterator &operator++() {
                node = node->getNext();
                return *this;
            }
            Iterator operator++(int) {
                Iterator tmp(*this);
                operator++();
                return tmp;
            }
            
            //Decrement
            //Optional
            Iterator &operator--() {
                //only decrement if the previous is not the list head
                if ( !(node->getPrev()->isHead()) )
                    node = node->getPrev();
                return *this;
            }
            Iterator operator--(int) {
                Iterator tmp(*this);
                operator--();
                return tmp;
            }
            
            //Dereference
            ValueType &operator*() const {return node->getPairRef();}
            ValueType *operator->() const {return node->getPairPtr();}
            
            bool isTail() const {
                return node->isTail();
            }
            
            //obtain node pointer
            SkipListNode *getNodePtr() const {return node;}

            template <typename IT_T>
            bool operator==(const IT_T &it) {
                return ( getNodePtr() == it.getNodePtr());
            }
            template <typename IT_T>
            bool operator!=(const IT_T &it) {
                return !(*this == it);
            }
            
        protected:
            SkipListNode *node;
            
            
            
        };
        
        class ConstIterator : public Iterator
        {
        public:
            //Delete Default Constructor
            ConstIterator() = delete;
            
            //Copy Constructor and Assignment Operator
            //Implicit functions should be sufficient
            ConstIterator(const ConstIterator &i) : Iterator(i) {}
            ConstIterator(const Iterator &i) : Iterator(i) {}
            ConstIterator& operator=(const ConstIterator &it) {
                if (&it == this)
                    return (*this);
                
                Iterator::node = it.node;
                return *this;
            }
            
            ConstIterator(SkipListNode *n) : Iterator(n) {}
            
            //increment
            //These should inherit from Iterator
            //ConstIterator &operator++() {}
            //ConstIterator operator++(int) {}
            
            //Decrement
            //Optional
            //These should inherit from Iterator
            //            ConstIterator &operator--() {}
            //            ConstIterator operator--(int) {}
            
            //Dereference
            //Able to const_cast Iterator base type operators?
            const ValueType &operator*() const {
                return Iterator::node->getConstPairRef();
            }
            const ValueType *operator->() const {
                return Iterator::node->getConstPairPtr();
            }
        };
        
    public:
        //Constructors and Assignment
        SkipList() : head(&headValue, &head, &tail), tail(&tailValue, &head, &tail), numEntries(0) {}
        
        SkipList(const SkipList &sl) {
            //reset state of current SkipList
            clear();
            
            //insert new entries from sl
            insert(sl.begin(), sl.end());
            
            assert(*this == sl);
        }
        
        //assignment must delete all dynamic memory and
        //allocate all new nodes
        SkipList &operator=(const SkipList &sl) {
            
            if (&sl == this)
                return (*this);
            
            //reset state of current SkipList
            clear();
            
            //insert new entries from sl
            insert(sl.begin(), sl.end());
            
            assert(*this == sl);
            
            return (*this);
        }
        
        //Destructor
        //Must Deallocate all dynamically allocated objects
        //This is done through clear()
        ~SkipList() {clear();}

        //Size
        size_t size() const { return numEntries; }
        
        //Iterators
        Iterator begin() { return Iterator(head.getNext()); }
        Iterator end() { return Iterator(&tail); }
        
        ConstIterator begin() const {
            ConstIterator tmp(head.getNext());
            return tmp;
        }
        ConstIterator end() const {
            return ConstIterator(const_cast<SkipListNode *>(&tail));
        }
        
        //Element Access
        Iterator find(const Key_T &key) {
            return Iterator(findHelper(key));
        }
        ConstIterator find(const Key_T &key) const {
            return ConstIterator(findHelper(key));
        }
        
        //Modifiers
        std::pair<Iterator, bool> insert(const ValueType &kv) {
            
            
            //first, use find to determine whether the entry exits
            Iterator it = find(kv.first);
            
            //if not the tail, an entry with a matching key was found
            //return the found iterator with a true value
            if ( !(it.isTail()) )
                return std::pair<Iterator, bool>(it, true);
            
            //If a node with the same key cannot be found on the list,
            //find the node that comes after this key
            SkipListNode *nextNode = findNextNode(kv.first);
            
            //insert a new node containing keyVal
            SkipListNode *newNode = insertHelper(kv, nextNode);
            
            numEntries++;
            
            return std::pair<Iterator, bool>(Iterator(newNode), true);
            
        }
        
        template <typename IT_T> void insert(IT_T range_beg, IT_T range_end) {
            IT_T it = range_beg;
            while (it != range_end) {
                
                //it should dereference to be ValueType
                insert(*it);
                ++it;
            }
        }
        
        void erase(Iterator pos) {
            eraseHelper(pos.getNodePtr());
            numEntries--;
        }
        
        void erase(Iterator range_beg, Iterator range_end) {
            Iterator it = range_beg;
            while (it != range_end) {
                erase(it);
                ++it;
            }
        }
        
        void clear() {
            erase(begin(), end());
            assert(numEntries == 0);
        }
        
        static ValueType headValue;
        static ValueType tailValue;
        
        
    private:
        //private helper functions
        //if the entry is not found, it returns a pointer to the
        //tail node
        SkipListNode *findHelper(const Key_T &key) {
            
            if (head.getNext()->isTail()) {
                assert(numEntries == 0);
                assert(tail.getPrev()->isHead());
                return &tail;
            }
            
            assert(numEntries > 0);
            
            SkipListNode *it = head.getNext();
            //start at element after head
            while (!(it->isTail())) {
                //if the keys match, exit loop
                if (it->keyMatch(key))
                    break;
                //otherwise, go to the next entry in the list
                it = it->getNext();
            }
            //at this point, either we found the entry
            //or it points to the tail node
            return it;
        }
        
        //returns a pointer to the existing node that should come after
        //the new node specified by key
        //This should only be called after we know the list does not contain
        //key
        SkipListNode *findNextNode(const Key_T &key) const {
            
            //start with the first entry of the list
            SkipListNode *it = head.getNext();
            
            //if head points to the tail, no entries
            if (it->isTail()) {
                assert(numEntries == 0);
                return it;
            }
            
            //loop until key in it is not less than key
            //meaning that the key in the node is greater than key
            //if we reach the tail, it should be inserted at the end of the list
            while( (it->lessThan(key)) ) {
                it = it->getNext();
                if (it->isTail())
                    break;
            }
            
            //return a pointer to the first node that is greater than key
            return it;
        }
        
        SkipListNode *insertHelper(const ValueType &keyVal, SkipListNode *nextNode) {
            
            //first, instantiate a new node
            ValueType *newKeyVal = new ValueType(keyVal);
            SkipListNode *prevNode = nextNode->getPrev();
            SkipListNode *newNode = new SkipListNode(newKeyVal, prevNode, nextNode);
            
            //setup bidirectional list pointers
            nextNode->setPrev(newNode);
            prevNode->setNext(newNode);
            
            return newNode;
            
        }
        
        void eraseHelper(SkipListNode *node) {
            //setup bidirectional list pointers
            node->getNext()->setPrev(node->getPrev());
            node->getPrev()->setNext(node->getNext());
            
            //delete pair and node
            delete node->getPairPtr();
            delete node;
        }
        
        //head does not refer to first actual entry in list
        //the entry after head is the first real value
        SkipListNode head;
        SkipListNode tail;
        size_t numEntries;
        
    };
    
    //Comparison
    template <typename Key_T, typename Mapped_T, size_t MaxLevel>
    bool operator==(const SkipList<Key_T, Mapped_T, MaxLevel> &L1, const SkipList<Key_T, Mapped_T, MaxLevel> &L2) {
        
        if (L1.size() != L2.size())
            return false;
        
        typename SkipList<Key_T, Mapped_T, MaxLevel>::Iterator it1 = L1.begin();
        typename SkipList<Key_T, Mapped_T, MaxLevel>::Iterator it2 = L2.begin();
        
        typename SkipList<Key_T, Mapped_T, MaxLevel>::Iterator itEnd = L1.end();
        
        while (it1 != itEnd) {
            if ( !((it1->first == it2->first) && (it1->second == it2->second)))
                return false;
            
            ++it1;
            ++it2;
        }
        return true;
    }
    
    template <typename Key_T, typename Mapped_T, size_t MaxLevel>
    bool operator!=(const SkipList<Key_T, Mapped_T, MaxLevel> &L1, const SkipList<Key_T, Mapped_T, MaxLevel> &L2) {
        return !(L1 == L2);
    }
    
    template <typename Key_T, typename Mapped_T, size_t MaxLevel>
    bool operator<(const SkipList<Key_T, Mapped_T, MaxLevel> &L1, const SkipList<Key_T, Mapped_T, MaxLevel> &L2) {
    
    }
    
    
    
    //static members
    template <typename Key_T, typename Mapped_T, size_t MaxLevel>
    typename SkipList<Key_T, Mapped_T, MaxLevel>::ValueType SkipList<Key_T, Mapped_T, MaxLevel>::headValue;
    template <typename Key_T, typename Mapped_T, size_t MaxLevel>
    typename SkipList<Key_T, Mapped_T, MaxLevel>::ValueType SkipList<Key_T, Mapped_T, MaxLevel>::tailValue;
    
    
    
    
    
    
    
    
}
#endif /* defined(__CS540Project2__SkipList__) */
