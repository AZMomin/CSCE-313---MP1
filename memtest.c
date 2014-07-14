#include <stdio.h>
#include "my_allocator.h"

int main(int argc, char ** argv) {
	int a = init_allocator(4, 10000);
	printf("%s %d\n", "Total memory allocated" ,a );
	printf("%s %d\n", "Total blocks" ,a/4 );
	print_freeList();
	check_list();
	print_freeList();
	return 0;
}
