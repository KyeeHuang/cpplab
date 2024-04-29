#include <iostream>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <stdio.h>

typedef struct clMemHeader {
  struct clMemHeader* pBlockHeaderNext;
  struct clMemHeader* pBlockHeaderPrev;

  const char* szFileName;
  int nLine;
  size_t nDataSize;
} clMemHeader;

clMemHeader* pFirstHead = nullptr;
clMemHeader* pLastHead = nullptr;

void* alloc_mem(size_t nSize, const char* lpszFileName, int nLine) {
  size_t nTotalSize = sizeof(clMemHeader) + nSize;
  clMemHeader* pHead = (clMemHeader*) malloc(nTotalSize);

  if (pFirstHead) {
    pFirstHead->pBlockHeaderPrev = pHead;
  } else {
    pLastHead = pHead;
  } 

  pHead->pBlockHeaderNext = pFirstHead;
  pHead->pBlockHeaderPrev = nullptr;
  pHead->szFileName = lpszFileName;
  pHead->nLine = nLine;
  pHead->nDataSize = nSize;

  pFirstHead = pHead;

  void* pRet = ((clMemHeader*) pHead) + 1;
  return pRet;
}

void* operator new (size_t nSize, const char* lpszFileName, int nLine) {
  return alloc_mem(nSize, lpszFileName, nLine);
}

void* operator new[] (size_t nSize, const char* lpszFileName, int nLine) {
  return alloc_mem(nSize, lpszFileName, nLine);
}

void free_mem(void* pUserData) {
  clMemHeader* pHead = (clMemHeader*)((char*)pUserData - sizeof(clMemHeader));
  
  if (pHead->pBlockHeaderNext) {
    pHead->pBlockHeaderNext->pBlockHeaderPrev = pHead->pBlockHeaderPrev;
  } else {
    pLastHead = pHead->pBlockHeaderPrev;
  }

  if (pHead->pBlockHeaderPrev) {
    pHead->pBlockHeaderPrev->pBlockHeaderNext = pHead->pBlockHeaderNext;
  } else {
    pFirstHead = pHead->pBlockHeaderPrev;
  }

  free(pHead);
}

void operator delete (void* p) {
  free_mem(p);
}

// 与new 的参数列表需要一致
void operator delete (void* p, const char* lpszFileName, int nLine) {
  free_mem(p);
}

void _dumpMemLeak() {
  std::cout << "*********************************" << std::endl;
  std::cout << "Detect memory leaks" << std::endl;

  char buf[1024] = {0};
  
  for (clMemHeader* pHead = pFirstHead; pHead != NULL; pHead = pHead->pBlockHeaderNext) {
    std::memset(buf, 0, 1024);
    std::sprintf(buf, "%s(%d) leak %d bytes at %p",
      pHead->szFileName, pHead->nLine, pHead->nDataSize, ((clMemHeader*)pHead + 1));
        
    std::cout << buf << std::endl; 
  }

  std::cout << "*********************************" << std::endl;
}



