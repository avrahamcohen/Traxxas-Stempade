#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

enum {min=1000,stop=1500,max=2000}; //Pos min2max

void moveLeft(int * channel , int * targetServo, int * servoOffset , int * mainFd);
void moveRight(int * channel , int * targetServo, int * servoOffset , int * mainFd);
void moveForward(int * channel , int * targetEsc, int * escOffset , int * mainFd);
void moveBackward(int * channel , int * targetEsc, int * escOffset , int * mainFd);
void stopAll(int * channel , int * targetServo, int * targetEsc , int * mainFd);
void stopESC(int * channel , int * targetEsc , int * mainFd);
void stopServo(int * channel , int * targetServo, int * mainFd);
void quit(int * channel , int * mainFd);
void maestroSetTarget(int *fd, unsigned char _channel, unsigned short target);
void maestroSetSpeed(int *fd, unsigned char _channel, unsigned short target);
void maestroSetAcceleraion(int *fd, unsigned char _channel, unsigned short target);
extern void restoreTerminal();

void moveLeft(int * channel , int * targetServo, int * servoOffset , int * mainFd){
			(*channel)=5; (*targetServo)+=(*servoOffset);
			if((*targetServo)>max) (*targetServo)=max;
#if DEBUG
			printf("LEFT %d\n",(*targetServo));
#endif
			maestroSetTarget(mainFd,(unsigned char)(*channel),(*targetServo));
}

void moveRight(int * channel , int * targetServo, int * servoOffset , int * mainFd){
			(*channel)=5; (*targetServo)-=(*servoOffset);
			if((*targetServo)<min) (*targetServo)=min;
#if DEBUG
			printf("RIGHT %d\n",(*targetServo));
#endif
			maestroSetTarget(mainFd,(unsigned char)(*channel),(*targetServo));
}

void moveForward(int * channel , int * targetEsc, int * escOffset , int * mainFd){
			(*channel)=0; (*targetEsc)+=(*escOffset);
			if((*targetEsc)>max) (*targetEsc)=max;
#if DEBUG
			printf("FORWARD %d\n",(*targetEsc));
#endif
			maestroSetTarget(mainFd,(unsigned char)(*channel),(*targetEsc));
}

void moveBackward(int * channel , int * targetEsc, int * escOffset , int * mainFd){
			(*channel)=0; (*targetEsc)-=(*escOffset);
			if((*targetEsc)<min) (*targetEsc)=min;
#if DEBUG
			printf("REVERSE %d\n",(*targetEsc));
#endif
			maestroSetTarget(mainFd,(unsigned char)(*channel),(*targetEsc));
}

void stopAll(int * channel , int * targetServo, int * targetEsc , int * mainFd){
			(*channel)=0; (*targetEsc)=stop; (*targetServo)=stop;
#if DEBUG
			printf("RESET %d,%d\n",(*targetServo),(*targetEsc));
#endif
			maestroSetTarget(mainFd,(unsigned char)(*channel),(*targetEsc));
			maestroSetTarget(mainFd,(unsigned char)(*channel)+5,(*targetServo));
}

void stopESC(int * channel , int * targetEsc , int * mainFd){
			(*channel)=0; (*targetEsc)=stop;
#if DEBUG
			printf("STOP_ESC %d\n",(*targetEsc));
#endif
			maestroSetTarget(mainFd,(unsigned char)(*channel),(*targetEsc));
}

void stopServo(int * channel , int * targetServo, int * mainFd){
			(*channel)=5; (*targetServo)=stop;
#if DEBUG
			printf("STOP_SERVO %d\n",(*targetEsc));
#endif
			maestroSetTarget(mainFd,(unsigned char)(*channel),(*targetServo));
}

void quit(int * channel , int * mainFd){
#if SERVER_MODE
			printf("Closing socket\n");
			close_socket();
#endif
#if DEBUG
			printf("QUIT\n");
#endif
			(*channel)=0;
			maestroSetTarget(mainFd,(unsigned char)(*channel),stop);
			maestroSetTarget(mainFd,(unsigned char)(*channel)+5,stop);
			restoreTerminal();
			exit(0);
}

void maestroSetTarget(int * fd, unsigned char channel, unsigned short target){
	char command[4];
	int iores;
	target *= 4;
	/*Compact protocol: 0x84, channel number, speed low bits, speed high bits*/
	command[0] = 0x84;
	command[1] = channel;
	command[2] = target & 0x7F;
	command[3] = (target >> 7) & 0x7F;
	iores = write(*fd, &command, sizeof(command));
	if(iores < 0){
		printf("I/O Error!\n");
		close(*fd);
		exit(1);
	}
}

void maestroSetSpeed(int *fd, unsigned char channel, unsigned short target){
	char command[4];
	int iores;
	target *= 4;
	/*Compact protocol: 0x87, channel number, speed low bits, speed high bits*/
	command[0] = 0x87;
	command[1] = channel;
	command[2] = target & 0x0;
	command[3] = (target >> 7) & 0x0;
	iores = write(*fd, &command, sizeof(command));
	if(iores < 0){
		printf("I/O Error!\n");
		close(*fd);
		exit(1);
	}
}

void maestroSetAcceleraion(int *fd, unsigned char channel, unsigned short target){
	char command[4];
	int iores;
	target *= 4;
	/*Compact protocol: 0x89, channel number, speed low bits, speed high bits*/
	command[0] = 0x89;
	command[1] = channel;
	command[2] = target & 0x0;
	command[3] = (target >> 7) & 0x0;
	iores = write(*fd, &command, sizeof(command));
	if(iores < 0){
		printf("I/O Error!\n");
		close(*fd);
		exit(1);
	}
}

