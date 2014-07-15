#include <stdio.h>
#include "my_allocator.h"

int main(int argc, char ** argv) {
	int a = init_allocator(4, 100000);
	/*printf("%s %d\n", "Total memory allocated" ,a );
	printf("%s %d\n", "Total blocks" ,a/4 );
	//print_freeList();
	//check_list();
	//print_freeList();
	void * b = my_malloc(16000);
	void * d = my_malloc(16000);
	void * e = my_malloc(16000);
	void * f = my_malloc(16000);
	void * c = my_malloc(81000);
	int x = my_free(b);
	int y = my_free(d);
	int r = my_free(e);
	int t = my_free(f);
	int p = my_free(c);*/

  ackerman_main();
}
