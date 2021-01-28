#include "Server.h"


Server::Server(int PORT, bool BroadcastPublically)
{
	//Set up WinSock
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData) != 0) //Error if not 0
	{
		MessageBoxA(NULL, "Winsock startup failed!", "ERROR", MB_OK | MB_ICONERROR);
		exit(0);
	}

	if (BroadcastPublically)
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");	//Locally broadcast
	}

	addr.sin_port = htons(PORT);					//Port
	addr.sin_family = AF_INET;						//IPv4 Socket

	sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen to connection



	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error";
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	serverptr = this;
}

bool Server::ListenForNewConnection()
{
	SOCKET newConnection;					 //Socket to hold the clients connection

	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept new connection

	if (newConnection == 0)
	{
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else
	{
		std::cout << "Client Connected" << std::endl;
		Connections[TotalConnections] = newConnection;

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(TotalConnections), NULL, NULL);	//Create thread
		std::string MOTD = "Welcome to hell";
		SendString(TotalConnections, MOTD);

		TotalConnections++;
		return true;
	}
}

bool Server::ProcessPacket(int ID, Packet _packettype)
{
	switch (_packettype)
	{
	case P_ChatMessage:
	{
		std::string Message;
		if (!GetString(ID, Message))
		{
			return false;
		}

		for (int i = 0; i < TotalConnections; i++)
		{
			if (i == ID)
			{
				continue;
			}
			if (!SendString(i, Message))
			{
				std::cout << "Failed to send message to Client ID: " << ID << std::endl;
			}

		}
		std::cout << "Processed chat message packet from ID:" << ID << "To : " << std::endl;
		break;

	}
	default:
		std::cout << "Unrecognised packet" << std::endl;
		break;
	}
	return true;
}

void Server::ClientHandlerThread(int ID)
{
	Packet packettype;
	while (true)
	{
		if (!serverptr->GetPacketType(ID, packettype))
			break;
		if (!serverptr->ProcessPacket(ID, packettype))
			break;
	}
	std::cout << "Lost connection to client :" << ID << std::endl;
	closesocket(serverptr->Connections[ID]);
}