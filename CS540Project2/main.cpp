//
//  main.cpp
//  CS540Project2
//
//  Created by James Kizer on 3/18/13.
//  Copyright (c) 2013 James Kizer. All rights reserved.
//
// Updated to test git

#include <iostream>
#include <sstream>
#include "Sptr.hpp"
#include "SkipList.hpp"

class A {
public:
    A() {std::cout << "A Default Constructor!\n";};
    A(const char *p) : s(p) {std::cout << "A char* Constructor!\n";};
    void foo(int i) {std::cout<<"Printing i: "<<i<<std::endl;};
    
    ~A() {
        std::cout << "A Destructor!\n";
    }
    
private:
    std::string s;
};
class B : public A {
public:
    B() : A("B"), buf(new char[10]) {
        std::cout << "B Default Constructor!\n";
    }
    ~B() {
        delete [] buf;
        std::cout << "B Destructor!\n";
    }
    
    void non_const_funct() {
        buf[0] = 'A';
    }
    
    void const_funct() const {};
private:
    char *buf;
};

class A2 {
public:
    A2() {};
    A2(const char *p) : s(p) {};
    void foo(int i) {std::cout<<"Printing i: "<<i<<std::endl;};
    
private:
    std::string s;
};

using namespace cs540;

int main(int argc, const char * argv[])
{

    // insert code here...
    std::cout << "Hello, World!\n";
    
    
                Sptr<A> ap1(new A("a string"));
                Sptr<A> ap2; // Initialized to point to null pointer.
                Sptr<A> ap2a = ap2;
    
                ap2a = ap1;
    
                ap2a = ap2;
    
                ap1->foo(1234); // Arrow operator.
                
                Sptr<B> bp1(new B);
                Sptr<B> bp2(bp1); // Copy constructor.
                Sptr<A> ap3(bp2); // Automatic conversion to base class pointer.
    
                //Sptr<A2> ap30(bp2); // Automatic conversion to base class pointer.

                bp1 = bp1; // Self-assignment must be safe.
                bp1 = bp2; // Self-assignment must be safe.
                ap3 = bp2; // Self-assignment must be safe, even if one is a base class pointer.
    
    bp2 = ap3; //

                (*bp1).foo(456); // Can be dereferenced.
                bp1.reset(); // Set to null pointer.
                bp1.reset(); // Set to null pointer.

    //need to test with 2 NULL, 1 NULL/non-NULL, 1 non-NULL/1NULL, 2 non-NULL
    if (bp1 == bp2) { std::cout << "Hello, World!\n"; } // Can be compared.
    if (bp2 == bp1) { std::cout << "Hello, World!\n"; } // Can be compared.
    if (bp1 == ap1) { std::cout << "Hello, World!\n"; } // Implicit conversion must happen.
                
                // Static casting, returns a smart pointer.
                Sptr<B> bp3(static_pointer_cast<B>(ap1));
    
                // Static casting, returns a smart pointer.
                Sptr<A> ap4(dynamic_pointer_cast<A>(bp3));
    
    {
        Sptr<A> ap;
        {
            Sptr<B> bp(new B); // Line L1
            ap = bp;
        }
        // At this point, the destructor for ap will cause the object created at Line
        // L1 to be deleted.  However, note that B is not polymorphic.  Thus, to
        // ensure that B::~B() is called and buf freed, the object must be deleted
        // via a pointer to B.
    }
                
                // const-ness should be preserved.
                Sptr<const B> c_bp(new B);
                //c_bp->non_const_funct(); // Should be a syntax error.
                c_bp->const_funct(); // Should be a syntax error.
                
                // Test for null using safe-bool idiom.
                if (bp1)
                    std::cout << "bp1 is not NULL!\n";
                else
                    std::cout << "bp1 is NULL!\n";
    
    // Test for null using safe-bool idiom.
    if (ap1)
        std::cout << "ap1 is not NULL!\n";
    else
        std::cout << "ap1 is NULL!\n";
    
    typedef SkipList<int, std::string, 16> SLI;
    SLI sl;
    SLI::ValueType pair0(0,"happy");
    SLI::ValueType pair1(1,"birthday");
    SLI::ValueType pair2(2,"yes");
    SLI::ValueType pair3(3,"no");
    
    std::cout<<sl<<std::endl;
    sl.insert(pair1);
    std::cout<<sl<<std::endl;
    sl.insert(pair0);
    std::cout<<sl<<std::endl;
    sl.insert(pair3);
    std::cout<<sl<<std::endl;
    sl.insert(pair3);
    std::cout<<sl<<std::endl;
    
    sl.clear();
    
    std::cout<<sl<<std::endl;
    sl.insert(pair1);
    std::cout<<sl<<std::endl;
    sl.insert(pair0);
    std::cout<<sl<<std::endl;
    sl.insert(pair3);
    std::cout<<sl<<std::endl;
    sl.insert(pair3);
    std::cout<<sl<<std::endl;
    
    SLI sl1 = sl;
    
    std::cout<<sl1<<std::endl;
    
    
    
    SLI::Iterator it = sl.begin();
    
    for (;it != sl.end(); ++it)
        std::cout << it->first<<":"<<it->second<<std::endl;
    
    SLI::ConstIterator c_it = sl.begin();
    
    SLI::ConstIterator c_it2(it);
    SLI::ConstIterator c_it3(c_it);
    
    c_it++;
    
    c_it2 = c_it;
    c_it = it;
    
    if (it == c_it)
        std::cout << "The iterators are equal!\n";
    else
        std::cout << "The iterators are NOT equal!\n";
    
    if (c_it == it)
        std::cout << "The iterators are equal!\n";
    else
        std::cout << "The iterators are NOT equal!\n";
    
    SLI::ValueType i1 = *(it);
    SLI::ValueType i2 = *(c_it);
    
    
    int x = 7;
    const int &c_int = x;
    
    x++;
    
    int y = c_int;
    
    
    
    
    it++;
    
    SLI::Iterator it2 = sl.end();
    
    it2 = it;
    it = sl.end();
    it--;
    it2--;
    
    auto retVal = sl.insert(pair2);
    retVal = sl.insert(pair2);
    
    sl.clear();
    
    
    
    sl1.insert(pair1);
    
    sl = sl1;
    
    
    if (sl == sl1)  
        std::cout << "The lists are equal!\n";
    else
        std::cout << "The lists are NOT equal!\n";
    
    std::cout<<sl<<std::endl;
    std::cout<<sl1<<std::endl;
    
//    it = sl.begin();
//    
//    for (;it != sl.end(); ++it)
//        std::cout << it->first<<":"<<it->second<<std::endl;
//    
//    c_it = sl1.begin();
//    
//    for (;c_it != sl1.end(); ++c_it)
//        std::cout << it->first<<":"<<it->second<<std::endl;
    
    sl.clear();
    
    for (size_t i=0; i<0xFFFF; i++) {
        
        std::string s = "teststring";
        std::stringstream ss;
        ss<<i;
        s += ss.str();
        
        sl.insert(std::make_pair(i, s));
    }
    std::cout<<sl<<std::endl;
    
    
    return 0;
}

