#include <algorithm>
#include <stdexcept>
#include <vector>
#include <iostream>

using namespace std;

class MaxHeap {
private:
    int cur_size;
    vector<int> _heap;

public:
    MaxHeap() {}
    MaxHeap(vector<int>& nums) : cur_size(nums.size()), _heap(nums.size()) {
        for (int i = 0; i < cur_size; i++) {
            _heap[i] = nums[i];
        }

        for (int i = cur_size / 2; i >= 0; i--) {
            maxHeapify(i, cur_size);
        }
    }

    void maxHeapify(int i, int len) {
        for (; (i << 1) + 1 <= len; ) {
            int lson = (i << 1) + 1;
            int rson = (i << 1) + 2;
            int large;
            if (lson <= len && _heap[lson] > _heap[i]) {
                large = lson;
            } else {
                large = i;
            }

            if (rson <= len && _heap[rson] > _heap[large]) {
                large = rson;
            }

            if (large != i) {
                swap(_heap[large], _heap[i]);
                i = large;
            } else {
                break;
            }
        }
    }

    void push(int value) {
        if (cur_size == _heap.size()-1) _heap.resize(cur_size * 2);
        
        int hole_pos = ++cur_size;
        for (; hole_pos > 1 && value > _heap[hole_pos / 2]; hole_pos /= 2) {
            _heap[hole_pos] = std::move(_heap[hole_pos/2]);
        }
        _heap[hole_pos] = std::move(value);
    }

    void pop() {
        _heap[0] = std::move(_heap[cur_size--]);
        maxHeapify(0, cur_size);
    }

    int top() {
        if (cur_size > 0) return _heap[0];
        throw runtime_error("heap is empty");
    }
};

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> nums(n);

    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }

    vector<int> nums_k(nums.begin(), nums.begin()+k);
    MaxHeap maxheap(nums_k);

    for (int i = k; i < n; i++) {
        maxheap.push(nums[i]);
        maxheap.maxHeapify(0, k);
        maxheap.pop();
    }

    cout << maxheap.top() << endl;

    return 0;
}
