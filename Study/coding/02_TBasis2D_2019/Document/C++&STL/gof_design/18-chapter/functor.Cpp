#include <iostream>
using namespace std;

// =================================================================
// FunctorBase, Functor0, Functor1, ...  
// =================================================================
// --- typeless representation of a function or object/mem-func
class FunctorBase {    
  public :         
    typedef void (FunctorBase::*_MemFunc)();                
    FunctorBase()  : callee(0), func(0)  {  }        
    FunctorBase(const void *c, const void *f, size_t sz) {            
      if ( c ) {   
        // --- must be callee/memfunc                
        callee = (void*) c;                
        memcpy(memFunc, f, sz);            
      }            
      else {     
        // --- must be ptr-to-func                
        func = f;            
      }        
    }        
    // for evaluation in conditions        
    // will be changed to bool instead of int when bool exists        
    operator int() const { return  func || callee ; }        
    class DummyInit { };        
    
    // Note : this code depends on all ptr-to-mem-funcs being same size        
    // If that is not the case then make memFunc as large as largest        
    union {            
      const void *func;            
      char memFunc[sizeof(_MemFunc)];         
    };        
    
    void *callee;
};

// --- Functor0 : No arg and no return  (Non template class)
class Functor0 : protected FunctorBase {    
  public :         
    Functor0(DummyInit * = 0)  { }        
    void operator() ( )  const   { thunk(*this); }        
    FunctorBase::operator int;    
  protected :         
    typedef void (*Thunk) (const FunctorBase&);        
    Functor0 (Thunk t, const void *c, const void *f, size_t sz)            
      : FunctorBase(c, f, sz), thunk(t)    {   }    
  private :         
    Thunk thunk;
};

// --- Functor1 : One arg and no return 
template <class P1>
class Functor1 : protected FunctorBase {    
  public :         
    Functor1(DummyInit * = 0)   { }        
    void operator() (P1 p1) const  { thunk(*this, p1); }        
    FunctorBase::operator int;    
  protected :         
    typedef void(*Thunk)(const FunctorBase&, P1);        
    Functor1(Thunk t, const void *c, const void *f, size_t sz)            
      : FunctorBase(c, f, sz), thunk(t)     {   }    
  private :        
    Thunk thunk;
};

// --- Functor2, ¡¦ , Functor0wRet, Functor1wRet, ¡¦ 
// --- will be similar to the above


// =================================================================
// FunctionTranslator0, MemberTranslator0
// =================================================================
// --- FunctionTranslator0
template <class Func>
class FunctionTranslator0 : public Functor0 {    
  public :         
    FunctionTranslator0 (Func f) : Functor0 (thunk, 0, (const void*)f, 0) { }        
    static void thunk (const FunctorBase &ftor) {            
      (Func (ftor.func) ) ();  
      // Type Casting and call FunctorBase's func        
    }
};

// --- MemberTranslator0
template <class Callee, class MemFunc>
class MemberTranslator0 : public Functor0 {    
  public :         
    MemberTranslator0 (Callee &c, const MemFunc &m)            
      : Functor0(thunk, &c, &m, sizeof(MemFunc))   {  }        
    static void thunk (const FunctorBase &ftor) {            
      Callee *callee = (Callee *)ftor.callee;            
      MemFunc &memFunc = (*(MemFunc*) (void *)(ftor.memFunc));  // inserted = in VC++
      // reference variable declaration using ftor.memFunc            
      (callee->*memFunc)();        
    }
};


// =================================================================
// FunctionTranslator1, MemberTranslator1
// =================================================================
// --- FunctionTranslator1
template <class P1, class Func>
class FunctionTranslator1 : public Functor1<P1> {    
  public :         
    FunctionTranslator1 (Func f): Functor1<P1> (thunk, 0, (const void*)f, 0) { }        
    static void thunk (const FunctorBase &ftor, P1 p1) {            
      (Func (ftor.func) ) (p1);  
      // Type Casting and call FunctorBase's func        
    }
};

// --- MemberTranslator1
template <class P1, class Callee, class MemFunc>
class MemberTranslator1 : public Functor1<P1> {    
  public :         
    MemberTranslator1 (Callee &c, const MemFunc &m)            
      : Functor1<P1>(thunk, &c, &m, sizeof(MemFunc))   {  }        
    static void thunk (const FunctorBase &ftor, P1 p1) {            
      Callee *callee = (Callee *)ftor.callee;            
      MemFunc &memFunc (*(MemFunc*) (void *)(ftor.memFunc));                
      // reference variable declaration using ftor.memFunc            
      (callee->*memFunc)(p1);        
    }
};

// --- all other FunctorTranslator and MemberTranslator 
// --- will be similar to the above


// =================================================================
// makeFunctor for FunctionTranslator0, MemberTranslator0
// =================================================================
// --- makeFunctor for ptr-to-function 
template <class TRT>    
// declare template parameters
inline FunctionTranslator0<TRT (*) ()>  
// declare return type
makeFunctor (Functor0 *, TRT (*f)() ) {     
  return FunctionTranslator0<TRT(*)()> (f);
}

// --- makeFunctor for ptr-to-non-const memfunc
template <class Callee, class TRT, class CallType> 
inline MemberTranslator0<Callee, TRT(CallType::*)()>  
// declare return type
makeFunctor (Functor0 *, Callee &c, TRT(CallType::* const &f)() ) {    
  // simplify function pointer type    
  typedef TRT(CallType::*MemFunc)();  
  return MemberTranslator0<Callee, MemFunc> (c, f);
}

// --- makeFunctor for ptr-to-const memfunc
template <class Callee, class TRT, class CallType> 
inline MemberTranslator0<const Callee, TRT(CallType::*)()const> 
makeFunctor (Functor0 *, const Callee &c, TRT(CallType::* const &f)()const ) {
  // simplify function pointer type    
  typedef TRT(CallType::*MemFunc)()const;  
  return MemberTranslator0<const Callee, MemFunc> (c, f);
}

// =================================================================
// makeFunctor for FunctionTranslator1, MemberTranslator1
// =================================================================
// --- makeFunctor for ptr-to-function 
template <class P1, class TRT, class TP1>    
// declare template parameters
inline FunctionTranslator1<P1, TRT (*) (TP1)>  
// declare return type
makeFunctor (Functor1<P1> *, TRT (*f)(TP1) ) {     
  return FunctionTranslator1<P1, TRT(*)(TP1)> (f);
}

// --- makeFunctor for ptr-to-non-const memfunc
template <class P1, class Callee, class TRT, class CallType, class TP1> 
inline MemberTranslator1<P1, Callee, TRT(CallType::*)(TP1)>  
// declare return type
makeFunctor (Functor1<P1> *, Callee &c, TRT(CallType::* const &f)(TP1) ) {    
  // simplify function pointer type    
  typedef TRT(CallType::*MemFunc)(TP1);  
  return MemberTranslator1<P1, Callee, MemFunc> (c, f);
}

// --- makeFunctor for ptr-to-const memfunc
template <class P1, class Callee, class TRT, class CallType, class TP1> 
inline MemberTranslator1<P1, const Callee, TRT(CallType::*)(TP1)const> 
makeFunctor (Functor1<P1> *, const Callee &c,                                  
    TRT(CallType::* const &f)(TP1)const ) {    
  // simplify function pointer type    
  typedef TRT(CallType::*MemFunc)(TP1)const;  
  return MemberTranslator1<P1, const Callee, MemFunc> (c, f);
}

// =================================================================
// Application Example
// =================================================================
// --- GUI Library for Button
class Button {
  public :
    Button(const Functor0 & uponClickDoThis) : notify(uponClickDoThis) { } 
    void click () { notify(); }  // call to operator()
  private :
    Functor0 notify;  // note --  held by value not pointer
};

// --- Application
class CDPlayer {
  public :
    void play() { cout << "Playing" << endl; }
    void stop() { cout << "Stopped" << endl; }
};

void wow() { 
  cout << "Wow !" << endl; 
}

void main () {    
  CDPlayer cd;     
  
  // --- makeFunctor from object and ptr-to-member function    
  Button playButton(makeFunctor( (Functor0 *)0, cd, &CDPlayer::play )); 
  Button stopButton(makeFunctor( (Functor0 *)0, cd, &CDPlayer::stop )); 
  
  // --- makeFunctor from ptr-to-function     
  Button wowButton(makeFunctor( (Functor0 *)0, &wow));    
    
  // --- call callback function     
  playButton.click();   // calls cd.play();    
  stopButton.click();   // calls cd.stop();    
  wowButton.click();   // calls wow();
}
