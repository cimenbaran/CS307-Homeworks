#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <pthread.h>

using namespace std;

int turn = 1;

int matrix[2][50];
int remainingSeats = 100;

void * thread1(void * ) {
	while( remainingSeats != 0 ) {
		// do step A
		int seatNum = rand() % 100; // number between 0 and 99
		int row = (seatNum / 50);
		int column = seatNum - row*50;

		while(turn != 1) {};
		// do step B

		printf("Agency 1 Entered Critical Region\n");

		if( remainingSeats != 0 && matrix[row][column] == 0) { // Empty seat
			matrix[row][column] = 1;
			remainingSeats -= 1;
			printf("Seat Number %d is reserved by Agency 1\n", (seatNum+1));
		}

		printf("Agency 1 Exit Critical Region\n\n");
		turn = 2;
	}
	return NULL; 
}


void * thread2(void * ) {
	while( remainingSeats != 0 ) {
		// do step A
		int seatNum = rand() % 100; // number between 0 and 99
		int row = (seatNum / 50);
		int column = seatNum - row*50;

		while(turn != 2) {};
		// do step B
		
		printf("Agency 2 Entered Critical Region\n");
		
		if( remainingSeats != 0 && matrix[row][column] == 0) { // Empty seat
			matrix[row][column] = 2;
			remainingSeats -= 1;
			printf("Seat Number %d is reserved by Agency 2\n", (seatNum+1));
		}

		printf("Agency 2 Exit Critical Region\n\n");
		turn = 1;
	}
	return NULL; 
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

	pthread_t thread_1, thread_2;

	pthread_create(&thread_1, NULL, thread1, NULL);
	// New process is created

	pthread_create(&thread_2, NULL, thread2, NULL);
	// New process is created

	
	// Main Thread

	// Wait until threads finish
	while( remainingSeats != 0 ) {};

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	printf("No Seats Left\n\nPlane is full:\n\n");
	for(int i=0; i<2; i++) {
		for(int j=0; j<50; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	
	return 0;
}
