#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
int main() {

	clock_t begin_time;
	begin_time = clock();

	int f = open("loremipsum.txt", O_RDONLY);
	struct stat s;
	size_t size;
	int status = fstat(f, &s);
	size = s.st_size;

	char *ptr = mmap(0, size, PROT_READ, MAP_PRIVATE, f, 0);

	if(ptr == MAP_FAILED) {
		printf("Mapping Failed\n");
		return 1;
	}

	close(f);
	
	
	unsigned int count = 0;
	size_t i;
	for(i=0; i < size; i++) {
		if(ptr[i] == 'a') {
			count++;
		}
	}

	printf("%u\n",count);

	status = munmap(ptr, size);
	if(status != 0) {
		printf("Unmapping Failed\n");
		return 1;
	}

	double time = (double)(clock() - begin_time);
	printf("Time: %f seconds\n", time/CLOCKS_PER_SEC);

	return 0;
}