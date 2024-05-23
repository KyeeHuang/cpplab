#include <iostream>
#include <mutex>
#include <thread>

class Counter {
public:
  Counter() : s(0), w(0) {};
  Counter(int _s, int _w) : s(_s), w(_w) {}; 
  int s;
  int w;
};

template <class T>
class Weak_Ptr;

template <class T>
class Shared_Ptr {
public:
  Shared_Ptr (T* ptr = nullptr) : _pPtr(ptr), _pRefCount(new Counter(1, 0)), _pMutex(new std::mutex()) {}

  ~Shared_Ptr () { Release(); }

  Shared_Ptr (const Shared_Ptr<T>& sp) : _pPtr(sp._pPtr), _pRefCount(sp._pRefCount), _pMutex(sp._pMutex) {
    AddRefCount();
  } 

  Shared_Ptr (Weak_Ptr<T> const &w) : _pPtr(w._pPtr), _pRefCount(w._pRefCount), _pMutex(w._pMutex) {
    _pRefCount->s++;
  }

  Shared_Ptr<T>& operator=(const Shared_Ptr<T>& sp) {
    if (_pPtr != sp._pPtr) {
      Release();
      _pPtr = sp._pPtr;
      _pRefCount->s = sp._pRefCount->s;
      _pMutex = sp._pMutex;
      AddRefCount();
    }

    return *this;
  }

  T& operator*() {
    return *_pPtr;
  }
  
  T* operator->() {
    return _pPtr;
  }

  int UseCount() { return *_pRefCount->s; }
  T* Get() { return _pPtr; }
  void AddRefCount() {
    _pMutex->lock();
    ++(*_pRefCount->s); 
    _pMutex->unlock();
  }

  friend class Weak_Ptr<T>;

private:
  void Release() {
    bool deleteflag = false;
    _pMutex->lock();
    if (--(*_pRefCount->s) == 0) {
      delete _pRefCount;
      delete _pPtr;
      deleteflag = true;
    }
    _pMutex->unlock();
    if (deleteflag)
      delete _pMutex;
  }

private:
  Counter *_pRefCount;
  T* _pPtr;
  std::mutex* _pMutex;
};

template <class T>
class Weak_Ptr {
public:
  Weak_Ptr(T* ptr = nullptr) : _pPtr(ptr), _pRefCount(new Counter(0, 0)), _pMutex(new std::mutex()) {}
  
  Weak_Ptr(Shared_Ptr<T>& sp) : _pPtr(sp._pPtr), _pRefCount(sp._pRefCount), _pMutex(sp._pMutex) {
    _pMutex->lock();
    _pRefCount->w++;
    _pMutex->unlock();
  }

  Weak_Ptr(Weak_Ptr<T>& wp) : _pPtr(wp._pPtr), _pRefCount(wp._pRefCount), _pMutex(wp._pMutex) {
    _pMutex->lock();
    _pRefCount->w++;
    _pMutex->unlock();
  } 

  ~Weak_Ptr() {
    Release();
  }

  Weak_Ptr<T>& operator=(Weak_Ptr<T> & wp) {
    if (this != wp) {
      Release();
      _pRefCount = wp._pRefCount;
      _pMutex->lock();
      _pRefCount->w++;
      _pMutex->unlock();
      _pPtr = wp._pPtr;
    }
    return *this;
  }

  Weak_Ptr<T>& operator=(Shared_Ptr<T>& sp) {
    Release();
    _pRefCount = sp._pRefCount;
    _pMutex->lock();
    _pRefCount->w++;
    _pMutex->unlock();
    _pPtr = sp._pPtr;
    return *this;
  }

  Shared_Ptr<T> lock() {
    return Shared_Ptr(*this);
  }

  bool expired() {
    if (_pRefCount) {
      if (_pRefCount->s > 0) {
        std::cout << "empty " << std::endl;
        return false;
      }
    }

    return true;
  }

  friend class Shared_Ptr<T>;

private:
  void Release() {
    if (_pRefCount) {
      bool deleteflag = false;
      _pMutex->lock();
      if (--(_pRefCount->w < 1) && (_pRefCount->s) < 1) {
        delete _pRefCount;
        delete _pPtr;
        deleteflag = true;
      }
      _pMutex->unlock();

      if (deleteflag) delete _pMutex;
    }
  }


private:
  Counter* _pRefCount;
  T* _pPtr;
  std::mutex* _pMutex;
};