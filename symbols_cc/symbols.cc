#include <iostream>

int gdata1 = 1;
int gdata2;
const int g_var1 = 1;
const int g_var2 = gdata1;

int fun_add(int a, int b) {
	const int local_var = 1;
	return a + b;
}

int main() {
	int a = 11;
	const char *p = "hello world";

	return 0;
}