//
//  Sptr.hpp
//  CS540Project2
//
//  Created by James Kizer on 3/27/13.
//  Copyright (c) 2013 James Kizer. All rights reserved.
//

#ifndef __CS540Project2__Sptr__
#define __CS540Project2__Sptr__

#include <iostream>
#include <assert.h>

namespace cs540 {
    
    class RefCounter {
    public:
        RefCounter(unsigned long long c) : count(c) { }
        
        //need to lock these
        //potentially could use atomic modify
        void Increment() {count++;}
        void Decrement() {count--;}
        unsigned long long getCount() {return count;}
        
    private:
        unsigned long long count;
    };
    
    template <typename T> class Sptr {
        //API
    public:
        //should this be initialized to 0 or 1 in this instance?
        Sptr() : object(nullptr), refCount(nullptr) {
            std::cout<<"Calling Empty Constructor"<<std::endl;};
        
        //
        template <typename U>
        Sptr(U * ptr) : object(ptr), refCount(new RefCounter(1)) {
            std::cout<<"Calling Obj Ptr Constructor"<<std::endl;
        }
        
        //need to account for copying null ptr
        Sptr(const Sptr &ptr) {
            object = ptr.object;
            refCount = ptr.refCount;
            //only increment reference count in the case
            //that there is an actual object
            if (object != nullptr) {
                assert(refCount != nullptr);
                refCount->Increment();
            }
                
        }
        
        //U* must be implicitly convertible to T*
        //in this case, U is the derived (B) and T is the base (A)
        template <typename U> Sptr(const Sptr<U> &ptr) {
            T *tPtr = static_cast<T*>(&(*ptr));
            
            object = tPtr;
            
            //improve upon this
            refCount = ptr.getRefCounter();
            
            //only increment reference count in the case
            //that there is an actual object
            if (object != nullptr) {
                assert(refCount != nullptr);
                refCount->Increment();
            }
            
        }
        
        RefCounter *getRefCounter() const {return refCount;}
        
        
        
        //need to account for assigning null ptr
        
        
        //handle cases where ptr refers to self AND
        //where objects are already the same
        //should do nothing in either case
        Sptr &operator=(const Sptr &ptr) {
            
            if ( (this == &ptr) || (object == ptr.object) )
                return *this;
            
            //if this pointer currently points to an object
            //decrement the reference count and delete
            //the object if there are no longer any references
            if (object != nullptr) {
                
                assert(refCount != nullptr);
                refCount->Decrement();
                if (refCount->getCount() == 0){
                    delete object;
                    delete refCount;
                }                
            }
            
            object = ptr.object;
            refCount = ptr.refCount;
            
            //only increment reference count in the case
            //that there is an actual object
            if (object != nullptr) {
                assert(refCount != nullptr);
                refCount->Increment();
            }
            
            return *this;
        }
        
        //I believe that if these are two different types,
        //ptr cannot refer to self
        //handles case where objects are already the same
        //should do nothing in either case
        template <typename U> Sptr<T> &operator=(const Sptr<U> &ptr) {
            T *tPtr = static_cast<T*>(&(*ptr));
            
            if ( object == tPtr )
                return *this;
            
            assert(refCount != nullptr);
            refCount->Decrement();
            if (refCount->getCount() == 0){
                delete object;
                delete refCount;
            }
            
            object = tPtr;
            //improve upon this
            refCount = ptr.getRefCounter();
            
            //only increment reference count in the case
            //that there is an actual object
            if (object != nullptr) {
                assert(refCount != nullptr);
                refCount->Increment();
            }
            
            return *this;
        }

        
        void reset() {
            
            //if the pointer is already reset
            //simply return
            if (object == nullptr)
                return;
            
            assert(refCount != nullptr);
            refCount->Decrement();
            if (refCount->getCount() == 0){
                delete object;
                delete refCount;
            }
            
            object = nullptr;
            refCount = nullptr;
        }
       
        T &operator*() const {
            std::cout<<"Deferencing... Reference Count: "<<refCount->getCount()<<std::endl;
            return *object;
        }
        
        T *operator->() const { return object; }
        
//        T *get() const;
//        
        //operator unspecified_bool_type() const;
               
    private:
        RefCounter *refCount;
        T *object;
        
    };
    
//    template <typename T1, typename T2>
//    bool operator==(const Sptr<T1> &, const Sptr<T2> &);
//    
//    template <typename T, typename U>
//    Sptr<T> static_pointer_cast(const Sptr<U> &sp);
//    
//    template <typename T, typename U>
//    Sptr<T> dynamic_pointer_cast(const Sptr<U> &sp);
    
}

#endif /* defined(__CS540Project2__Sptr__) */
