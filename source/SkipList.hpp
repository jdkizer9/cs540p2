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
#include <random>
#include <chrono>

namespace cs540 {
 
    
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution;
    static bool seeded = false;
    
    
    template <typename Key_T, typename Mapped_T, size_t MaxLevel> class SkipList {
    
    public:
        typedef std::pair<const Key_T, Mapped_T> ValueType;
        
        

        
        class SkipListNode {
            
        public:
            //SkipListNode() : keyVal(nullptr), prev(nullptr), next(nullptr) {}
            
            //This should only be called in the case of head and tail
            SkipListNode(ValueType *kv, SkipListNode *p, SkipListNode *n) : keyVal(kv), numLevels(MaxLevel) {
                assert(isHead() || isTail());
                for (size_t i=0; i<MaxLevel; i++) {
                    prev[i] = p;
                    next[i] = n;
                }
                
            }
            
            SkipListNode(ValueType *kv, SkipListNode *p[MaxLevel], SkipListNode *n[MaxLevel]) : keyVal(kv), numLevels(generateNodeLevel(.5, MaxLevel)) {
            
                //only populate numLevels worth of valid pointers
                for (size_t i=0; i<numLevels; i++) {
                    prev[i] = p[i];
                    next[i] = n[i];
                }
                
                //the rest should be nullptr
                for (size_t i=numLevels; i<MaxLevel; i++) {
                    prev[i] = nullptr;
                    next[i] = nullptr;
                }
                
            }
            //default copy constructor should be fine
            SkipListNode(const SkipListNode &);
            
            SkipListNode *getNext(size_t level) const {
                assert(level < numLevels);
                return next[level];
            }
            SkipListNode *getPrev(size_t level) const {
                assert(level < numLevels);
                return prev[level];
            }
            void setNext(SkipListNode *n, size_t level) {
                assert(level < numLevels);
                next[level] = n;
            }
            void setPrev(SkipListNode *p, size_t level) {
                assert(level < numLevels);
                prev[level] = p;
            }
            
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
            
            size_t getLevels() {return numLevels;}
            
            
        private:
            
            int generateNodeLevel(double p, int maxLevel)
            {
                
                if (seeded == false) {
                    unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();
                    generator = std::default_random_engine((unsigned)seed);
                    distribution = std::uniform_real_distribution<double>(0.0,1.0);
                    seeded = true;
                }
                int level = 1;
                
                while (distribution(generator) < p)
                    level++;
                
                //assert(level > 1);
                
                return (level > maxLevel) ? maxLevel : level;
            }
            
            ValueType       *keyVal;
            SkipListNode    *prev[MaxLevel];
            SkipListNode    *next[MaxLevel];
            const size_t          numLevels;
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
                node = node->getNext(0);
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
                if ( !(node->getPrev(0)->isHead()) )
                    node = node->getPrev(0);
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
        SkipList() : head(&headValue, &head, &tail), tail(&tailValue, &head, &tail), highestLevel(0) {
            for(size_t i=0; i<MaxLevel; i++)
                numEntries[i]=0;
        }
        
        SkipList(const SkipList &sl) : head(&headValue, &head, &tail), tail(&tailValue, &head, &tail), highestLevel(0)  {
            
            for(size_t i=0; i<MaxLevel; i++)
                numEntries[i]=0;
            
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
        ~SkipList() {
            clear();
        }

        //Size
        size_t size() const { return numEntries[0]; }
        
        //Iterators
        Iterator begin() { return Iterator(head.getNext(0)); }
        Iterator end() { return Iterator(&tail); }
        
        ConstIterator begin() const {
            ConstIterator tmp(head.getNext(0));
            return tmp;
        }
        ConstIterator end() const {
            return ConstIterator(const_cast<SkipListNode *>(&tail));
        }
        
        //Element Access
        Iterator find(const Key_T &key) {
            SkipListNode *dummyArray[MaxLevel];
            return Iterator(findHelper(key, dummyArray));
        }
        ConstIterator find(const Key_T &key) const {
            SkipListNode *dummyArray[MaxLevel];
            return ConstIterator(findHelper(key, dummyArray));
        }
        
        //Modifiers
        std::pair<Iterator, bool> insert(const ValueType &kv) {
            
            
            //first, use find to determine whether the entry exits
            //Iterator it = find(kv.first);
            SkipListNode *prevArray[MaxLevel];
            
            SkipListNode *it = findHelper(kv.first, prevArray);
            
            //if not the tail, an entry with a matching key was found
            //return the found iterator with a true value
            if ( !(it->isTail()) )
                return std::pair<Iterator, bool>(it, true);
            
            //If a node with the same key cannot be found on the list,
            //find the node that comes after this key
            //SkipListNode *nextNode = findNextNode(kv.first);
            
            //insert a new node containing keyVal
            SkipListNode *newNode = insertHelper(kv, prevArray);
            
            //numEntries++;
            
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
            //numEntries--;
        }
        
        void erase(Iterator range_beg, Iterator range_end) {
            Iterator it = range_beg;
            while (it != range_end) {
                Iterator tmp(it);
                ++it;
                erase(tmp);
            }
        }
        
        void clear() {
            erase(begin(), end());
            assert(numEntries[0] == 0);
        }
        
        void Print(std::ostream &out) const {
            Iterator it = begin();
            for (;it!=end(); ++it)
                out<<"("<<it->first<<", "<<it->second<<"), ";
        }
        
        
        
        static ValueType headValue;
        static ValueType tailValue;
        
        
    private:
        
        //private helper functions
        //if the entry is not found, it returns a pointer to the
        //tail node
        SkipListNode *findHelper(const Key_T &key, SkipListNode *(prevList[MaxLevel]) ) {
            
            //initialize to head
            //these values will get overwritten if necessary
            for(size_t i=0; i<MaxLevel; i++)
                prevList[i] = &head;
            
            SkipListNode *node = &head;
            
            //need to use i-1 since highest level [0..MaxLevel)
            //0 signifies no entries
            //essentially, highestLevel should be set to the
            //index of the first 0 in numEntries
            for(size_t i=highestLevel+1; i>0; i--) {
                
                size_t level = i-1;
                
                do {
                    SkipListNode *nextNode = node->getNext(level);
                    //check for tail since tail key is not set
                    bool nextIsTail = (nextNode->isTail());
                    //check to see if input key is greater than next node's key
                    bool nextkeyGTInputKey =(key < nextNode->key());
                    //if the next node is not tail or input key >= next node's key,
                    //iterate the pointer
                    //in the case where keys are equal (match), we want to make sure we set
                    //node to next node. In this case, key will not be greater than next key
                    if ( nextIsTail || nextkeyGTInputKey)
                        break;
                    node = nextNode;
                } while (true);
                prevList[i-1] = node;
                
                //check to see if keys match, if so, we found our entry
                //must first check for the head though, since head's key not set
                if (!(node->isHead()) && (node->keyMatch(key)))
                    return node;
            }
            
            return &tail;
            
            
            
            
//            if (head.getNext()->isTail()) {
//                assert(numEntries == 0);
//                assert(tail.getPrev()->isHead());
//                return &tail;
//            }
//            
//            assert(numEntries > 0);
//            
//            SkipListNode *it = head.getNext();
//            //start at element after head
//            while (!(it->isTail())) {
//                //if the keys match, exit loop
//                if (it->keyMatch(key))
//                    break;
//                //otherwise, go to the next entry in the list
//                it = it->getNext();
//            }
//            //at this point, either we found the entry
//            //or it points to the tail node
//            return it;
        }
        
        //returns a pointer to the existing node that should come after
        //the new node specified by key
        //This should only be called after we know the list does not contain
        //key
//        SkipListNode *findNextNode(const Key_T &key) const {
//            
//            //start with the first entry of the list
//            SkipListNode *it = head.getNext();
//            
//            //if head points to the tail, no entries
//            if (it->isTail()) {
//                assert(numEntries == 0);
//                return it;
//            }
//            
//            //loop until key in it is not less than key
//            //meaning that the key in the node is greater than key
//            //if we reach the tail, it should be inserted at the end of the list
//            while( (it->lessThan(key)) ) {
//                it = it->getNext();
//                if (it->isTail())
//                    break;
//            }
//            
//            //return a pointer to the first node that is greater than key
//            return it;
//        }
        
//        SkipListNode *insertHelper(const ValueType &keyVal, SkipListNode *nextNode) {
//            
//            //first, instantiate a new node
//            ValueType *newKeyVal = new ValueType(keyVal);
//            SkipListNode *prevNode = nextNode->getPrev();
//            SkipListNode *newNode = new SkipListNode(keyVal, prevNode, nextNode);
//            
//            //setup bidirectional list pointers
//            nextNode->setPrev(newNode);
//            prevNode->setNext(newNode);
//            
//            return newNode;
//            
//        }
        
        SkipListNode *insertHelper(const ValueType &keyVal, SkipListNode *prevList[MaxLevel]) {
            
            //first, instantiate a new node
            ValueType *newKeyVal = new ValueType(keyVal);
            
            SkipListNode *nextList[MaxLevel];
            //prevList will be insert path with head above it
            //next list will contain tail above insert path
            for(size_t i=0; i<MaxLevel; i++)
                    nextList[i] = prevList[i]->getNext(i);
            
            
            SkipListNode *newNode = new SkipListNode(newKeyVal, prevList, nextList);
            
            //insert new node into all required lists
            //setup bidirectional list pointers and iterate level count
            for(size_t i=0; i<newNode->getLevels(); i++) {
                nextList[i]->setPrev(newNode, i);
                prevList[i]->setNext(newNode, i);
                numEntries[i]++;
            }
            
            //potentially set new high water mark for levels
            //0 signifies no entries
            //essentially, highestLevel should be set to the
            //index of the first 0 in numEntries
            if (newNode->getLevels()-1 > highestLevel)
            {
                highestLevel = newNode->getLevels()-1;
                assert(highestLevel < MaxLevel);
            }
            
            
            return newNode;
            
        }

        void eraseHelper(SkipListNode *node) {
            
            //remove node fronm the lists that it is on
            //setup bidirectional list pointers and decrement level count
            for(size_t i=0; i<node->getLevels(); i++) {
                node->getNext(i)->setPrev(node->getPrev(i),i);
                node->getPrev(i)->setNext(node->getNext(i),i);
                numEntries[i]--;
            }
            
            //potentially update high water mark if the top level
            //no longer has any entries
            
            if (numEntries[highestLevel] == 0)
            {
                assert(highestLevel < MaxLevel);
                size_t i;
                for(i=0; i<MaxLevel; i++)
                    if (numEntries[i] == 0)
                        break;
                if(i<MaxLevel)
                    highestLevel = i;
                else
                    highestLevel = MaxLevel-1;
            }
            
            
            //delete pair and node
            delete node->getPairPtr();
            delete node;
        }
        
//        void eraseHelper(SkipListNode *node) {
//            //setup bidirectional list pointers
//            node->getNext()->setPrev(node->getPrev());
//            node->getPrev()->setNext(node->getNext());
//            
//            //delete pair and node
//            delete node->getPairPtr();
//            delete node;
//        }
        
        //head does not refer to first actual entry in list
        //the entry after head is the first real value
        SkipListNode head;
        SkipListNode tail;
        size_t numEntries[MaxLevel];
        size_t highestLevel;
        
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
        
        typename SkipList<Key_T, Mapped_T, MaxLevel>::Iterator it1 = L1.begin();
        typename SkipList<Key_T, Mapped_T, MaxLevel>::Iterator it2 = L2.begin();
        
        typename SkipList<Key_T, Mapped_T, MaxLevel>::Iterator it1End = L1.end();
        typename SkipList<Key_T, Mapped_T, MaxLevel>::Iterator it2End = L2.end();
        
        while (it1 != it1End) {
            
            //if we have reached the end of L2, L1 is NOT less than L2
            if (it2 == it2End)
                return false;
            
            //if there is an element in l1 that is less than its cooresponding
            //element in l2, return l1 < l2. Return true
            if ( (it1->first < it2->first) || (it1->second < it2->second) )
                return true;
            
            // if an element in L1 is greater than its core
            if ( (it2->first < it1->first) || (it2->second < it1->second) )
                return false;
            
            ++it1;
            ++it2;
        }
        //all elements up until it1 end are equal
        if (it2 == it2End)
            return false;
        
        return true;
    
    }
    
    template <typename Key_T, typename Mapped_T, size_t MaxLevel>
    std::ostream &operator<<(std::ostream &out, const SkipList<Key_T, Mapped_T, MaxLevel> &sl) {
        sl.Print(out);
        return out;
    }
    
    
    
    //static members
    template <typename Key_T, typename Mapped_T, size_t MaxLevel>
    typename SkipList<Key_T, Mapped_T, MaxLevel>::ValueType SkipList<Key_T, Mapped_T, MaxLevel>::headValue;
    template <typename Key_T, typename Mapped_T, size_t MaxLevel>
    typename SkipList<Key_T, Mapped_T, MaxLevel>::ValueType SkipList<Key_T, Mapped_T, MaxLevel>::tailValue;
    
    
    
    
    
    
    
    
}
#endif /* defined(__CS540Project2__SkipList__) */
