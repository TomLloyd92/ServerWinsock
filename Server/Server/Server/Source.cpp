#include "Server.h"


void ClientHandlerThread(int ID) //ID = the index in the socket connections array
{

}


int main()
{
	Server MyServer(1111, false); //Not broadcasting publically

	//Upto 100 Connections
	for (int i = 0; i < 100; i++)
	{
		MyServer.ListenForNewConnection();
	}
	

	system("pause");
	return 0;


}
