#pragma once
#include <atomic>

struct AtomicNode {
  volatile std::atomic<AtomicNode*> _next;
  void* data; 
};

class AtomicQueue {
  volatile std::atomic<AtomicNode*> _tail;
  volatile std::atomic<AtomicNode*> _head;
public:
  AtomicQueue();
  ~AtomicQueue();
  
  void Enqueue(AtomicNode* node) {
  }

  AtomicNode* Dequeue() {}
};