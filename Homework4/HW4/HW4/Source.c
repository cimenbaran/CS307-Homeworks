#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
int main() {
	clock_t begin_time;
	begin_time = clock();

	char file_name[25] = "loremipsum.txt";
	char buffer[128];
	FILE *fp;

	fp = fopen(file_name, "r");

	unsigned int count = 0;
	
	size_t i;
	while(fgets(buffer, sizeof(buffer), (FILE*)fp)) {
		for(i=0; i < 128; i++) {
			if(buffer[i] == '\0')
				break;
			else if(buffer[i] == 'a') 
				count++;
		}
	}
	fclose(fp);
	printf("%u\n", count);
	
	double time = (double)(clock() - begin_time);
	printf("Time: %f seconds\n", time/CLOCKS_PER_SEC);

	return 0;
}