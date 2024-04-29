#include <vector>
#include <iostream>

using namespace std;

class MinHeap {
private:
  int cur_size;
  vector<int> _heap;

public:
  MinHeap() {}
  MinHeap(vector<int> & nums) : cur_size(nums.size()), _heap(nums.size()) {
    for (int i = 0; i < cur_size; i++) {
      _heap[i] = nums[i];
    }

    for (int i = cur_size / 2; i >= 0; i--) {
      minHeapify(i, cur_size);
    }
  }

  void minHeapify(int i, int len) {
    for (; (i << 1) + 1 <= len; ) {
      int lson = (i << 1) + 1;
      int rson = (i << 1) + 2;
      int small;
      if (lson <= len && _heap[lson] < _heap[i]) {
        small = lson;
      } else {
        small = i;
      }
      if (rson <= len && _heap[rson] < _heap[small]) {
        small = rson;
      }

      if (small != i) {
        swap(_heap[small], _heap[i]);
        i = small;
      } else {
        break;
      }
    }
  }

  void push(int value) {
    if (value > _heap[0]) {
      _heap[0] = value;
      minHeapify(0, cur_size);
    } else {
      return;
    }
  }

  int top() {
    if (_heap.size() > 0) 
      return _heap[0];
  }

};



