#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <signal.h>
#include <stdio.h>

using namespace std;

	  #define SIGINT 2
	  #define SIGKILL 9
	  #define SIGQUIT 3
	//SOCKETS
	  SOCKET sock,client;

void s_handle(int s)
{
	if(sock)
	   closesocket(sock);
	if(client)
	   closesocket(client);
	WSACleanup();
	cout<<"EXIT SIGNAL :"<<s;
	exit(0);
}


void s_cl(char *a, int x)
{
	cout<<a;
	s_handle(x+1000);
}

void main()
{
	//Declarations
	int res,i=1,port=100;
	char buf[100];
	WSADATA data;

	signal(SIGINT,s_handle);
	signal(SIGKILL,s_handle);
	signal(SIGQUIT,s_handle);

	cout<<"\t\tEcho Client";


	sockaddr_in ser;
	sockaddr addr;

	ser.sin_family=AF_INET;
	ser.sin_port=htons(port);						//Set the port
	ser.sin_addr.s_addr=inet_addr("127.0.0.1");		//Set the address we want to connect to

	memcpy(&addr,&ser,sizeof(SOCKADDR_IN));

	res = WSAStartup(MAKEWORD(1,1),&data);			//Start Winsock
	cout<<"\n\nWSAStartup"
		<<"\nVersion: "<<data.wVersion
		<<"\nDescription: "<<data.szDescription
		<<"\nStatus: "<<data.szSystemStatus<<endl;

	if(res != 0)
		s_cl("WSAStarup failed",WSAGetLastError());

	sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);		//Create the socket
		if(sock==INVALID_SOCKET )
			s_cl("Invalid Socket ",WSAGetLastError());
		else if(sock==SOCKET_ERROR)
			s_cl("Socket Error)",WSAGetLastError());
		else
			cout<<"Socket Established"<<endl;

	res=connect(sock,&addr,sizeof(addr));				//Connect to the server
		if(res !=0 )
		{
			s_cl("SERVER UNAVAILABLE",res);
		}
		else
		{
			cout<<"\nConnected to Server: ";
			memcpy(&ser,&addr,sizeof(SOCKADDR));
		}

	while(true)
	{
		cout<<"\n>";
		gets(buf);

		res =  send(sock,buf,100,0);					//Send Data

		if(res==0)
		{
			//0==other side terminated conn
			printf("\nSERVER terminated connection\n");
			closesocket(client);
			client =0;
			break;
		}
		else if(res==SOCKET_ERROR)
		{
			//-1 == send error
			printf("Socket error\n");
			s_handle(res);
			break;
		}


	res=recv(sock,buf,100,0);							//Recive Data

	if(res>0)
	{
		cout<<"\nRecieved string:"<<buf;
	}

  }
	WSACleanup();
}
