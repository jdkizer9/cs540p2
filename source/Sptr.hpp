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
#include <atomic>

namespace cs540 {
    
    class RefCounter {
    public:
        RefCounter(unsigned long long c) : count(c) { }

        void Increment() {count++;}
        unsigned long long Decrement() {return count--;}
        unsigned long long getCount() const {return count;}
    private:
        std::atomic<unsigned long long> count;
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
            T *tPtr = static_cast<T*>(ptr.operator->());
            
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
            
            //reset the pointer
            reset();
            
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
            T *tPtr = static_cast<T*>(ptr.operator->());
            
            if ( object == tPtr )
                return *this;
            
            //reset the pointer
            reset();
            
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
            
            //this pointer currently points to an object
            //decrement the reference count
            // if there are no more references to the object,
            //delete the object
            //1) refCount Incrememnt and Decrement are atomic operations
            //2) the actual Sptr's are not shared by threads /tasks
            //Therefore, if Decrement returns 0, that means that this Sptr
            //was the lone reference and the object and refCount can now be safely
            //deleted
            assert(refCount != nullptr);
            if (refCount->Decrement() == 0){
                delete object;
                delete refCount;
            }
            
            object = nullptr;
            refCount = nullptr;
        }
       
        T &operator*() const {
            assert(object != nullptr);
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
    
    template <typename T1, typename T2>
    bool operator==(const Sptr<T1> &ptr1, const Sptr<T2> &ptr2) {
        T1 *t1Ptr2 = static_cast<T1*>(ptr2.operator->());
        return (ptr1.operator->() == t1Ptr2);
    }
    
    template <typename T, typename U>
    Sptr<T> static_pointer_cast(const Sptr<U> &sp) {
        //get object* defined in sp
        //static cast from U* to T*
        
        T *tPtr = static_cast<T*>(sp.operator->());
        //return new T Sptr
        return Sptr<T>(tPtr);
    }
    
//    template <typename T, typename U>
//    Sptr<T> dynamic_pointer_cast(const Sptr<U> &sp);
    
}

#endif /* defined(__CS540Project2__Sptr__) */
