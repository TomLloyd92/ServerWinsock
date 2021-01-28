/*
	Initialise Winsock
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include<stdio.h>
#include<winsock2.h>
#include <String>
#pragma comment(lib,"ws2_32.lib") //Winsock Library


enum Packet
{
	P_ChatMessage,
	P_Test

};



class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);	//Set to false to not allow other network to connect
	bool ListenForNewConnection();


private:
	bool sendall(int ID, char* data, int totalbytes);
	bool recvall(int ID, char* data, int totalbytes);


	bool SendInt(int ID, int _int);
	bool GetInt(int ID, int& _int);

	bool SendPacketType(int ID, Packet _packettype);
	bool GetPacketType(int ID, Packet& _packettype);

	bool SendString(int ID, std::string& _string);
	bool GetString(int ID, std::string& _string);



	bool ProcessPacket(int ID, Packet _packettype);

	static void ClientHandlerThread(int ID);


private:
	//Amount of Connections
	SOCKET Connections[100];
	int TotalConnections = 0;

	SOCKADDR_IN addr;								//Address that will bind our listening socket to
	int addrlen = sizeof(addr);						// length of address

	SOCKET sListen;

};

static Server* serverptr;	//Server poiner neccessary so static Client Handler method can access the server