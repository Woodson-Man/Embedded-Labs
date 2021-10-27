//Use g++ joystick.cc -std=c++11 -o Lab4EX3B Lab4EX3B.cpp

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "joystick.hh"
#include <cstdlib>
#define  PORT 8080
using namespace std;

#include <wiringPi.h>
#include <wiringSerial.h>

int createSocket();

int sock = 0, valread;

void movement(int, int);


int main(int argc, char const *argv[])
{

	//Open the file stream for the joystick
	Joystick joystick("/dev/input/js0");
	JoystickEvent event;

	if(!joystick.isFound()){
		cout << "Error opening joystick" << endl;
		exit(1);
	}


	//Create the connection to the server
	createSocket();

	while(true)
	{

		/*Sample the events from the joystick*/
		if (joystick.sample(&event))
		{
			if (event.isButton())
			{
				printf("isButton: %u | Value: %d\n", event.number, event.value);
				/*Interpret the joystick input and use that input to move the Kobuki*/
				switch(event.number)
				{
					case 7:
							movement(0,0);
					break;
					
					case 8:
						if(event.value) cout << ""; //send close command 
						else continue;	
					break;
				}
				movement(0,0);

			}
			if (event.isAxis())
			{
				printf("isAxis: %u | Value: %d\n", event.number, event.value);
				/*Interpret the joystick input and use that input to move the Kobuki*/

				switch(event.number)
				{
					case 7:
						if(event.value < 0)
						{
							movement(250, 0);
						}
						else if(event.value > 0)
						{
							movement(-250, 0);
						}
						
					break;
					
					case 6:
						if(event.value > 0)
						{
							movement(181, -1);
						}
						
						else if(event.value > 0)
						{
							movement(181, 1);
						}
					break;
				}
					
			}
		}
				
		/*Convert the event to a useable data type so it can be sent*/
		connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

		/*Print the data stream to the terminal*/

		/*Send the data to the server*/
	

		/*if() 
		{
		//Closes out of all connections cleanly

		//When you need to close out of the connection, please
		//close TTP/IP data streams.
		//Not doing so will result in the need to restart
		//the raspberry pi and Kobuki
			close(sock);
			exit(0);

		Set a delay
		}*/

	return 0;
	}
}

//Creates the connection between the client and
//the server with the controller being the client
int createSocket(){
	struct sockaddr_in address;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\nSocket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(PORT);

	/*Use the IP address of the server you are connecting to*/
	if(inet_pton(AF_INET, "10.227.59.225" , &serv_addr.sin_addr) <= 0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}

	send(sock, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);

	return 0;
}
