
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

int main()
{
	clock_t begin_time;
	begin_time = clock();
	
	


	unsigned int count = 0;
	ifstream file("loremipsum.txt");
	string line;
	while (getline(file, line)) {
		for(int i=0; i < line.length(); i++) {
			if(line[i] == 'a') {
				count++;
			}
		}
	}
	file.close();
	cout << count << endl;

	double time = double(clock() - begin_time);
	cout << "Time: " << double(time)/CLOCKS_PER_SEC << " seconds" << endl;

	return 0;

}

