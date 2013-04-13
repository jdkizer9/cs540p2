//
//  main.cpp
//  CS540Project2
//
//  Created by James Kizer on 3/18/13.
//  Copyright (c) 2013 James Kizer. All rights reserved.
//
// Updated to test git

#include <iostream>
#include "Sptr.hpp"

class A {
public:
    A() {};
    A(const char *p) : s(p) {};
    void foo(int i) {std::cout<<"Printing i: "<<i<<std::endl;};
    
private:
    std::string s;
};
class B : public A {
public:
    B() : A("B"), buf(new char[10]) {}
    ~B() { delete [] buf; }
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

                (*bp1).foo(456); // Can be dereferenced.
                bp1.reset(); // Set to null pointer.
//                
//                if (bp1 == bp2) { ... } // Can be compared.
//                if (bp1 == ap1) { ... } // Implicit conversion must happen.
//                
//                // Static casting, returns a smart pointer.
//                Sptr<B> bp3(static_pointer_cast<B>(ap1));
//                
//    {
//        Sptr<A> ap;
//        {
//            Sptr<B> bp(new B); // Line L1
//            ap = bp;
//        }
//        // At this point, the destructor for ap will cause the object created at Line
//        // L1 to be deleted.  However, note that B is not polymorphic.  Thus, to
//        // ensure that B::~B() is called and buf freed, the object must be deleted
//        // via a pointer to B.
//    }
//                
//                // const-ness should be preserved.
//                Sptr<const B> c_bp(new B);
//                c_bp->non_const_funct(); // Should be a syntax error.
//                
//                // Test for null using safe-bool idiom.
//                if (sp) { ... } // True if non-null.
    
    return 0;
}

