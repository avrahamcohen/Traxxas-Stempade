#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define PORT 4500
#define NUM_OF_CONNECTIONS 1
int socket_channel;
struct sockaddr_in serv_name;
int client;
unsigned int buffer;
socklen_t len;
struct termios stdinTerminalConfig;

const int create_socket();
void listen_and_accept();
void setTerminal();
void openPortAndsetBaudRate();
void configTerminal();
void restoreTerminal();

const int create_socket() {
	socket_channel = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_channel < 0) {
		perror("Error opening channel");
		restoreTerminal();
		close(socket_channel);
		exit(1);
	}
	bzero(&serv_name, sizeof(serv_name));
	serv_name.sin_family = AF_INET;
	serv_name.sin_port = htons(PORT);

	if (bind(socket_channel, (struct sockaddr *) &serv_name, sizeof(serv_name)) < 0) {
		perror("Error naming channel");
		restoreTerminal();
		close(socket_channel);
		exit(1);
	}
	len = sizeof(struct sockaddr_in);

	return 0;
}

void listen_and_accept() {
#if DEBUG
		printf("Listening to socket ... \n");
#endif
		listen(socket_channel,NUM_OF_CONNECTIONS);
		client = accept(socket_channel,(struct sockaddr *) &serv_name, &len);
#if DEBUG
		printf("Socket number is %d\n", client);
#endif
}

void close_socket(){
#if DEBUG
		printf("Closing socket number is %d\n", client);
		close(client);
#endif
}

void setTerminal(int * mainFd){
	configTerminal();
	//openPortAndsetBaudRate(mainFd);
}

/* Open the Maestro's serial port. */
/* Choose the baud rate (bits per second).*/
void openPortAndsetBaudRate(int * mainFd){
	struct termios options;
	(*mainFd) = open("/dev/ttyACM0",O_RDWR|O_NOCTTY);
	if ((*mainFd) == -1) {
		printf("Unable to open port /dev/ttyACM0\n");
		restoreTerminal();
		exit(1);
	}
	//Define the serial port to communicate with MM
	tcgetattr((*mainFd), &options);
	options.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	options.c_cc[VMIN] = 2;
	tcsetattr((*mainFd), TCSANOW, &options);
#if DEBUG
	printf("Port open successfully :)\n");
#endif
}

/* Config the STDIN serial port. */
void configTerminal(){
	int oldf;
	struct termios newt;
	tcgetattr(STDIN_FILENO, &stdinTerminalConfig);
	newt = stdinTerminalConfig;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
}

/* Restore the STDIN serial port. */
void restoreTerminal(){
	int oldf;
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	tcsetattr(STDIN_FILENO, TCSANOW, &stdinTerminalConfig);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
}
