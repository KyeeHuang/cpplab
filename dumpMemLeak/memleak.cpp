#include <iostream>
#include "memleak.h"

#define new new(__FILE__, __LINE__)

class CPerson {
private:
  char* pBuf;

public:
  CPerson() {
    int a = 0;
  }

  CPerson(int nBufsize) {
    pBuf = new char[nBufsize];
  }

  ~CPerson() {
    delete pBuf;
    pBuf = nullptr;
  }
};

struct stPerson {
  int nID;
};

int main() {
  char* pBuf = new char[1024];
  
  CPerson* pPerson1 = new CPerson;
  delete pPerson1;
  pPerson1 = nullptr;
  
  CPerson* pPerson2 = new CPerson(100);
  delete pPerson2;
  pPerson2 = nullptr;

  // stPerson* pPersonArray = new stPerson[10];

  _dumpMemLeak();

  return 0;
}