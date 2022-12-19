#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h> //hostname->IP
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h> //verificare conectivitate port
#include <ctype.h>
#include <unistd.h>

int main(int argc , char *argv[])
{
	struct hostent *host; //server dns
	int start , end;
	char hostname[100];
	struct sockaddr_in sa; //conectare socket
	
	// Hostname
	printf("Enter hostname or IP : ");
	scanf("%s", &hostname);
	
	//Port start
	printf("Enter start port number : ");
	scanf("%d" , &start);
	
	//Port end 
	printf("Enter end port number : ");
	scanf("%d" , &end);

	//Initializare structura sockaddr
	strncpy((char*)&sa , "" , sizeof(sa));
	sa.sin_family = AF_INET; //IPv4
	
	//Daca s-a introdus adresa IP
	if(isdigit(hostname[0]))
	{
		printf("inet_address...");
		sa.sin_addr.s_addr = inet_addr(hostname);
		printf("Done\n");
	}
	//Daca s-a introdus nume de domeniu, translatare in adresa IP
	else if( (host = gethostbyname(hostname)) != 0)
	{
		printf("Translating hostname...");
		strncpy((char*)&sa.sin_addr , (char*)host->h_addr , sizeof(sa.sin_addr));
		printf("Done\n %s\n", (char*)host->h_addr);
	}
	else
	{
		printf("Error on translating hostname.");
		herror(hostname);
		exit(2);
	}
	
	//Start scanning
	int sock, err;
	printf("Starting the portscan loop : \n");
	for( int i = start ; i <= end ; i++) 
	{
		//Fill in the port number
		sa.sin_port = htons(i); //little-endian in memorie
		//Creare port de tip Internet
		sock = socket(AF_INET , SOCK_STREAM , 0);
		
		//Verificare creare socket
		if(sock < 0) 
		{
			perror("\nSocket");
			exit(1);
		}

		//Conectare- folosind socketul si structura sockaddr
		err = connect(sock , (struct sockaddr*)&sa , sizeof sa);

		//Conectare reusita
		if( err >= 0 )
		{
			printf("%-5d open\n",  i);
		}
		//Eroare conectare
		/*else
		{
			printf("%s %-5d %s\r\n" , hostname , i, strerror(errno));
			fflush(stdout);
		}*/
		close(sock);
	}
	
	printf("\r");
	fflush(stdout);
	return(0);
}