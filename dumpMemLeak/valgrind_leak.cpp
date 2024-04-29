
#include <iostream>

class CPerson {
public:
  CPerson() {
    int a = 0;
  }

  CPerson(int nBufsize) {
    char* pBuf = new char[nBufsize];
    delete[] pBuf;
    pBuf = nullptr;
  }

  ~CPerson() {
  }
};

struct stPerson {
  int nID;
};

int main() {
  char* pBuf = new char[1024];
  delete[] pBuf;
  pBuf = nullptr;
  
  CPerson* pPerson1 = new CPerson;
  delete pPerson1;
  pPerson1 = nullptr;
  
  CPerson* pPerson2 = new CPerson(100);
  delete pPerson2;
  pPerson2 = nullptr;

  stPerson* pPersonArray = new stPerson[10];
  delete[] pPersonArray;
  pPersonArray = nullptr;

  return 0;
}