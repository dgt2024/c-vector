#include "cpp.h"

int main() {
	Vector x;
	VecInit(&x);
	x.push_back(12);
	x.push_back(39);
	x.push_back(21);
	printf("Size = %ld\n", x.size());
	printf("x[0] = %d\n", fromVoidPtr(int, x.at(0)));
	printf("x[1] = %d\n", fromVoidPtr(int, x.at(1)));
	printf("x[2] = %d\n", fromVoidPtr(int, x.at(2)));
	x.pop_back();
	printf("Size = %ld\n", x.size());
	printf("x[0] = %d\n", fromVoidPtr(int, x.at(0)));
	printf("x[1] = %d\n", fromVoidPtr(int, x.at(1)));
}
