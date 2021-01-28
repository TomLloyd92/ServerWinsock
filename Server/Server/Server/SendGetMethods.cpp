
#include "Server.h"

bool Server::recvall(int ID, char* data, int totalbytes)
{
	int bytesrecived = 0;
	while (bytesrecived < totalbytes)
	{
		int RetnCheck = recv(Connections[ID], data + bytesrecived, totalbytes - bytesrecived, NULL);
		if (RetnCheck == SOCKET_ERROR)
		{
			return false;
		}
		bytesrecived += RetnCheck;
	}
	return true;	//Great Successs
}

bool Server::sendall(int ID, char* data, int totalbytes)
{
	int bytessent = 0;
	while (bytessent < totalbytes)
	{
		int RetnCheck = send(Connections[ID], data + bytessent, totalbytes - bytessent, NULL);	//TEST THIS
		if (RetnCheck == SOCKET_ERROR)
		{
			return false;
		}
		bytessent += RetnCheck;
	}
	return true;	//Success
}

bool Server::SendInt(int ID, int _int)
{
	if (!sendall(ID, (char*)&_int, sizeof(int)))
	{
		return false;
	}
	return true;
}

bool Server::GetInt(int ID, int& t_int)
{
	if (!recvall(ID, (char*)&t_int, sizeof(int)))
	{
		return false;
	}
	return true;
}

bool Server::SendPacketType(int ID, Packet t_packettype)
{
	if (!sendall(ID, (char*)&t_packettype, sizeof(Packet)))
	{
		return false;
	}
	return true;
}

bool Server::SendString(int ID, std::string& t_string)
{
	if (!SendPacketType(ID, P_ChatMessage))
	{
		return false;	//String send failed
	}
	int bufferLength = t_string.size();

	if (!SendInt(ID, bufferLength))
	{
		return false;
	}
	if (!sendall(ID, (char*)t_string.c_str(), bufferLength))
	{
		return false;
	}
	return true;	//String Successfully sent
}

bool Server::GetString(int ID, std::string& t_string)
{
	int bufferLength;
	if (!GetInt(ID, bufferLength))
	{
		return false;
	}
	char* buffer = new char[bufferLength + 1];
	buffer[bufferLength] = '\0';

	if (!recvall(ID, buffer, bufferLength))
	{
		delete[] buffer;
		return false;
	}

	t_string = buffer;
	delete[] buffer;
	return true;
}

bool Server::GetPacketType(int ID, Packet& t_packettype)
{
	if (!recvall(ID, (char*)&t_packettype, sizeof(Packet)))
	{
		return false;
	}
	return true;
}