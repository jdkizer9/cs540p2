//
//  SLTest.cpp
//  CS540Project2
//
//  Created by James Kizer on 4/20/13.
//  Copyright (c) 2013 James Kizer. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <vector>


#include "SkipList.hpp"

//create class for key and type that only has
//copy constructor, destructor, and less-than operator
//due to Skip List comparison, both key and value must also support == operator
//to support << operator, also overload <<
template <typename T>
class KeyValClass {
  
public:
    //no default constructor
    KeyValClass() = delete;
    KeyValClass(T k) : key(k) {}
    
    bool operator<(const KeyValClass &k) const{
        return key < k.key;
    }
    
    bool operator==(const KeyValClass &k) const{
        return key == k.key;
    }
    
    void Print(std::ostream &out) const {
        out<<key;
    }
    
    void set(const T &k) {
        key = k;
    }
    
    
    
private:
    //make assignment private to disable default
    //asignment operator
    KeyValClass& operator=(const KeyValClass &it);
    T key;
    
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const KeyValClass<T> &k) {
    k.Print(out);
    return out;
}

template <typename Key_T, typename Mapped_T, size_t MaxLevel>
void populateList(cs540::SkipList<Key_T, Mapped_T, MaxLevel> &sl, size_t entries, size_t starting=0) {
    
    for (size_t i=starting; i<(entries+starting); i++) {
        
        std::string s = "teststring";
        std::stringstream ss;
        ss<<i;
        s += ss.str();
        
        sl.insert(std::make_pair(i, s));
    }
}

template <typename Key_T, typename Mapped_T, size_t MaxLevel>
void testSize(cs540::SkipList<Key_T, Mapped_T, MaxLevel> &sl) {
    
    size_t testSize = 0;
    auto it = sl.begin();
    while (it != sl.end()) {
        testSize++;
        ++it;
    }
    assert(testSize == sl.size());
}

//template <typename Key_T, typename Mapped_T, size_t MaxLevel>
//std::ostream &operator<<(std::ostream &out, const typename cs540::SkipList<Key_T, Mapped_T, MaxLevel>::ValueType &keyval) {
//    //out<<"("<<it->first<<", "<<it->second<<"), ";
//    return out;
//}

//template <typename Key_T, typename Mapped_T, size_t MaxLevel>
//std::ostream &operator<<(std::ostream &out, const SkipList<Key_T, Mapped_T, MaxLevel> &sl) {
//    sl.Print(out);
//    return out;
//}




int main(int argc, const char * argv[]) {
    
    //Key Value Class Tests
    std::cout<<"KeyVal<T> Class Tests\n";
    std::cout<<"*********************\n";
    KeyValClass<int> key0(0);
    KeyValClass<int> key1(key0);
    
    if (key0 < key1)
        std::cout<<key0<<" is less than "<<key1<<std::endl;
    else
        std::cout<<key0<<" is NOT less than "<<key1<<std::endl;
    
    if (key0 == key1)
        std::cout<<key0<<" is equal to "<<key1<<std::endl;
    else
        std::cout<<key0<<" is NOT equal to "<<key1<<std::endl;
    
    KeyValClass<std::string> val0("test string");
    KeyValClass<std::string> val1(val0);
    
    if (val0 < val1)
        std::cout<<val0<<" is less than "<<val1<<std::endl;
    else
        std::cout<<val0<<" is NOT less than "<<val1<<std::endl;
    
    if (val0 == val1)
        std::cout<<val0<<" is equal to "<<val1<<std::endl;
    else
        std::cout<<val0<<" is NOT equal to "<<val1<<std::endl;
    
    
    std::cout<<"*********************\n\n";
    typedef cs540::SkipList<int, std::string, 16> SLI_INT_STR_16_a;
    
    typedef cs540::SkipList<KeyValClass<int>, KeyValClass<std::string>, 16> SLI_INT_STR_16;
    typedef cs540::SkipList<KeyValClass<int>, KeyValClass<std::string>, 32> SLI_INT_STR_32;
    typedef cs540::SkipList<KeyValClass<int>, KeyValClass<int>, 32> SLI_INT_INT_32;
    typedef cs540::SkipList<KeyValClass<std::string>, KeyValClass<std::string>, 16> SLI_STR_STR_32;
    
    typedef typename cs540::SkipList<KeyValClass<int>, KeyValClass<std::string>, 16>::ValueType VT;
    
    typedef typename cs540::SkipList<KeyValClass<int>, KeyValClass<std::string>, 16>::Iterator IT;
    
    typedef typename cs540::SkipList<KeyValClass<int>, KeyValClass<std::string>, 16>::ConstIterator CT;
    
    std::cout<<"SkipList Constructor Tests\n";
    std::cout<<"*********************\n";
    
    //default constructors
    //SLI_INT_STR_16_a sl;
    SLI_INT_STR_16 sl1;
    SLI_INT_STR_32 sl2;
    SLI_INT_INT_32 sl3;
    SLI_STR_STR_32 sl4;
    
    //copy constructors - normal test
    SLI_INT_STR_16 sl1a(sl1);
    assert(sl1a == sl1);
    SLI_INT_STR_32 sl2a(sl2);
    assert(sl2a == sl2);
    SLI_INT_INT_32 sl3a(sl3);
    assert(sl3a == sl3);
    SLI_STR_STR_32 sl4a(sl4);
    assert(sl4a == sl4);
    
    //copy constructors - large list
    populateList(sl1, 0xFFFF);
    testSize(sl1);
    populateList(sl2, 0xFF);
    testSize(sl1);
    
    SLI_INT_STR_16 sl1b(sl1);
    assert(sl1b == sl1);
    
    SLI_INT_STR_32 sl2b(sl2);
    assert(sl2b == sl2);
    
    //copy constructors - robust test
    //same key / val types, different MaxLevels
    //These cause clang to crash, ignore for now
    //SLI_INT_STR_16 sl1b(sl2a);
    //SLI_INT_STR_32 sl2b(sl1a);
    //SLI_INT_INT_32 sl3b(sl2a);
    
    //assignment operators
    SLI_INT_STR_16 sl1c;
    sl1c = sl1;
    assert(sl1c == sl1);
    
    SLI_INT_STR_32 sl2c;
    sl2c = sl2;
    assert(sl2c == sl2);
    
    //element operation tests
    sl1.clear();
    sl2.clear();
    
    populateList(sl1, 8);
    populateList(sl2, 8, 8);
    
    std::cout<<"SL1"<<std::endl;
    std::cout<<sl1<<std::endl;
    
    std::cout<<"SL2"<<std::endl;
    std::cout<<sl2<<std::endl;
    
    
    std::cout<<"*********************\n";
    std::cout<<"Before First Insert"<<std::endl;
    std::cout<<"*********************\n";
    for(int i=0; i<16; i++) {
        auto it1 = sl1.find(i);
        auto it2 = sl2.find(i);
        
        if (it1 != sl1.end())
            std::cout<<"Found "<<"("<<it1->first<<", "<<it1->second<<"), "<<" on sl1"<<std::endl;
        
        if (it2 != sl2.end())
            std::cout<<"Found "<<"("<<it2->first<<", "<<it2->second<<"), "<<" on sl2"<<std::endl;
    }
    
    auto it2b = sl2.begin();
    auto it2e = sl2.end();
    
    sl1.insert(it2b, it2e);
    
    std::cout<<"*********************\n";
    std::cout<<"After First Insert"<<std::endl;
    std::cout<<"*********************\n";
    
    for(int i=0; i<16; i++) {
        auto it1 = sl1.find(i);
        auto it2 = sl2.find(i);
        
        if (it1 != sl1.end())
            std::cout<<"Found "<<"("<<it1->first<<", "<<it1->second<<"), "<<" on sl1"<<std::endl;
        
        if (it2 != sl2.end())
            std::cout<<"Found "<<"("<<it2->first<<", "<<it2->second<<"), "<<" on sl2"<<std::endl;
    }
    
    auto it1b = sl1.begin();
    auto it1e = sl1.end();
    
    sl2.insert(it1b, it1e);
    
    std::cout<<"*********************\n";
    std::cout<<"After Second Insert"<<std::endl;
    std::cout<<"*********************\n";
    
    for(int i=0; i<16; i++) {
        auto it1 = sl1.find(i);
        auto it2 = sl2.find(i);
        
        if (it1 != sl1.end())
            std::cout<<"Found "<<"("<<it1->first<<", "<<it1->second<<"), "<<" on sl1"<<std::endl;
        
        if (it2 != sl2.end())
            std::cout<<"Found "<<"("<<it2->first<<", "<<it2->second<<"), "<<" on sl2"<<std::endl;
    }
    
    //remove evens from sl1 and odds from sl2
    for(int i=0; i<16; i++) {
        auto it1 = sl1.find(i);
        auto it2 = sl2.find(i);
        
        if ( (it1 != sl1.end()) && (i%2 == 0) )
            sl1.erase(it1);
            
        
        if ( (it2 != sl2.end()) && (i%2 == 1))
            sl2.erase(it2);
    }
    
    std::cout<<"*********************\n";
    std::cout<<"After First Erase"<<std::endl;
    std::cout<<"*********************\n";
    
    for(int i=0; i<16; i++) {
        auto it1 = sl1.find(i);
        auto it2 = sl2.find(i);
        
        if (it1 != sl1.end())
            std::cout<<"Found "<<"("<<it1->first<<", "<<it1->second<<"), "<<" on sl1"<<std::endl;
        
        if (it2 != sl2.end())
            std::cout<<"Found "<<"("<<it2->first<<", "<<it2->second<<"), "<<" on sl2"<<std::endl;
    }
    
    
    //populate vector with sl1 entries in range 4-12 (odds)
    std::vector<VT> vec;
    for(int i=4; i<12; i++) {
        auto it1 = sl1.find(i);
        
        if (it1 != sl1.end())
            vec.push_back(*it1);
    }
    
    //clear sl1 using erase
    std::cout<<sl1<<std::endl;
    sl1.erase(sl1.begin(), sl1.end());
    std::cout<<sl1<<std::endl;
    
    //insert range from vector
    sl1.insert(vec.begin(), vec.end());
    std::cout<<sl1<<std::endl;
    
    //test comparison functions
    sl1a.clear();
    sl1a.insert(sl1.begin(), sl1.end());
    
    assert(sl1 == sl1a);
    assert(!(sl1 != sl1a));
    assert(!(sl1 < sl1a));
    assert(!(sl1a < sl1));
    
    sl1a.insert( std::make_pair(0, std::string("newTestString")) );
    
    assert(!(sl1 == sl1a));
    assert(sl1 != sl1a);
    assert(!(sl1 < sl1a));
    assert(sl1a < sl1);
    
    sl1.clear();
    sl1.insert(std::make_pair(0, std::string("newTestString")));
    
    sl1a = sl1;
    std::cout<<sl1<<std::endl;
    std::cout<<sl1a<<std::endl;
    assert(sl1 == sl1a);
    assert(!(sl1 != sl1a));
    assert(!(sl1 < sl1a));
    assert(!(sl1a < sl1));
    
    IT it = sl1a.find(0);
    it->second.set(std::string("newerTestString"));
    
    std::cout<<sl1<<std::endl;
    std::cout<<sl1a<<std::endl;
    
    assert(!(sl1 == sl1a));
    assert(sl1 != sl1a);
    assert(sl1 < sl1a);
    assert(!(sl1a < sl1));
    
    
    //Iterators should let you only read the key
    //and let you update the value
    std::cout<<sl1a<<std::endl;
    it = sl1a.find(0);
    //syntax error
    //it->first.set(1);
    it->second.set(std::string("EvenNewerTestString"));
    std::cout<<sl1a<<std::endl;
    
    //ensure that ConstIterator will not let you set either
    CT c_it = sl1a.find(0);
    std::cout<<"Found "<<"("<<c_it->first<<", "<<c_it->second<<") on sl1a"<<std::endl;
    //syntax error
    //c_it->first.set(1);
    //syntax error
    //c_it->second.set(std::string("EvenNewerTestString"));
    std::cout<<sl1a<<std::endl;
    
    sl1.clear();
    populateList(sl1, 8);
    
    it = sl1.begin();
    while (it != sl1.end()) {
        std::cout<<"Found "<<"("<<it->first<<", "<<it->second<<") on sl1"<<std::endl;
        ++it;
    }
    do {
        --it;
        std::cout<<"Found "<<"("<<it->first<<", "<<it->second<<") on sl1"<<std::endl;
    } while (it != sl1.begin());
    
    std::cout<<"*********************\n";
    
    it = sl1.begin();
    while (it != sl1.end()) {
        std::cout<<"Found "<<"("<<it->first<<", "<<it->second<<") on sl1"<<std::endl;
        it++;
    }
    do {
        it--;
        std::cout<<"Found "<<"("<<it->first<<", "<<it->second<<") on sl1"<<std::endl;
    } while (it != sl1.begin());
    
    
    
    std::cout<<"*********************\n";
    c_it = sl1.begin();
    while (c_it != sl1.end()) {
        std::cout<<"Found "<<"("<<c_it->first<<", "<<c_it->second<<") on sl1"<<std::endl;
        ++c_it;
    }
    do {
        --c_it;
        std::cout<<"Found "<<"("<<c_it->first<<", "<<c_it->second<<") on sl1"<<std::endl;
    } while (c_it != sl1.begin());
    
    
    std::cout<<"*********************\n";
    c_it = sl1.begin();
    while (c_it != sl1.end()) {
        std::cout<<"Found "<<"("<<c_it->first<<", "<<c_it->second<<") on sl1"<<std::endl;
        c_it++;
    }
    do {
        c_it--;
        std::cout<<"Found "<<"("<<c_it->first<<", "<<c_it->second<<") on sl1"<<std::endl;
    } while (c_it != sl1.begin());
    
    it = sl1.begin();
    IT it3(it);
    CT c_it3(it3);
    CT c_it2(c_it3);
    
    assert(it == it3);
    assert(c_it3 == it3);
    assert(it3 == c_it3);
    assert(c_it3 == c_it2);
    
    it++;
    ++c_it2;
    assert(it == c_it2);
    assert(it != it3);
    assert(c_it3 == it3);
    assert(it3 == c_it3);
    assert(c_it3 != c_it2);
    
    --it;
    c_it2--;
    assert(it == it3);
    assert(c_it3 == it3);
    assert(it3 == c_it3);
    assert(c_it3 == c_it2);
    
    std::cout<<"Found "<<"("<<c_it3->first<<", "<<c_it3->second<<") on sl1"<<std::endl;
    c_it3 = it3;
    std::cout<<"Found "<<"("<<c_it3->first<<", "<<c_it3->second<<") on sl1"<<std::endl;
    c_it3 = c_it3;
    std::cout<<"Found "<<"("<<c_it3->first<<", "<<c_it3->second<<") on sl1"<<std::endl;
    c_it3 = c_it2;
    std::cout<<"Found "<<"("<<c_it3->first<<", "<<c_it3->second<<") on sl1"<<std::endl;
    
    it3 = c_it3;
    std::cout<<"Found "<<"("<<c_it3->first<<", "<<c_it3->second<<") on sl1"<<std::endl;
    
    c_it3 = sl1.find(4);
    std::cout<<"Found "<<"("<<c_it3->first<<", "<<c_it3->second<<") on sl1"<<std::endl;
    
    CT ctB(sl1.begin());
    ctB = sl1.begin();
    CT ctE = sl1.end();
    ctE = sl1.end();
    
    CT ctI = ctB;
    do {
        std::cout<<"Found "<<"("<<ctI->first<<", "<<ctI->second<<") on sl1"<<std::endl;
    } while(++ctI != ctE);
    
    
    it = sl1.begin();
    it++;
    c_it = it;
    assert(it++ != ++c_it);
    assert(it == c_it);
    assert(--it != c_it--);
    assert(it == c_it);
    
}