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
    
    //Safe Bool Idiom Code courtesy of Bjorn Karlsson
    //http://www.artima.com/cppsource/safebool3.html
    class safe_bool_base {
    public:
        typedef void (safe_bool_base::*bool_type)() const;
        void this_type_does_not_support_comparisons() const {}
        
        safe_bool_base() {}
        safe_bool_base(const safe_bool_base&) {}
        safe_bool_base& operator=(const safe_bool_base&) {return *this;}
        ~safe_bool_base() {}
    };
    
    template <typename T=void> class safe_bool : public safe_bool_base {
    public:
        operator bool_type() const {
            return (static_cast<const T*>(this))->boolean_test()
            ? &safe_bool_base::this_type_does_not_support_comparisons : 0;
        }
    protected:
        ~safe_bool() {}
    };
    
    template<> class safe_bool<void> : public safe_bool_base {
    public:
        operator bool_type() const {
            return boolean_test()==true ?
            &safe_bool_base::this_type_does_not_support_comparisons : 0;
        }
    protected:
        virtual bool boolean_test() const=0;
        virtual ~safe_bool() {}
    };
    
    template <typename T, typename U>
    bool operator==(const safe_bool<T>& lhs,const safe_bool<U>& rhs) {
        lhs.this_type_does_not_support_comparisons();
        return false;
    }
    
    template <typename T,typename U>
    bool operator!=(const safe_bool<T>& lhs,const safe_bool<U>& rhs) {
        lhs.this_type_does_not_support_comparisons();
        return false;	
    }
    
    //C++ type erasure code courtesy of user jsmith
    //http://www.cplusplus.com/articles/oz18T05o/    
    class Object {
        
        struct ObjectConcept {
            virtual ~ObjectConcept() {}
        };
        
        template< typename T > class OriginalObject : public ObjectConcept {
        public:
            OriginalObject ( T *t ) : objPtr( t ) {}
            virtual ~OriginalObject() {
                assert(objPtr != nullptr);
                delete objPtr;
            }
        private:
            T *objPtr;
        };
        
        ObjectConcept *objCon;
        
    public:
        template< typename T > Object( T *obj ) :
        objCon( new OriginalObject<T>( obj ) ) {
            //std::cout<<"Creating new OriginalObject: "<<obj<<std::endl;
        }
        
        ~Object() {delete objCon;}
        
    };
    
    class RefCounter {
    public:
        RefCounter(unsigned long long c) : count(c) { }

        void Increment() {count++;}
        
        //only the value from Decrement should be trusted to determine whether to delete objects
        unsigned long long Decrement() {return --count;}
        
        //value return from getCount not considered thread safe.
        unsigned long long getCount() const {return count;}
    private:
        std::atomic<unsigned long long> count;
    };
    
    template <typename T> class Sptr : public safe_bool<> {
        //API
    public:
        //should this be initialized to 0 or 1 in this instance?
        Sptr() : object(nullptr), refCount(nullptr), originalObject(nullptr) {
            //std::cout<<"Calling Empty Constructor"<<std::endl;
        };
        
        //
        template <typename U>
        Sptr(U * ptr) : object(ptr), refCount(new RefCounter(1)) {
            originalObject = new Object(ptr);
            //std::cout<<"Calling Obj Ptr Constructor: "<<ptr<<std::endl;
        }
        
        ~Sptr() {
            reset();
        }
        
        //need to account for copying null ptr
        Sptr(const Sptr &ptr) {
            object = ptr.object;
            originalObject = ptr.originalObject;
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
            T *tPtr = static_cast<T*>(ptr.get());
            //T *tPtr = dynamic_cast<T*>(ptr.get());
            T &t = *(ptr.get());
            (void)t;
            
                        
            object = tPtr;
            originalObject = ptr.getOriginalObject();
            
            //improve upon this
            refCount = ptr.getRefCounter();
            
            //only increment reference count in the case
            //that there is an actual object
            if (object != nullptr) {
                assert(refCount != nullptr);
                refCount->Increment();
            }
            
        }
        
        Sptr(T *tPtr, RefCounter *rc, Object *oo) : object(tPtr), refCount(rc), originalObject(oo) {
            object = tPtr;
            originalObject = oo;
            
            //only increment reference count in the case
            //that there is an actual object
            if (object != nullptr) {
                assert(refCount != nullptr);
                refCount->Increment();
            }
            
        }
        
        
        //handle cases where ptr refers to self AND
        //where objects are already the same
        //should do nothing in either case
        Sptr &operator=(const Sptr &ptr) {
            
            if ( (this == &ptr) || (object == ptr.object) )
                return *this;
            
            //reset the pointer
            reset();
            
            object = ptr.object;
            originalObject = ptr.originalObject;
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
            T *tPtr = static_cast<T*>(ptr.get());
            
            T &t = *(ptr.get());
            (void)t;
            
            if ( object == tPtr )
                return *this;
            
            //reset the pointer
            reset();
            
            object = tPtr;
            originalObject = ptr.getOriginalObject();
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
                //delete object;
                //only delete original object
                assert(originalObject != nullptr);
                delete originalObject;
                delete refCount;
            }
            
            object = nullptr;
            originalObject = nullptr;
            refCount = nullptr;
        }
       
        T &operator*() const {
            assert(object != nullptr);
            //std::cout<<"Deferencing... Reference Count: "<<refCount->getCount()<<std::endl;
            return *object;
        }
        
        T *operator->() const { return get(); }
        
        T *get() const { return object; }
        
        //operator unspecified_bool_type() const;
        
        
        RefCounter *getRefCounter() const {return refCount;}
        Object *getOriginalObject() const {return originalObject;}
        
        
        
        //we need to be very careful with this, as this
        //could cause objects to be leaked
        void setObject(T *obj) {
            //assert(object == nullptr);
            object = obj;
        }
        
    protected:
        bool boolean_test() const {
            return (object != nullptr);
        }
        
    private:
        RefCounter *refCount;
        T *object;
        Object *originalObject;
        
    };
    
    template <typename T1, typename T2>
    bool operator==(const Sptr<T1> &ptr1, const Sptr<T2> &ptr2) {
        T1 *t1Ptr2 = static_cast<T1*>(ptr2.get());
        return (ptr1.get() == t1Ptr2);
    }
    
    template <typename T, typename U>
    Sptr<T> static_pointer_cast(const Sptr<U> &sp) {
        //possible that static_cast changes the pointer value
        //due to multiple inheritance
        T *tPtr = static_cast<T*>(sp.get());
        
        //therefore, we need to assign the pointer returned from
        //static_cast, but we also need to preserve the originalObject
        //and refCount from sp
        Sptr<T> retPtr(tPtr, sp.getRefCounter(), sp.getOriginalObject());
        
        //return new smart pointer
        return retPtr;
    }
    
    template <typename T, typename U>
    Sptr<T> dynamic_pointer_cast(const Sptr<U> &sp) {
        //possible that dynamic_cast changes the pointer value
        //due to multiple inheritance
        T *tPtr = dynamic_cast<T*>(sp.get());
        
        //if the returned value is null, return a Sptr to nullptr
        if (tPtr==0)
            return Sptr<T>();
        
        //we need to assign the pointer returned from dynamic_cast
        //but we also need to preserve the originalObject
        //and refCount from sp
        Sptr<T> retPtr(tPtr, sp.getRefCounter(), sp.getOriginalObject());
        
        //return new smart pointer
        return retPtr;
    }
    
}

#endif /* defined(__CS540Project2__Sptr__) */
