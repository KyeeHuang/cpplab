#include <bits/types/clock_t.h>
#include <iostream>
#include <cassert>
#include <ctime>
#include <memory>
#include <vector>

#include "StackAlloc.hpp"

#define ELEMS 25000000 
#define REPS  50 

int main() {
    clock_t start;

    StackAlloc<int, std::allocator<int>> stackDefault;
    start = clock();
    for (int j = 0; j < REPS; j++) {
        assert(stackDefault.empty());
        for (int i =0; i < ELEMS; i++)
            stackDefault.push(i);
        for (int i =0; i < ELEMS; i++)
            stackDefault.pop();
    }

    std::cout << "Default Allocator Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";
    
    return 0;
}
