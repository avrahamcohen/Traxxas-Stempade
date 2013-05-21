/*
	Author : Tomy Sela Meron & Avraham Cohen .
	Program Description : This is a program to control Micro Maestro 6 channel
	using linux glibc library.

	-- Project Description :
	 this is a simple program that controls the Maestro Micro Controller,
	You can use keyboard arrow's to control the car , R key for reset &,
	Q key is for quitting the program & SPACE for stopping the esc.
	NOTE : use lowercase letters only
	Enjoy ...

	-- compile with : gcc * or Eclipse!
*/

#include "TSMain.h"

int main(void) {
	init();
	initPArray();

	if(SERVER_MODE){
		if (0 == create_socket())
			printf("Server is alive and waiting for client connection.\n");
		listen_and_accept();
	}

	while(1){
	if(HOST_MODE){
		ch = getchar();
		switch(ch){
		case RIGHT:{printf("RIGHT\n"); carOpArray[4](&channel , &targetServo, &servoOffset , &mainFd);}
			break;
		case LEFT:{printf("LEFT\n"); carOpArray[3](&channel , &targetServo, &servoOffset , &mainFd);}
			break;
		case FORWARD:{printf("FORWARD\n"); carOpArray[1](&channel , &targetEsc, &escOffset , &mainFd);}
			break;
		case REVERSE:{printf("REVERSE\n"); carOpArray[2](&channel , &targetEsc, &escOffset , &mainFd);}
			break;
		case STOP_ESC:{printf("STOP_ESC\n"); carOpArray[6](&channel , &targetEsc , &mainFd);}
			break;
		case STOP_SERVO:{printf("STOP_SERVO\n"); carOpArray[7](&channel , &targetServo, &mainFd);}
			break;
		case RESET:{printf("RESET\n"); carOpArray[5](&channel , &targetServo, &targetEsc , &mainFd);}
			break;
		case QUIT:{printf("QUIT\n"); carOpArray[8](&channel , &mainFd);}
			break;
		default: puts("Input Error :-(");
			break;
		}
	}

	if(SERVER_MODE){
		if(0>=read(client, &_buffer, 1)){
			printf("Unable to get data from client .. closing\n");
			carOpArray[8]();
		}
#if DEBUG
		printf("buffer %d\n",_buffer);
#endif
		carOpArray[_buffer]();
	}

	}
	return EXIT_SUCCESS;
}

void initPArray(){
	carOpArray[1]=&moveForward;
	carOpArray[2]=&moveBackward;
	carOpArray[3]=&moveLeft;
	carOpArray[4]=&moveRight;
	carOpArray[5]=&stopAll;
	carOpArray[6]=&stopESC;
	carOpArray[7]=&stopServo;
	carOpArray[8]=&quit;
}

/* Init First Value's For Channel's */
void init(){
	int target = 1500;
	int channel_0 = 0;
	int channel_5 = 5;
	channel=0;
	targetEsc=stop;
	targetServo=stop;
	escOffset=25; //Let's change it to 5 to slow motion
	servoOffset=50; //Let's change it to 5 to slow motion

	fflush(stdout);
	setTerminal(&mainFd);

	/* Init Channel 0 & 5 To Target 1500 */
	maestroSetTarget(&mainFd,channel_0,target);
	maestroSetTarget(&mainFd,channel_5,target);
	target = 0;
	/* Init Channel 0 & 5 To Speed 0 */
	maestroSetSpeed(&mainFd,channel_0,target);
	maestroSetSpeed(&mainFd,channel_5,target);
	/*Init Channel 0 & 5 To Acceleration 0 */
	maestroSetAcceleraion(&mainFd,channel_0,target);
	maestroSetAcceleraion(&mainFd,channel_5,target);

}
