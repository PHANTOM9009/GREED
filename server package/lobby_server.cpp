#pragma once

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() WSAGetLastError()


#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif
#include<iostream>
#include<string>
#include<Windows.h>
#include<thread>
#include<condition_variable>
#include<deque>
#include<mutex>
#include<algorithm>
#include<vector>
#include<unordered_map>
#include <cstdlib>
#include <ctime>
#include <string>

#include<stdlib.h>

//#include "online_lib2.hpp"
//#include "online_lib2.cpp"

//#define max_player 3 //number of maximum players that can play simultaneously in a lobby

//runs on the port 8080
#define ALLOWED_PLAYERS 4
#define GAME_SERVER_COUNT 1
using namespace std;
string version = "v0.0.1";//version of the current lobby server
class user_credentials
{
public:
	char username[20];
	char password[20];
	
	user_credentials() { }
	user_credentials(string user, string pass)
	{
		strcpy(username, user.c_str());
		strcpy(password, pass.c_str());
	}
};
class user_credentials_array
{
public:
	int length;
	user_credentials arr[50];
};
class server_startup//startup configuration file sent to the game server by the lobby server
{
public:
	char token[20];
	int port;
};
class greet_client
{
public:
	int code;
	user_credentials user_cred;
};
unordered_map<SOCKET, int> socket_pid;//socket to process id map
unordered_map<SOCKET, string> socket_token;//token of every server. changes every time when a new game begins
int max_player;
deque<user_credentials> user_cred;//id and password of the active users.


std::string generateRandomSequence()
{
	const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*+=|?><";
	const int length = 16;

	std::string randomSequence;
	randomSequence.reserve(length);

	// Set the seed for the random number generator


	for (int i = 0; i < length; ++i)
	{
		int randomIndex = std::rand() % characters.length();
		randomSequence += characters[randomIndex];
	}
	return randomSequence;
}


std::string GetLastErrorAsString()
{
	//Get the error message ID, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) {
		return std::string(); //No error message has been recorded
	}

	LPSTR messageBuffer = nullptr;

	//Ask Win32 to give us the string version of that message ID.
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	//Copy the error message into a std::string.
	std::string message(messageBuffer, size);

	//Free the Win32's string's buffer.
	LocalFree(messageBuffer);

	return message;
}
deque<SOCKET> valid_connections;//valid connections which are coming but not given a lobby yet
deque<pair<SOCKET,int>> free_lobby;//pair of socket and number of players in the lobby

deque<sockaddr_storage> valid_connections_ad;//address of the sockets that will be transferred to the game server
unordered_map<SOCKET, int> lobby;//(lobby number,no of players)

unordered_map<SOCKET, int> server_port;//socket and the port at which that server shall run.

class Mutex
{
public:
	mutex m_valid;//mutex for valid_connections
	mutex m_lobby;
	condition_variable is_data;//to check if there is more data to process
	condition_variable is_lobby;
}mm;
Mutex* m=&mm;
class transfer_socket
{
public:
	int length;
	WSAPROTOCOL_INFO  socket_listen[50];
	
};

void listener()
{
	/*this function will listen to the incoming socket requests and only forwards them to the next section only if the requests are valid
	valid requests are those whose username and password is correct*/
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	struct addrinfo* bind_address;
	getaddrinfo(0, "8080", &hints, &bind_address);

	SOCKET socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
	if (!ISVALIDSOCKET(socket_listen))
	{
		cout << "\n socket not created=>" << GETSOCKETERRNO();
	}
	int option = 0;
	if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&option, sizeof(option)))//for accepting ipv6 as well
	{
		cout << "\n problem in setting the flag==>";
	}
	//cout << "\n binding the socket==>";
	if (bind(socket_listen, (const sockaddr*)bind_address->ai_addr, (int)bind_address->ai_addrlen))
	{
		cout << "\n failed to bind the socket==>" << GETSOCKETERRNO()<<"\n"<<GetLastErrorAsString();
	}

	if (listen(socket_listen, 20) < 0)
	{
		cout << "\n socket failed";
	}
	freeaddrinfo(bind_address);
	fd_set master;//master set containing all the sockets
	FD_ZERO(&master);
	FD_SET(socket_listen, &master);
	SOCKET max_socket = socket_listen;
	deque<pair<SOCKET,int>> temp_socket;//temporary holder of new incoming connections
	int count = 0;
	fd_set reads;
	FD_ZERO(&reads);
	cout << "\n listening for the other players..";
	while (1)
	{
		count++;
		
		reads = master;
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		select(max_socket + 1, &reads, 0, 0, &timeout);
		for (int i = 1; i <= max_socket; i++)
		{
			if (FD_ISSET(i, &reads))
			{
				if (i == socket_listen)
				{
					struct sockaddr_storage client_address;
					socklen_t client_len = sizeof(client_address);
					SOCKET socket_ = accept(socket_listen, (sockaddr*)&client_address, &client_len);
					temp_socket.push_back(pair<SOCKET, int>(socket_, count));

					FD_SET(socket_, &master);
					if (socket_ > max_socket)
					{
						max_socket = socket_;
					}

					//}				
					//else//asking for username and passowrd
					//{
				}
				else
				{
					user_credentials cred;
					int bytes = recv(i, (char*)&cred, sizeof(cred), 0);
					if (bytes < 1)
					{
						cout << "\n could not recv bytes from the incoming client=>" << GetLastErrorAsString();
						cout << GETSOCKETERRNO();

					}
					if (bytes > 0)
					{
						
						if(strcmp(cred.password,"password")==0)//put the condition if the current user is verified or not
						{
							unique_lock<mutex> lk(m->m_valid);
							valid_connections.push_back(i);
							user_cred.push_back(user_credentials(cred.username, cred.password));
							m->is_data.notify_one();
							cout << "\n connected with a valid player...";
							FD_CLR(i, &master);
						}
						else//if the user is not authenticated then tear down the socket connection and remove it from the set of master
						{
							cout << "\n invalid user came..";
							CLOSESOCKET(i);				

						}
					}

					//}
				}
				
			}

		}
	}
}
bool comparator(pair<SOCKET, int>& one, pair<SOCKET, int>& two)
{
	if (one.second >= two.second)
	{
		return true;
	}
	return false;
	

}
void lobby_contact(vector<SOCKET> &sockets)//sockets are the socket connection to the game servers
{
	fd_set master;
	FD_ZERO(&master);
	SOCKET max_socket = 0;
	for (int i = 0; i < sockets.size(); i++)
	{
		FD_SET(sockets[i], &master);
		if (sockets[i] > max_socket)
		{
			max_socket = sockets[i];
		}
	}
	fd_set reads;
	FD_ZERO(&reads);
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	while (1)
	{
		reads = master;
	
		select(max_socket + 1, &reads, 0, 0, &timeout);
		for (int i = 1;i<=max_socket; i++)
		{
			if (FD_ISSET(i, &reads))
			{
				int data;//pair of socket,bool(in int)
				int byes = recv(i, (char*)&data, sizeof(data), 0);
				
				if (byes > 1)
				{
					cout << "\n received data is==>" << data;
					if (data == 0)//0 means that the server is busy
					{
						for (int j = 0; j < free_lobby.size(); j++)
						{
							if (free_lobby[j].first == i)
							{
								unique_lock<mutex> lk(m->m_lobby);
								auto it = free_lobby.begin();
								advance(it, j);
								free_lobby.erase(it);
								break;
							}
						}
					}
					else if (data == 1)
					{
						unique_lock<mutex> lk(m->m_lobby);
						free_lobby.push_back(pair<SOCKET, int>(i, 0));
						cout << "\n received available message from game server=>" << i;
						//re assigning the token to the server for security purpose
						
					}
				}
			}
		}
	}
}
void transferSocket(deque<SOCKET>& player_queue, deque<user_credentials> &player_cred,const int st,const int end, SOCKET& recvr)//potential problem in this when the number of players in a game lobby increases
{
	//pid is the process id of the receive process
	cout << "\n sending data to the connected client";
	int  port = server_port[recvr];
	server_startup start;
	//we are sending the token of the game, and the port of the game server
	strcpy(start.token, socket_token[recvr].c_str());
	start.port = port;
	for (int i = st; i <= end; i++)
	{
	
		int bytes = send(player_queue[i],(char*)&start, sizeof(start), 0);
		if (bytes < 1)
		{
			cout << "\n couldnt send bytes==>" << GetLastErrorAsString();
		}
	}
	//close all the client sockets, since now there is no use of all those sockets
	for (int i = st; i <= end; i++)
	{
		CLOSESOCKET(player_queue[i]);
	}
		
	//now sendint the recvr server the credentials of the incoming user
	/*
	//preparing the packet
	int length = end - st + 1;
	user_credentials_array ob;
	ob.length = length;
	int j = 0;
	for (int i = st; i <= end; i++)
	{
		ob.arr[j] = player_cred[i];
		j++;
	}
	int bytes = send(recvr, (char*)&ob, sizeof(ob), 0);
	if (bytes < 1)
	{
		cout << "\n couldnt send bytes to the game server(i was sending credentials)==>" << GetLastErrorAsString();
	}
	*/
	
	/*
	//transfering the sockets
		//pid is the process id of the receive process
	const int num = end - st + 1;
	transfer_socket ob;
	if (num <= 100)
	{
		ob.len = num;
	}
	else
	{
		ob.len = 50;
	} 
	int j = 0;
	WSAPROTOCOL_INFO protocolInfo;
	for (int i = st; i < end; i++)
	{
		if (WSADuplicateSocket(player_queue[i],socket_pid[recvr], &protocolInfo) != 0)
		{
			std::cerr << "Failed to duplicate socket handle: " << WSAGetLastError() << std::endl;
			cout << GetLastErrorAsString();
		}
		ob.socket_listen[i] = protocolInfo;
	}
	int bytes = send(recvr, (char*)&ob, sizeof(ob), 0);
	cout << "\n sockets sent to process=>" << socket_pid[recvr];
	//transfering the sockets
	*/

		
}
void assign_lobby()//to assign the lobby to the incoming authenticated connections
{
	deque<SOCKET> player_queue;
	deque<user_credentials> player_cred;
	while (1)
	{
		unique_lock<mutex> lk(m->m_valid);
		m->is_data.wait(lk, [] {return !valid_connections.empty(); });
		
		//enter only when the there are connections asking for the lobby
		
		for (int i = 0; i < valid_connections.size(); i++)
		{
			player_queue.push_back(valid_connections[i]);
			player_cred.push_back(user_cred[i]);
		}
		valid_connections.clear();
		user_cred.clear();
		lk.unlock();

		unique_lock<mutex> lk1(m->m_lobby);
		m->is_lobby.wait(lk1, [] { return !free_lobby.empty(); });
		//now queue has the connections to whom we will provide the lobbies
		//assuming now we know the status of the lobbies and are stored in free_lobbies dequeue
		//	lk1.unlock();
		sort(free_lobby.begin(), free_lobby.end(), comparator);
		int si = free_lobby.size();
		for(int i=0;i<free_lobby.size();i++)
		{
			if (player_queue.size()  <= max_player-free_lobby[i].second)//game server has more requirment
			{
				//transfer all the sockets to that particular process
				//....
				transferSocket(player_queue,player_cred,0, player_queue.size() - 1,free_lobby[i].first);
			//	lk1.lock();
				free_lobby[i].second += player_queue.size();
				//lk1.unlock();
				player_queue.clear();//clearing the player queue
				player_cred.clear();

				if(max_player==free_lobby[i].second)//if supply and demand are equal then remove the game server from free_lobby
				{
					auto it = free_lobby.begin();
					advance(it, i);
					free_lobby.erase(it);
					i--;
				}
								
			}
			else
			{
				//transfer first "max_player-free_lobby[i].second" sockets to the process
				//,....
				//removing the first max_player-free_lobby[i].second sockets
				transferSocket(player_queue, player_cred, 0,max_player-free_lobby[i].second-1, free_lobby[i].first);
				for (int j = 1; j <= (max_player - free_lobby[i].second); j++)
				{
					player_queue.pop_front();
					player_cred.pop_front();
				}
				//remove the lobby from free lobbies queue
				auto it = free_lobby.begin();
				advance(it, i);
				free_lobby.erase(it);
				i--;
				
			}
			i++;
		}

	}
}
int main()
{
	cout << "\n WELCOME TO GREED!\n This is the community version of OFFLINE GREED. \n allowed number of game servers=>1\n maxiumum number of players in a single game server=>4";
	cout << "\n if you want to host bigger offline competitions over GREED visit the website ai_playgrounds.in to know more!";
	cout << "\n lobby server is currently at the version-->" << version;
#if defined(_WIN32)
	WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d))
	{
		cout << "\n failed to initialize";
	}
#endif // defined
	//we need 2 connections to game servers

	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	
	cout << "\n input the number of players=>";
	cin >> max_player;
	max_player = min(ALLOWED_PLAYERS,max_player);
	cout << "\n number of players playing in this game are==>" << max_player;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	struct addrinfo* bind_address;
	getaddrinfo(0, "8080", &hints, &bind_address);//this server is running on port 8080

	SOCKET socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
	if (!ISVALIDSOCKET(socket_listen))
	{
		cout << "\n socket not created=>" << GETSOCKETERRNO();
	}
	/*
	int option = 0;
	if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&option, sizeof(option)))//for accepting ipv6 as well
	{
		cout << "\n problem in setting the flag==>";
	}
	*/
	//cout << "\n binding the socket==>";
	if (bind(socket_listen, (const sockaddr*)bind_address->ai_addr, (int)bind_address->ai_addrlen))
	{
		cout << "\n failed to bind the socket==>" << GETSOCKETERRNO();
	}
	if (listen(socket_listen, 20) < 0)
	{
		cout << "\n socket failed";
	}
	freeaddrinfo(bind_address);
	fd_set master;//master set containing all the sockets
	FD_ZERO(&master);
	FD_SET(socket_listen, &master);
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	SOCKET max_socket = socket_listen;
	
	int start_port = 8081;
	cout << "\n start your game server at the local ip address..";
	while (lobby.size() < GAME_SERVER_COUNT)
	{ 
		fd_set reads;
		FD_ZERO(&reads);
		reads = master;
		select(max_socket + 1, &reads, 0, 0, &timeout);
		if (FD_ISSET(socket_listen, &reads))
		{

			struct sockaddr_storage client_address;
			socklen_t client_len = sizeof(client_address);
			SOCKET socket_ = accept(socket_listen, (sockaddr*)&client_address, &client_len);
			lobby[socket_] = 0;
			server_port[socket_] = start_port;
			cout << "\n connected to a game server";
			//sending the port number to the server:(at which port will the server listen for the clients)
			
			
			socket_token[socket_] = generateRandomSequence();//generated random sequence
			server_startup start;
			strcpy(start.token, socket_token[socket_].c_str());
			start.port = start_port;
			
			int bytes = send(socket_, (char*)&start, sizeof(start), 0);
			if (bytes < 0)
			{
				cout << "\n did not send the port to the server=>" << GetLastErrorAsString();
			}
			start_port+=4;//8081 to  8084 will be used by one game server.
			
			/*
			//receiving the process id for the process
			int process_id;
			int bytes = recv(socket_, (char*)&process_id, sizeof(process_id), 0);
			if (bytes < 0)
			{
				cout << "\n coulndnt recv the process id from the server==>" << GetLastErrorAsString();
			}
			socket_pid[socket_] = process_id;
			*/
		}
				
	}
	int count = 0;
	FD_ZERO(&master);
	max_socket = 0;
	for (int i = 0; i < lobby.size(); i++)
	{
		auto it = lobby.begin();
		advance(it, i);
		FD_SET(it->first,&master);
		if (it->first > max_socket)
		{
			max_socket = it->first;
		}
	}

	
	
	vector<SOCKET> socks;
	for (auto it : lobby)
	{
		free_lobby.push_back(pair<SOCKET, int>(it.first,0));
		socks.push_back(it.first);
	}

	for (auto it : lobby)
	{
		cout << it.first << " " << it.second << endl;
	}
	CLOSESOCKET(socket_listen);
	thread t1(listener);
	thread t2(assign_lobby);
	thread t3(lobby_contact, ref(socks));
	
	t1.join();
	t2.join();
	t3.join();
	while(1)
	{ }
}










