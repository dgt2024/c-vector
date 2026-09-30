#include "cpp.h"
int main() {
	Vector x;
	VecInit(&x);
	x.push_back(toVoidPtr(12));
	x.push_back(toVoidPtr(15));
	x.push_back(toVoidPtr(17));
	printf("%d\n", fromVoidPtr(int, x.first()));
	printf("%d\n", fromVoidPtr(int, x.last()));
	printf("%d\n", fromVoidPtr(int, x.at(1)));
	Vector y;
	VecInit(&y);
	y.push_back(toVoidPtr(3.4f));
	printf("%f\n", fromVoidPtr(float, y.at(0)));
	printf("%d\n", fromVoidPtr(int, x.at(2)));
}
