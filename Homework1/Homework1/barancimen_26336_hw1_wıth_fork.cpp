#include <stdio.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

using namespace std;

short turn;

int matrix[2][50];
int remainingSeats = 100;

void thread1() {
	while(remainingSeats != 0) {
		// do step A
		int seatNum = rand() % 100; // number between 0 and 99
		int row = (seatNum / 50);
		int column = seatNum - row*50;

		while(turn != 1 && remainingSeats != 0) {};
		// do step B

		printf("Agency 1 Entered Critical Region\n");

		if(matrix[row][column] == 0) { // Empty seat
			printf("Seat Number %d is reserved by Agency 1\n", seatNum+1);
			remainingSeats--;
		}

		printf("Agency 1 Exit Critical Region\n\n");
		turn = 2;
		
	}
}


void thread2() {
	while(remainingSeats != 0) {
		// do step A
		int seatNum = rand() % 100; // number between 0 and 99
		int row = (seatNum / 50);
		int column = seatNum - row*50;

		while(turn != 2 && remainingSeats != 0) {};
		// do step B

		printf("Agency 2 Entered Critical Region\n");

		if(matrix[row][column] == 0) { // Empty seat
			printf("Seat Number %d is reserved by Agency 2\n", seatNum+1);
			remainingSeats--;
		}

		printf("Agency 2 Exit Critical Region\n\n");
		turn = 1;
	}
}


int main() {
	// seed creation
	srand(time(NULL));

	// matrix is filled with 0's
	for(int i=0; i<2; i++) {
		for(int j=0; j<50; j++) {
			matrix[i][j] = 0;
		}
	}


	pid_t returnedValue1 = fork();
	// New process is created
	if( returnedValue1 == 0 )
	{
		// Child 1
		thread1();
	}
	else 
	{
		// Parent

		pid_t returnedValue2 = fork();
		// New process is created
		if( returnedValue2 == 0 )
		{
			// Child 2
			thread2();
		}
		else 
		{
			// Parent, Also Main Thread
			while( remainingSeats != 0 ) {};
			printf("No Seats Left\n\n");

			printf("Plane is full:\n\n"); 
			for(int i=0; i<2; i++) {
				for(int j=0; j<50; j++) {
					printf("%d", matrix[i][j]);
				}
				printf("\n");
			}
			
		}


	}


}
