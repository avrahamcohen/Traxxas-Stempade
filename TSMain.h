#ifndef TSMAIN_H_
#define TSMAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG 0
#define HOST_MODE 1 /* Get commands from keyboard */
#define SERVER_MODE !HOST_MODE /* Get commands from client */
#define stop 1500

enum {LEFT=68,RIGHT=67,FORWARD=65,REVERSE=66,
	/*Q*/QUIT=113,/*R*/RESET=114,/*SPACE*/STOP_ESC=32,/*Z*/STOP_SERVO='z'};

void initPArray();
void init();
void helloWorld();
extern void moveLeft(int * channel , int * targetServo, int * servoOffset , int * mainFd);
extern void moveRight(int * channel , int * targetServo, int * servoOffset , int * mainFd);
extern void moveForward(int * channel , int * targetEsc, int * escOffset , int * mainFd);
extern void moveBackward(int * channel , int * targetEsc, int * escOffset , int * mainFd);
extern void stopAll(int * channel , int * targetServo, int * targetEsc , int * mainFd);
extern void stopESC(int * channel , int * targetEsc , int * mainFd);
extern void stopServo(int * channel , int * targetServo, int * mainFd);
extern void quit(int * channel , int * mainFd);
extern void maestroSetTarget(int *fd, unsigned char _channel, unsigned short target);
extern void maestroSetSpeed(int *fd, unsigned char _channel, unsigned short target);
extern void maestroSetAcceleraion(int *fd, unsigned char _channel, unsigned short target);
extern void listen_and_accept();
extern void setTerminal();
extern const int create_socket();

void (*carOpArray[7])();
char ch;
int _buffer;
int channel;
int client;
int escOffset;
int mainFd;
int servoOffset;
int targetEsc;
int targetServo;


#endif /* TSMAIN_H_ */
