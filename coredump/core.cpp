#include <iostream>
using namespace std;

void swap(int *px, int *py) {
    int tmp = *px;
    *px = *py;
    *py = tmp;
}

int main() {
    int a = 1;
    int b = 2;
    int c = a + b;
    cout << a << ' ' << b << ' ' << c << endl;

    swap(&a, &b);
    
    cout << a << ' ' << b << ' ' << c << endl;

    int *p = nullptr;
    *p = 0;
    
    return 0;
}
