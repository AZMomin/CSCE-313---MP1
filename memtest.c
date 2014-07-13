#include <iostream>
#include "my_allocator.h"

using namespace std;
int main(int argc, char ** argv) {
	int a = init_allocator(1, 15);
	cout << a << endl;

	return 0;
}
