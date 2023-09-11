/*this version is the final version of the game server, this doesn't show the graphics, only does the computation*/
/*how to run this shit
start lobby_server
start server_v3
start client_v1
starting client_v1 will automatically start the client_v2 unit so no need to start it manually

*/
#pragma once
#include<SFML/Graphics.hpp>
#include<conio.h>

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
#include<ctime>
#include "online_lib2.hpp"
#include "online_lib2.cpp"

#include "sqlite3.h" 
#include<stdlib.h>
#define ALLOWED_MAX_PLAYER 4
int max_player=0;
string my_token;//token of the current game instance
string version = "v0.0.1";
/*
* 1. a function which connects the server with the clients and assign each client's ship an id
* 2. after all required clients are connected properly or the time of connection is up, send all the connected clients initial configuration of the game
* 3. when all the clients have received the initial configuration, send message to every client to start thier functions
* 4. This gets the game running
*/
vector<int> socket_display;//depracated we dont use it anymore, we are using an unordered_map for this
unordered_map<int,sockaddr_storage> socket_id_display;
unordered_map<int, user_credentials> user_cred;


deque<pair<int,recv_data>> terminal_data;//data queue to send data to the client terminal unit..pair of id and data
deque<pair<int, top_layer>> display_data;//data for the display unit of the client: pair of id and top_layer data

SOCKET socket_listen;//UDP socket to send data to the client terminal unit
SOCKET socket_listen2;//UDP socket to send data to the client display unit
SOCKET recver;//UDP socket to recv data from the client terminal unit

SOCKET lobby_socket;//TCP  socket to talk to the lobby server

bool gameOver = false;//making it public so that the running theads of chaseShip1 and nav_data_processor can check its status and closes themselves.

sqlite3* db;
char* zErrMsg = 0;
int rc = sqlite3_open("Greed.db", &db);


/* Open database */
bool user_found = false;
static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	/*
	* this thing is running for every row, argc is the number of columns in the database,
	* argv is the value at that column
	* azColName is the name of the column
	*/
	if (argc > 0)
	{
		user_found = true;
	}
	return 0;

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




bool SENDTO(SOCKET sock, char* buff, int length,sockaddr* addr, int tolen)
{
	/*overload of send function of UDP having the mechanism of resending and ack*/
	fd_set master;
	FD_ZERO(&master);
	FD_SET(sock, &master);
	fd_set read;
	FD_ZERO(&read);
	chrono::steady_clock::time_point now = chrono::steady_clock::now();

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	int b = sendto(sock, buff, length, 0,addr,tolen);
	if (b < 1)
	{
		cout << "\n cannot send the bytes.." << GETSOCKETERRNO();
		return false;
	}
	else if (b > 0)
	{
		cout << "\n from SENDTO: sent the bytes to the destination successfully..";
	}
	while (1)
	{
		read = master;
		select(sock + 1, &read, 0, 0, &timeout);
		if (FD_ISSET(sock, &read))
		{
			int ack;
			struct sockaddr_storage addr1;
			socklen_t fromlen = sizeof(addr1);
			int bytes = recvfrom(sock, (char*)&ack, sizeof(ack), 0, (sockaddr*)&addr1, &fromlen);
			if (bytes < 1)
			{
				cout << "\n cannot recv the ack=>" << GetLastErrorAsString();
				return false;
			}
			if (ack == 1)
			{
				cout << "\n from SENDTO: recveived the ack..";
				return true;
			}
		}
		auto end = chrono::steady_clock::now();
		chrono::duration<double> iteration_time = chrono::duration_cast<chrono::duration<double>>(end - now);
		if (iteration_time.count() > 1)
		{
			//resend the packet
			int bytes = sendto(sock, buff, length, 0, addr, tolen);
			if (bytes < 1)
			{
				cout << "\n cannot send the bytes again=>" << GETSOCKETERRNO();
				return false;
			}
			else
			{
				cout << "\n from SENDTO: sent the bytes again to the destination..apparently the ack did not come.";
			}
			now = chrono::steady_clock::now();//changing the time since the last message sent
		}

	}
}
bool RECVFROM(SOCKET sock, char* buff, int length,sockaddr* addr,int tolen)
{
	/*overload for UDP recv, to recv the data carefully*/
	while (1)
	{		
		int b = recvfrom(sock, buff, length, 0,addr,&tolen);
		if (b > 1)
		{
			int ack = 1;
			int bytes = sendto(sock, (char*)&ack, sizeof(ack), 0,addr,tolen);
			if (bytes < 1)
			{
				cout << "\n cannot send the bytes.. " << GETSOCKETERRNO();
				return false;
			}
			return true;
		}
	}

}
class transfer_socket
{
public:
	int length;
	WSAPROTOCOL_INFO protocolInfo[100];

};
class debug
{
public:
	double avg_processing1;
	double avg_processing2;
	double avg_sending;
	double avg_receiving;
	double fire_upgrade;
	double navigation_ship;
	double fire_ship;
	double fire_cannon;
	double avg_send1;//for sending to the client terminal
	double avg_send2;//for sending to the client display unit

	debug()
	{

	}
	debug(double avg_pro1,double avg_pro2,double fire_up,double nav_ship,double fire_s,double fire_c,double avg_send,double avg_rec,double a,double b)
	{
		avg_send1 = a;
		avg_send2 = b;
		avg_processing1 = avg_pro1;
		avg_processing2 = avg_pro2;
		fire_upgrade = fire_up;
		avg_sending = avg_send;
		avg_receiving = avg_rec;

		navigation_ship = nav_ship;
		fire_ship = fire_s;
		fire_cannon = fire_c;
	}
};
void control1::nav_data_processor(deque<ship*>& pl1, Mutex* mutx)
{
	int cur_frame = -1;
	int next_frame = -1;
	double elapsed_time = 0;
	sf::Clock clock;
	while (1)
	{
	
		sf::Time time = clock.restart();
		elapsed_time += time.asSeconds();
		next_frame = elapsed_time * 30;
		if (elapsed_time > 1)
		{
			elapsed_time = 0;
		}
		if (next_frame != cur_frame)
		{
			cur_frame = next_frame;
			unique_lock<mutex> lk(mutx->gameOver_check);
			if (gameOver == true)
			{
				return;
			}
			lk.unlock();
			if (mutx->updating_data.try_lock())
			{
			
				for (int i = 0; i < pl1.size(); i++)
				{
					if (pl1[i]->died == 1)
					{
						continue;
					}

					if (pl1[i]->nav_data.size() > 0)
					{
						if (pl1[i]->nav_data[0].type == 0)//for target type
						{
							if (pl1[i]->nav_data[0].target.r != -1 && pl1[i]->nav_data[0].target.c != -1)
							{
								Greed::path_attribute path = pl1[i]->setTarget(pl1[i]->nav_data[0].target);
								pl1[i]->setPath(path.getPath());
								//	cout << "\n in type 0 in target=>" << pl1[i]->nav_data[0].target.r << " " << pl1[i]->nav_data[0].target.c;
							}
							else if (pl1[i]->nav_data[0].s_id != -1 && pl1[pl1[i]->nav_data[0].s_id]->getDiedStatus() == 0)
							{
								Greed::path_attribute path = pl1[i]->setTarget(pl1[i]->nav_data[0].s_id);
								pl1[i]->setPath(path.getPath());
								cout << "\n in type 0 for ship_id=>" << pl1[i]->nav_data[0].s_id;
							}

						}
						else if (pl1[i]->nav_data[0].type == 1 && pl1[i]->nav_data[0].n > 0 && pl1[i]->nav_data[0].dir != Direction::NA)
						{
							pl1[i]->sail(pl1[i]->nav_data[0].dir, pl1[i]->nav_data[0].n);
							cout << "\n in sail";
						}
						else if (pl1[i]->nav_data[0].type == 2 && pl1[i]->nav_data[0].s_id >= 0 && pl1[pl1[i]->nav_data[0].s_id]->getDiedStatus()==0)
						{
							pl1[i]->chaseShip(pl1[i]->nav_data[0].s_id);
								//cout << "\n " << i << " ship is chasing=>" << pl1[i]->nav_data[0].s_id;
							
						}
						else if (pl1[i]->nav_data[0].type == 3)
						{
							pl1[i]->anchorShip();
						}
						pl1[i]->nav_data.pop_front();

					}

				}
				
				mutx->updating_data.unlock();
			}
		}
	}
	
}
void connector(vector<int>& socks, unordered_map<int, int>& sockets_id, int n)//n is the number of clients we are expecting to be connected the clients connected in reality may be lot less
{
	//accepting the connections

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;

	hints.ai_flags = AI_PASSIVE;


	struct addrinfo* bind_address;
	getaddrinfo(0, "8080", &hints, &bind_address);
	SOCKET socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
	if (!ISVALIDSOCKET(socket_listen))
	{
		cout << "\n socket not created==>" << GETSOCKETERRNO();
	}

	int option = 0;
	if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&option, sizeof(option)))//for accepting ipv6 as well
	{
		cout << "\n problem in setting the flag==>";
	}
	int yes = 1;
	if (setsockopt(socket_listen, IPPROTO_TCP, TCP_NODELAY, (char*)&yes, sizeof(yes)) < 0) //disabling nagle's algorithm for speed in sending the data
	{
		fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
	}


	cout << "\n binding the socket==>";
	if (bind(socket_listen, (const sockaddr*)bind_address->ai_addr, (int)bind_address->ai_addrlen))
	{
		cout << "\n failed to bind the socket==>" << GETSOCKETERRNO();
	}

	if (listen(socket_listen, 20) < 0)
	{
		cout << "\n socket failed";
	}
	freeaddrinfo(bind_address);
	int count = 0;
	SOCKET max_socket = socket_listen;
	FD_SET master;
	FD_ZERO(&master);
	FD_SET(recver, &master);
	while (count < n)
	{
		FD_SET reads;
		reads = master;
		select(max_socket + 1, &reads, 0, 0, 0);
		if (FD_ISSET(socket_listen, &reads))
		{
			struct sockaddr_storage client_address;
			socklen_t client_len = sizeof(client_address);
			SOCKET socket_listen = 0;
			socket_listen=accept(socket_listen, (struct sockaddr*)&client_address, &client_len);
			socks.push_back(socket_listen);
			sockets_id[socket_listen] = count;
			cout << "\n connection established for client=>" << count;
			count++;

		}
	}
	CLOSESOCKET(socket_listen);
	
}

//making the connector number 2 to display the game to the client
void connector_show(vector<int>& socks, unordered_map<int,sockaddr_storage>& sockets_id, int n)//n is the number of clients we are expecting to be connected the clients connected in reality may be lot less
{

	int count = 0;
	SOCKET max_socket = socket_listen;
	fd_set master;
	FD_ZERO(&master);
	FD_SET(socket_listen, &master);
	while (count < n)
	{
		FD_SET reads;
		reads = master;
		select(max_socket + 1, &reads, 0, 0, 0);
		if (FD_ISSET(socket_listen, &reads))
		{
			struct sockaddr_storage client_address;
			socklen_t client_len = sizeof(client_address);
			char buff[100];
			
			int bytes = recvfrom(socket_listen, buff, sizeof(buff), 0, (sockaddr*)&client_address, &client_len);
			sockets_id[count] = client_address;
			
			cout << "\n message received from the client=>" << buff;
			count++;

		}
	}
	 
	
}
void send_data_terminal(unordered_map<int, sockaddr_storage> addr_info, Mutex* m)
{
	while (1)
	{
		unique_lock<mutex> lk1(m->gameOver_check);
		if (gameOver)
		{
			cout << "\n breaking from the send_data_terminal..";
			break;
		}
		lk1.unlock();
		unique_lock<mutex> lk(m->send_terminal);
		m->cond_terminal.wait(lk, [] {return !terminal_data.empty(); });
		//terminal_data queue has data now make its copy and clear the queue
		deque<pair<int,recv_data>> data;
		for (int i = 0; i < terminal_data.size(); i++)
		{
			data.push_back(terminal_data[i]);
			
		}
		terminal_data.clear();//clearing the queue
		lk.unlock();
		//now start sending the data over the network to each individual clients..
		for (int i = 0; i < data.size(); i++)
		{
			//send the data to the client
			int bytes = sendto(socket_listen, (char*)&data[i].second, sizeof(data[i].second), 0, (sockaddr*)&addr_info[data[i].first], sizeof(addr_info[data[i].first]));
			if (bytes < 0)
			{
				cout << "\n error in sending the data to the client==>"<<i<<" due to the reason==>"<<GetLastErrorAsString();
			}
			
		}	
	}
}
void send_data_display(unordered_map<int, sockaddr_storage> addr_info, Mutex* m)
{
	
	while (1)
	{
		unique_lock<mutex> lk1(m->gameOver_check);
		if (gameOver)
		{
			cout << "\n breaking from send_data_display";
			break;
		}
		lk1.unlock();
		unique_lock<mutex> lk(m->send_display);
		m->cond_display.wait(lk, [] { return !display_data.empty(); });
		//display_data queue has data now make its copy and clear the queue
		deque<pair<int, top_layer>> data;
		for (int i = 0; i < display_data.size(); i++)
		{
			data.push_back(display_data[i]);
		}
		display_data.clear();
		lk.unlock();
		
		//now sending the data using socket_listen2
		for (int i = 0; i < data.size(); i++)
		{
			int bytes = sendto(socket_listen2, (char*)&data[i].second, sizeof(data[i].second), 0, (sockaddr*)&addr_info[data[i].first], sizeof(addr_info[data[i].first]));
			if (bytes > 0)
			{
				//cout << "\n sent bytes to the client display unit==>" << data[i].first;
			}
			if (bytes < 0)
			{
				cout << "\n error in sending the data to the client display unit==>" << i << " due to the reason==>" << GetLastErrorAsString();
			}
		}
	}
}

bool checkUser(user_credentials& cred)
{
	string username = cred.username;
	string password = cred.password;
	string query = "SELECT * FROM USER_DATA WHERE USERNAME=" + username + " AND PASSWORD=" + password + ";";
	char* zErrMsg = 0;
	int rc = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
	if (user_found == true)
	{
		user_found = false;
		return true;
	}
	return false;
}

void graphics::callable(Mutex* mutx, int code[rows][columns], Map& map_ob, int n,unordered_map<int,sockaddr_storage>& socket_id,vector<int> &socket_display)//taking the ship object so as to access the list of the player
{

	//making a reverse mapping of socket_id
	vector<sockaddr_storage> socket_id_rev(n);
	for (auto it : socket_id)
	{
		socket_id_rev.push_back(it.second);
	}
	
	deque<int> dying_ships;
	int ran = 0;
	this->mutx = mutx;
	long total_time = 0;
	//retriving the ship list from the control class
	Control nm;
	deque<ship*>& pl1 = nm.getShipList(2369);
	List<Greed::cannon>& cannon_list = nm.getCannonList(2369);//retiving the cannon list
	cout << "\n number of cannons are==>" << cannon_list.howMany();
	List<Greed::coords> opaque = nm.getOpaqueCoords();

//working on player now

	sf::Clock clock;
	double elapsed_time = 0;
	int c = 0;
	/*
	sf::View view;
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	view.setCenter(400, 400);
	view.rotate(50);
	window.setView(view);
	*/
	double t2 = 0;
	int frames = 0;
	no_of_chase++;
	Greed::coords* prev = new Greed::coords[pl1.size()];
	for (int i = 0; i < pl1.size(); i++)//setting the previous position
	{
		prev[i] = pl1[i]->tile_pos_front;
		pl1[i]->name = user_cred[i].username;//setting the name of the player
	}
	total_time = 0;
	

	//mover_tgui<tgui::Picture::Ptr> moveit;

	//moveit.moving(window,gui,gui_renderer.firing_icon[0]);
	//write a function to check if a number is even or odd
	deque<int> dead_ship;//id of the dead ships
	
	bool display = true;
	vector<int> winner;//id's of the winner of the game, this is a vector in case of a tie
	int var = 0;
	control1 con;

	fd_set master, read, write;
	FD_ZERO(&master);
	FD_ZERO(&read);
	FD_ZERO(&write);
	FD_SET(recver,&master);
	control1 control;
	thread t(&control1::nav_data_processor, &con, ref(pl1), mutx);
	t.detach();

	thread t1(send_data_terminal, socket_id, mutx);
	t1.detach();

	thread t3(send_data_display, socket_id_display, mutx);
	t3.detach();
		
	List<graphics::animator> animation_list;

	vector<int> checker;

    double avg_send = 0;//total send
	double avg_send1 = 0;//sending to the player terminals
	double avg_send2 = 0;
	double avg_recv = 0;
	double avg_processing = 0;
	double avg_processing2 = 0;
	double avg_rendering = 0;
	double fire_upgrade = 0;
	int total_frames = 0;
	double navigation_ship = 0;
	double fire_ship = 0;
	double fire_cannon = 0;
	//end of the part1 processing:

	int current_frame = -1;
	int next_frame = 0;
	double ep = 0;//for elapsed time for maintaining the frame rate
	sf::Clock clock1;//for maintaining the frame rate 
	vector<int> died;//died ships in the last frame.
	int happen = 0;

	vector<debug> de;

	cout << "\n size of recv_data=>" << sizeof(recv_data);
	cout << "\n size of top player=>" << sizeof(top_layer);

	vector<int> prev_packet_id(n, -1);//previous packet id received from the player so that any old packet may not be use
	int game_over_frame;//at which frame did the game get over
	int checked = 0;
	//checking for the socket connection
	
	int prev_packet_terminal = 0;
	int prev_packet_display = 0;
	//sending the data to the client display to check the connectivity issue
		
	
	while (1)
	{

		sf::Time tt = clock1.restart();
		ep += tt.asSeconds();
		if (ep > 1)
		{
			ep = 0;
		}
		next_frame = ep * 60;

	
		if (next_frame != current_frame)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				break;
			}

			//creating a method for sending data to the clients
			deque<int> client_terminal;
			deque<int> client_display;
			for (int i = 0; i < max_player; i++)
			{
				client_terminal.push_back(i);
				client_display.push_back(i);
			}
			current_frame = next_frame;

			read = master;
			write = master;
			
			struct timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			c++;
			frames++;

			sf::Event event;

			sf::Time deltime = clock.restart();
			elapsed_time += deltime.asSeconds();
			t2 += deltime.asSeconds();
			total_secs += deltime.asSeconds();
			//total_time += deltime.asSeconds();
			total_time++;

			no_of_chase++;
			if (t2 >= 1)
			{
				cout << "\n\n frame rate is==>" << c;
				de.push_back(debug(avg_processing, avg_processing2, fire_upgrade, navigation_ship, fire_ship, fire_cannon, avg_send, avg_recv, avg_send1, avg_send2));
				avg_processing = 0;
				avg_processing2 = 0;
				avg_send = 0;
				avg_recv = 0;
				avg_send1 = 0;
				avg_send2 = 0;
				fire_upgrade = 0;
				navigation_ship = 0;
				fire_ship = 0;
				fire_cannon = 0;

				total_frames++;
				/*
				if (c <= 40 )
				{
					cout << "\n\n previous data is=>\n";
					cout << "\n processing part1=>" << de[de.size() - 1].avg_processing1;
					cout << "\n processing part2=>" << de[de.size() - 1].avg_processing2;
					cout << "\n processing by parts..";
					cout << "\n for fire and upgrade part(1)=>" << de[de.size() - 1].fire_upgrade;
					cout << "\n for navigation ship(part2)=>" << de[de.size() - 1].navigation_ship;
					cout << "\n fire ship (part3)=>" << de[de.size() - 1].fire_ship;
					cout << "\n fire cannon(part4)=>" << de[de.size() - 1].fire_cannon;

					cout << "\n\n total processing time=>" << de[de.size() - 1].avg_processing1 + de[de.size() - 1].avg_processing2;
					cout << "\n sending=>" << de[de.size() - 1].avg_sending;
					cout << "\n receiving=>" << de[de.size() - 1].avg_receiving;
				}
				*/
				c = 0;
				t2 = 0;
			}

			died.clear();
			sf::Clock processing;
			processing.restart();

			sf::Clock processing1;
			processing1.restart();

			if (check_game_over(pl1) || (total_secs/60) >= 10)//if game is over/if time of the game is 10 minutes, then the game will be over.
			{
				if (!checked)
				{
					game_over_frame = total_time;
				}
				checked = 1;
				for (int k = 0; k < pl1.size(); k++)
				{
					unique_lock<mutex> lk(mutx->m[k]);
					pl1[k]->gameOver = true;
				}
				//adding +200 to the score of the winning ship
				for (int i = 0; i < pl1.size(); i++)
				{
					if (pl1[i]->minutes == INT_MAX && pl1[i]->seconds == INT_MAX)
					{
						pl1[i]->score += 200;
					}
				}
				unique_lock<mutex> lk(mutx->gameOver_check);
				gameOver = true;
				lk.unlock();
				
			}
			

				/*code to update the timer*/
				int min = total_secs / 60;
				int sec = (int)total_secs % 60;

				/*code to update the timer ends*/

				//run a thread here which will update the path buffer according to the data received
				//lets run just one thread now
				//thread t(&control1::nav_data_processor, &con, ref(pl1), mutx);
				//t.detach();

				//con.nav_data_processor(pl1, mutx);

				for (int i = 0; i < pl1.size(); i++)
				{
					if (pl1[i]->died == 1)
					{
						continue;
					}
					//upgrading the fuel, health, or anything else
					for (int j = 0; j < pl1[i]->udata.size(); j++)
					{
						if (pl1[i]->udata[j].type == 0)
						{
							pl1[i]->upgradeAmmo(pl1[i]->udata[j].n);//when the user buys something
						}
						else if (pl1[i]->udata[j].type == 1)
						{
							pl1[i]->upgradeHealth(pl1[i]->udata[j].n);
						}
						else if (pl1[i]->udata[j].type == 2)
						{
							pl1[i]->upgradeFuel(pl1[i]->udata[j].n);
						}
					}
					pl1[i]->udata.clear();
					//printing the position of ship one
					

					for (int j = 0; j < pl1[i]->bullet_info.size(); j++)
					{

						if (pl1[i]->bullet_info[j].type == 0)
						{

							if (pl1[i]->bullet_info[j].s_id >= 0 && pl1[i]->bullet_info[j].s_id < pl1.size() && pl1[i]->bullet_info[j].s_id != i && (int)pl1[i]->bullet_info[j].can >= 0 && (int)pl1[i]->bullet_info[j].can <= 1 && (int)pl1[i]->bullet_info[j].s >= 0 && (int)pl1[i]->bullet_info[j].s <= 2)
							{
								pl1[i]->fireCannon(pl1[i]->bullet_info[j].can, pl1[i]->bullet_info[j].s_id, pl1[i]->bullet_info[j].s);

							}
						}
						else if (pl1[i]->bullet_info[j].type == 1)
						{

							if (pl1[i]->bullet_info[j].c_id >= 0 && pl1[i]->bullet_info[j].c_id < cannon_list.howMany() && (int)pl1[i]->bullet_info[j].c_id >= 0 && (int)pl1[i]->bullet_info[j].c_id <= 1)
							{
								pl1[i]->fireAtCannon(pl1[i]->bullet_info[j].c_id, pl1[i]->bullet_info[j].can);

							}
						}
					}
					pl1[i]->bullet_info.clear();
					//updating the cost of the local map of the player
					for (int j = 0; j < pl1[i]->map_cost_data.size(); j++)
					{
						cout << "\n request to udpate the cost came from==>" << i;//just a check statement
						pl1[i]->updateCost(pl1[i]->map_cost_data[j].ob, pl1[i]->map_cost_data[j].new_cost);
					}
					pl1[i]->map_cost_data.clear();
				}
				//till upgrade and fire
				fire_upgrade += processing1.getElapsedTime().asSeconds();
				sf::Clock processing2;
				processing2.restart();
				for (int i = 0; i < pl1.size(); i++)
				{


					//getPointPath has to be protected by a mutex
					if (pl1[i]->died == 1)
					{

						continue;
					}
					//doing the navigation stuff

					for (int j = 0; j < pl1.size(); j++)
					{
						if (i != j && pl1[j]->died == 0)
						{

							if (pl1[i]->collide(j, pl1[i]->tile_pos_front))
							{

								pl1[i]->collided_ships.push_back(j);

								pl1[i]->anchorShip_collision();

							}
						}
					}
					if (mutx->m[i].try_lock())
					{
						List<Greed::abs_pos>& l1 = pl1[i]->getPathList(2369);

						if (pl1[i]->fuel > 0 && l1.howMany() > 0 && l1.howMany() - 1 != pl1[i]->getPointPath(2369) && pl1[i]->tile_path.howMany() > 0)
						{
							// cout << "\n fuel " << i << " now=>" << pl1[i]->fuel;
							pl1[i]->motion = 1;

							pl1[i]->update_pointPath(pl1[i]->getPointPath(2369) + 1, 2369);

							pl1[i]->rect.setPosition(::cx(l1[pl1[i]->getPointPath(2369)].x + origin_x), ::cy(l1[pl1[i]->getPointPath(2369)].y + origin_y));

							pl1[i]->update_tile_pos(l1[pl1[i]->getPointPath(2369)].x, l1[pl1[i]->getPointPath(2369)].y);

							if (prev[i] != pl1[i]->tile_pos_front)//updating the fuel here
							{
								prev[i] = pl1[i]->tile_pos_front;
								pl1[i]->fuel -= pl1[i]->globalMap[pl1[i]->tile_pos_front.r * columns + pl1[i]->tile_pos_front.c].b.cost;
								if (pl1[i]->fuel <= 0)
								{
									mutx->m[i].unlock();
									pl1[i]->anchorShip();
									mutx->m[i].lock();

								}

							}

						}
						else
						{
							pl1[i]->motion = 0;
						}

						mutx->m[i].unlock();
					}
					//navigation part of the ship ends here
					navigation_ship += processing2.getElapsedTime().asSeconds();

					sf::Clock processing3;
					processing3.restart();
					if (pl1[i]->ammo > 0 && pl1[i]->cannon_ob.activeBullets.size() > 0 && frames % 30 == 0)
					{
						frames = 0;
						int val1 = (pl1[i]->cannon_ob.activeBullets.size() - 1);
						int val2 = pl1[i]->bullet_pointer;
						if (val1 > val2)
						{
							pl1[i]->bullet_pointer++;
							Greed::bullet& bull = pl1[i]->cannon_ob.activeBullets[pl1[i]->bullet_pointer];
							if (bull.target_ship != -1)
								pl1[i]->setBullet(bull, bull.can, bull.target_ship, bull.s);
							else if (bull.target_cannon != -1)
							{
								pl1[i]->setBullet_forCannon(bull);
							}
						}
						pl1[i]->isFiring = true;

					}
					else
					{
						pl1[i]->isFiring = false;
					}
					for (int j = 0; j <= pl1[i]->bullet_pointer; j++)
					{


						if (pl1[i]->cannon_ob.activeBullets[j].bullet_trajectory.size() > 0)
						{
							pl1[i]->cannon_ob.activeBullets[j].absolute_position = Greed::abs_pos(pl1[i]->cannon_ob.activeBullets[j].bullet_trajectory[0].x, pl1[i]->cannon_ob.activeBullets[j].bullet_trajectory[0].y);
							pl1[i]->cannon_ob.activeBullets[j].bullet_entity.setPosition(sf::Vector2f(::cx(pl1[i]->cannon_ob.activeBullets[j].bullet_trajectory[0].x + origin_x), ::cy(pl1[i]->cannon_ob.activeBullets[j].bullet_trajectory[0].y + origin_y)));
							for (int l = 1; l <= 3; l++)//this is speed of bullet. skipping 3 pixels per frame, speed is 180 frames(pixels) per sec
							{
								if (pl1[i]->cannon_ob.activeBullets[j].bullet_trajectory.size() > 0)
								{
									pl1[i]->cannon_ob.activeBullets[j].bullet_trajectory.erase(pl1[i]->cannon_ob.activeBullets[j].bullet_trajectory.begin());
								}
							}
						}
						else //when bullet's trajectory is over and nothing happend
						{
							//pl1[i]->cannon_ob.allBullets[pl1[i]->cannon_ob.activeBullets[j].id].set_after_data(0, -1, false);
							auto it = pl1[i]->cannon_ob.activeBullets.begin();
							advance(it, j);
							pl1[i]->cannon_ob.activeBullets.erase(it);
							j--;
							pl1[i]->bullet_pointer--;
							continue;
						}
						if (j >= 0)
						{
							int found = 0;
							//checking if the bullet has hit an opaque tile in that case the bullet has to be deleted instantaniously
							for (int k = 0; k < opaque.howMany(); k++)
							{
								//   cout << "\n1";
								Greed::coords coord((int)(::ry(pl1[i]->cannon_ob.activeBullets[j].bullet_entity.getPosition().y) - origin_y) / len, (int)(::rx(pl1[i]->cannon_ob.activeBullets[j].bullet_entity.getPosition().x - origin_x)) / len);
								// cout << "\n2";

								if (coord == opaque[k] && !find(cannon_list, coord))
								{
									//pl1[i]->cannon_ob.allBullets[pl1[i]->cannon_ob.activeBullets[j].id].set_after_data(0, -1, false);
									auto it = pl1[i]->cannon_ob.activeBullets.begin();
									advance(it, j);
									pl1[i]->cannon_ob.activeBullets.erase(it);
									j--;
									found = 1;
									pl1[i]->bullet_pointer--;
									break;
								}


							}
							if (found == 1)
							{
								continue;
							}

							if (pl1[i]->cannon_ob.activeBullets.size() > 0)
							{

								//checking if bullet hit another ship
								Greed::coords cor = Greed::coords((int)(::ry(pl1[i]->cannon_ob.activeBullets[j].bullet_entity.getPosition().y) - origin_y) / 80, (int)(::rx(pl1[i]->cannon_ob.activeBullets[j].bullet_entity.getPosition().x) - origin_x) / 80);

								if (!pl1[i]->cannon_ob.activeBullets[j].hit_or_not)
								{


									for (int u = 0; u < pl1.size(); u++)
									{
										if (i != u && pl1[u]->died == 0)
										{
											if (pl1[i]->checkCollision(u, pl1[i]->cannon_ob.activeBullets[j]))
											{
												pl1[i]->cannon_ob.activeBullets[j].set_after_data(5, pl1[u]->ship_id, true);//the bullet is still active for animation
												pl1[i]->cannon_ob.activeBullets[j].hit_or_not = true;
												// cout << "\n collision taken";

												found = 1;
												break;
											}
										}
									}
									if (found == 1)
									{
										continue;
									}
									//checking if the bullet hit a cannon

									for (int k = 0; k < cannon_list.howMany(); k++)
									{
										if (cannon_list[k].isDead == true)
										{
											continue;
										}
										Greed::coords cor = Greed::coords((int)(::ry(pl1[i]->cannon_ob.activeBullets[j].bullet_entity.getPosition().y) - origin_y) / 80, (int)(::rx(pl1[i]->cannon_ob.activeBullets[j].bullet_entity.getPosition().x) - origin_x) / 80);
										if (cannon_list[k].tile == cor)
										{
											// pl1[i]->cannon_ob.allBullets[pl1[i]->cannon_ob.activeBullets[j].id].set_after_data_for_cannon(5, cannon_list[k].cannon_id, true);
											pl1[i]->cannon_ob.activeBullets[j].set_after_data_for_cannon(5, cannon_list[k].cannon_id, true);
											pl1[i]->cannon_ob.activeBullets[j].hit_or_not = true;

											found = 1;
											break;
										}
									}
									if (found == 1)
									{
										continue;
									}

								}


							}
							// cout << "\n end";

							if (pl1[i]->cannon_ob.activeBullets[j].hit_ship != -1)
							{
								//add an animation everytime a bullet gets deleted by colliding with a ship

									//pl1[i]->cannon_ob.allBullets[pl1[i]->cannon_ob.activeBullets[j].id].isActive = false;
									//add the bullet in the list of the hit bullet of the victim ship
								//increasing the score of the firing ship
								pl1[i]->score += 2;
								if (pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->health > 0)
								{
									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->health -= pl1[i]->cannon_ob.activeBullets[j].power;
								}
								if (pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->gold >= GOLD_OFF_A_BULLET)
									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->gold -= GOLD_OFF_A_BULLET;
								//
								//
								//here ships are killed like flies
								//

								if (pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->health <= 0)
								{
									//20% of money is taken from the victim ship
									died.push_back(pl1[i]->cannon_ob.activeBullets[j].hit_ship);

									dying_ships.push_back(pl1[i]->cannon_ob.activeBullets[j].hit_ship);
									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->minutes = total_secs / 60;
									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->seconds = (int)total_secs % 60;
									int ch = (30 * pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->gold) / 100;
									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->gold -= ch;
									pl1[i]->gold += ch;
									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->killer_ship_id = i;
									//passing the fuel of the victim ship
									pl1[i]->fuel += pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->fuel;
									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->fuel = 0;
									//passing the ammo to the victim ship
									pl1[i]->ammo += pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->ammo;
									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->ammo = 0;
									pl1[i]->killed_ships.push_back(pl1[i]->cannon_ob.activeBullets[j].hit_ship);
									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->died = 1;
									//gui_renderer.player_panels[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->getRenderer()->setBackgroundColor(sf::Color::Red);
									//the ship is dead turn off the autopilot mode now
									mutx->mchase[pl1[i]->cannon_ob.activeBullets[j].hit_ship].lock();
									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->autopilot = 0;
									mutx->mchase[pl1[i]->cannon_ob.activeBullets[j].hit_ship].unlock();

									mutx->timeMutex[i].lock();
									timeline t;
									t.eventype = timeline::EventType::DEFEATED_SHIP;
									t.timestamp = total_secs;
									t.d.id = pl1[i]->cannon_ob.activeBullets[j].hit_ship;
									pl1[i]->time_line.push_back(t);
									mutx->timeMutex[i].unlock();

									timeline t1;
									t1.eventype = timeline::EventType::SHIP_DIED;
									t1.timestamp = total_secs;

									pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->time_line.push_back(t1);
									pl1[i]->score += 100;


								}
								pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->bullet_hit.add_rear(pl1[i]->cannon_ob.activeBullets[j]);
								//bullet hit tempo for finding the bullet_hit event
								pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->bullet_hit_tempo.push_back(pl1[i]->cannon_ob.activeBullets[j]);
								animation_list.add_rear(animator(sf::Vector2f(::rx(pl1[i]->cannon_ob.activeBullets[j].bullet_entity.getPosition().x), ::ry(pl1[i]->cannon_ob.activeBullets[j].bullet_entity.getPosition().y)), elapsed_time, ANIMATION_TYPE::EXPLOSION, sf::Vector2f(pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->absolutePosition.x + origin_x, pl1[pl1[i]->cannon_ob.activeBullets[j].hit_ship]->absolutePosition.y + origin_y), pl1[i]->cannon_ob.activeBullets[j].hit_ship));



								auto it = pl1[i]->cannon_ob.activeBullets.begin();
								advance(it, j);
								pl1[i]->cannon_ob.activeBullets.erase(it);
								j--;
								pl1[i]->bullet_pointer--;
								continue;

							}
							if (j >= 0 && pl1[i]->cannon_ob.activeBullets[j].hit_cannon != -1 && pl1[i]->cannon_ob.activeBullets[j].ttl == 15)//this condition is specially for the cannon
							{

								//here the cannon will suffer a  loss
								if (cannon_list[pl1[i]->cannon_ob.activeBullets[j].hit_cannon].health >= pl1[i]->cannon_ob.activeBullets[j].power)
								{
									cannon_list[pl1[i]->cannon_ob.activeBullets[j].hit_cannon].health -= pl1[i]->cannon_ob.activeBullets[j].power;
									pl1[i]->score += 2;
								}
								else
								{
									cannon_list[pl1[i]->cannon_ob.activeBullets[j].hit_cannon].health = 0;

								}
								//if the cannon is dead..give the 1000 money to the killer ship, and set dead status in cannon list
								if (cannon_list[pl1[i]->cannon_ob.activeBullets[j].hit_cannon].health == 0)
								{
									cannon_list[pl1[i]->cannon_ob.activeBullets[j].hit_cannon].isDead = true;
									pl1[i]->score += 100;
									pl1[i]->gold += 1000;
								}
								//pl1[i]->cannon_ob.allBullets[pl1[i]->cannon_ob.activeBullets[j].id].isActive = false;
								animation_list.add_rear(animator(sf::Vector2f(::rx(pl1[i]->cannon_ob.activeBullets[j].bullet_entity.getPosition().x), ::ry(pl1[i]->cannon_ob.activeBullets[j].bullet_entity.getPosition().y)), elapsed_time, ANIMATION_TYPE::EXPLOSION));

								auto it = pl1[i]->cannon_ob.activeBullets.begin();
								advance(it, j);
								pl1[i]->cannon_ob.activeBullets.erase(it);
								j--;
								pl1[i]->bullet_pointer--;
								continue;
							}
							else if (j >= 0 && pl1[i]->cannon_ob.activeBullets[j].hit_or_not)
							{
								pl1[i]->cannon_ob.activeBullets[j].ttl++;
							}
						}
					}
					fire_ship += processing3.getElapsedTime().asSeconds();
				}
				//end of firing of the ship

				sf::Clock processing4;
				processing4.restart();
				for (int j = 0; j < cannon_list.howMany(); j++)
				{
					if (cannon_list[j].isDead == true)
					{
						continue;
					}
					for (int k = 0; k < cannon_list[j].bullet_list.size(); k++)
					{

						if (cannon_list[j].bullet_list[k].bullet_trajectory.size() > 0)
						{
							cannon_list[j].bullet_list[k].absolute_position = Greed::abs_pos(cannon_list[j].bullet_list[k].bullet_trajectory[0].x, cannon_list[j].bullet_list[k].bullet_trajectory[0].y);
							cannon_list[j].bullet_list[k].bullet_entity.setPosition(sf::Vector2f(cx(cannon_list[j].bullet_list[k].bullet_trajectory[0].x + origin_x), cy(cannon_list[j].bullet_list[k].bullet_trajectory[0].y + origin_y)));
							for (int l = 1; l <= 3; l++)
							{
								if (cannon_list[j].bullet_list[k].bullet_trajectory.size() > 0)
								{
									cannon_list[j].bullet_list[k].bullet_trajectory.erase(cannon_list[j].bullet_list[k].bullet_trajectory.begin());
								}
							}
						}
						else //when bullet's trajectory is over and nothing happend
						{
							//  cannon_list[j].allBullets[pl1[i]->cannon_ob.activeBullets[j].id].set_after_data(0, -1, false);
							cannon_list[j].bullet_list.erase(cannon_list[j].bullet_list.begin());
							k--;
							continue;
						}

						if (k >= 0)
						{

							if (cannon_list[j].bullet_list.size() > 0)
							{
								//cout << "\n reached";
								//ship vs ship collision

								// cout << "\n caught";
								// cout << "\n cor has==>" << cor.r << " " << cor.c;
								 //cout << "\n ship 0 has position==>" << pl1[0]->getCurrentTile().r << " " << pl1[0]->getCurrentTile().c;
								for (int u = 0; u < pl1.size(); u++)
								{
									if (pl1[u]->died == 1)
									{
										continue;
									}
									if (checkCollision(u, cannon_list[j].bullet_list[k]))//collision is there
									{
										//cannon_list[j].allBullets[cannon_list[j].bullet_list[k].id].set_after_data(5, pl1[u]->ship_id, true);
										cannon_list[j].bullet_list[k].set_after_data(5, pl1[u]->ship_id, true);
										//pl1[i]->cannon_ob.activeBullets[j].set_after_data(5, pl1[u]->ship_id, true);//the bullet is still active for animation
										cannon_list[j].bullet_list[k].hit_or_not = true;
										// cout << "\n collision taken";
									}

								}
							}
							// cout << "\n end";
						}
						if (cannon_list[j].bullet_list[k].ttl == 1)//here cannon bullet strikes the ship
						{
							//add an animation everytime a bullet gets deleted by colliding with a ship
							cannon_list[j].bullet_list[k].isActive = false;
							//cannon_list[j].allBullets[cannon_list[j].bullet_list[k].id].isActive = false;

							//add the bullet in the list of the hit bullet of the victim ship
							if (pl1[cannon_list[j].bullet_list[k].hit_ship]->health >= cannon_list[j].bullet_list[k].power)
							{
								pl1[cannon_list[j].bullet_list[k].hit_ship]->health -= cannon_list[j].bullet_list[k].power;//reducing the power of the ship
							}
							else
							{
								pl1[cannon_list[j].bullet_list[k].hit_ship]->health = 0;
							}
							if (pl1[cannon_list[j].bullet_list[k].hit_ship]->health <= 0)
							{
								died.push_back(cannon_list[j].bullet_list[k].hit_ship);

								int id = cannon_list[j].bullet_list[k].hit_ship;
								pl1[id]->died = 1;

								mutx->mchase[id].lock();
								pl1[id]->autopilot = 0;
								mutx->mchase[id].unlock();

								pl1[id]->minutes = total_secs / 60;
								pl1[id]->seconds = (int)total_secs % 60;
								int ch = (30 * pl1[id]->gold) / 100;
								pl1[id]->gold -= ch;

								pl1[id]->killer_cannon_id = j;
								//mutx->timeMutex[id].lock();
								timeline t1;
								t1.eventype = timeline::EventType::SHIP_DIED;
								t1.timestamp = total_secs;

								pl1[id]->time_line.push_back(t1);
								//mutx->timeMutex[id].unlock();

							}

							pl1[cannon_list[j].bullet_list[k].hit_ship]->getBulletHitList().add_rear(cannon_list[j].bullet_list[k]);
							pl1[cannon_list[j].bullet_list[k].hit_ship]->bullet_hit_tempo.push_back(cannon_list[j].bullet_list[k]);

							animation_list.add_rear(animator(sf::Vector2f(rx(cannon_list[j].bullet_list[k].bullet_entity.getPosition().x), ry(cannon_list[j].bullet_list[k].bullet_entity.getPosition().y)), elapsed_time, ANIMATION_TYPE::EXPLOSION, sf::Vector2f((pl1[cannon_list[j].bullet_list[k].hit_ship]->absolutePosition.x + origin_x), pl1[cannon_list[j].bullet_list[k].hit_ship]->absolutePosition.y + origin_y), cannon_list[j].bullet_list[k].hit_ship));
							cannon_list[j].bullet_list.erase(cannon_list[j].bullet_list.begin());
							k--;
						}
						else if (cannon_list[j].bullet_list[k].hit_or_not)
						{
							cannon_list[j].bullet_list[k].ttl++;
						}

					}

				}
				fire_cannon += processing4.getElapsedTime().asSeconds();

		
			top_layer ship_packet;
			memset((void*)&ship_packet, 0, sizeof(ship_packet));
			for (int i = 0; i < pl1.size(); i++)
			{
				//	unique_lock<mutex> lk(mutx->timeMutex[i]);
				if (pl1[i]->time_line.size() > 0)
				{
					ship_packet.ob[i].isthere = true;

					ship_packet.ob[i].time_line = pl1[i]->time_line[0];
					pl1[i]->time_line.pop_front();
				}
				//	lk.unlock();
			}
			
			ship_packet.no_of_players = pl1.size();
			int bullet_no = 0;//number of total bullets

			ship_packet.gameOver = gameOver;
			
			for (int i = 0; i < pl1.size(); i++)
			{
				ship_packet.ob[i].ship_id = i;
				strcpy(ship_packet.ob[i].name,pl1[i]->name.c_str());
				ship_packet.ob[i].seconds = pl1[i]->seconds;
				ship_packet.ob[i].minutes = pl1[i]->minutes;
				//initialize all the members of class pack_ship
				ship_packet.ob[i].killer_ship_id = pl1[i]->killer_ship_id;
				if (pl1[i]->killed_ships.size() <= 10)
				{
					ship_packet.ob[i].killed_ships_size = pl1[i]->killed_ships.size();
				}
				else
				{
					ship_packet.ob[i].killed_ships_size = 10;
				}
				for (int j = 0; j < ship_packet.ob[i].killed_ships_size; j++)
				{
					ship_packet.ob[i].killed_ships[j] = pl1[i]->killed_ships[j];
				}
				ship_packet.ob[i].score = pl1[i]->score;
				ship_packet.ob[i].front_abs_pos = pl1[i]->front_abs_pos;
				ship_packet.ob[i].rear_abs_pos = pl1[i]->rear_abs_pos;
				ship_packet.ob[i].dir = pl1[i]->dir;
				ship_packet.ob[i].absolutePosition = pl1[i]->absolutePosition;

				ship_packet.ob[i].radius = pl1[i]->radius;
				ship_packet.ob[i].bullet_radius = pl1[i]->bullet_radius;


				ship_packet.ob[i].health = pl1[i]->health;

				ship_packet.ob[i].gold = pl1[i]->gold;
				ship_packet.ob[i].died = pl1[i]->died;
				ship_packet.ob[i].motion = pl1[i]->motion;
				ship_packet.ob[i].fuel = pl1[i]->fuel;
				ship_packet.ob[i].invisible = pl1[i]->invisible;
				ship_packet.ob[i].ammo = pl1[i]->ammo;
				ship_packet.ob[i].bullet_pointer = pl1[i]->bullet_pointer;

				ship_packet.ob[i].died = pl1[i]->died;
				ship_packet.ob[i].tile_pos_front = pl1[i]->tile_pos_front;
				ship_packet.ob[i].tile_pos_rear = pl1[i]->tile_pos_rear;
				for (int j = 0; j <= pl1[i]->bullet_pointer && bullet_no < 500; j++)
				{
					if (pl1[i]->died == 0)
					{
						bullet_no++;
						ship_packet.bullet_pos[j] = pl1[i]->cannon_ob.activeBullets[j].absolute_position;
						if (pl1[i]->cannon_ob.activeBullets[j].absolute_position.x == 0 && pl1[i]->cannon_ob.activeBullets[j].absolute_position.y == 0)
						{
							cout << "\n problem caused by ship=>" << i;
							cout << "\n bullet information is=>\n target ship=>" << pl1[i]->cannon_ob.activeBullets[j].target_ship;
							cout << "\n target cannon=>" << pl1[i]->cannon_ob.activeBullets[j].target_cannon;
						}
					}

				}
			}
			//adding the bullets of the cannons
			for (int i = 0; i < cannon_list.howMany(); i++)
			{
				for (int j = 0; j < cannon_list[i].bullet_list.size() && bullet_no < 500; j++)
				{
					if (cannon_list[i].isDead == 0)
					{
						ship_packet.bullet_pos[bullet_no] = cannon_list[i].bullet_list[j].absolute_position;
						bullet_no++;
						if (cannon_list[i].bullet_list[j].absolute_position.x == 0 && cannon_list[i].bullet_list[j].absolute_position.y == 0)
						{
							cout << "\n problem caused by cannon=>" << i;
							cout << "\n bullet information is=>\n target ship=>" << cannon_list[i].bullet_list[j].target_ship;
						}
					}
				}

			}



			ship_packet.no_of_bullets = bullet_no;
			sf::Time time = processing.getElapsedTime();
			avg_processing += time.asSeconds();

			sf::Clock sending;
			sending.restart();
			//sending the data over here to the client terminal
			recv_data data1;
			
			
			memset((void*)&data1, 0, sizeof(data1));
			//preparing the packet for client terminal
			strcpy(data1.token, my_token.c_str());
			data1.packet_id = total_time;
			data1.s1 = pl1.size();
			data1.gameOver = gameOver;
			shipData_exceptMe sdem[20];
			control.pl_to_packet(data1.shipdata_exceptMe, pl1);
			for(int sid=0;sid<max_player;sid++)
			{
					shipData_forMe sdfm;
					control.me_to_packet(sdfm, sid, pl1);
					data1.shipdata_forMe = sdfm;
					//sending the data
					data1.packet_id = total_time;
					unique_lock<mutex> lk(mutx->send_terminal);
					terminal_data.push_back(pair<int, recv_data>(sid, data1));
					
			}
			mutx->cond_terminal.notify_one();//notify that the data is ready to be read from the queue.			
		
			sf::Time time2 = sending.getElapsedTime();
			avg_send1 += time2.asSeconds();
			avg_send += time2.asSeconds();
			//rendering starts from here
			


			//cannon_list[0].fireCannon(1, ShipSide::FRONT);
			processing.restart();
			for (int i = 0; i < cannon_list.howMany(); i++)
			{
				if (cannon_list[i].isDead == true)
				{
					continue;
				}

				int si = cannon_list[i].getVictimShip();
				double req_angle = cannon_list[i].get_required_angle();
				double current_angle = cannon_list[i].current_angle;

				if ((int)current_angle != (int)req_angle)
				{
					double diff = abs(req_angle - current_angle);

					if (diff < 5)
					{
						cannon_list[i].current_angle = req_angle;
					//	cannon_list[i].cannon_sprite.setRotation(req_angle);

					}
					else if (current_angle > req_angle)
					{
						//cannon_list[i].cannon_sprite.rotate(-2.5);
						cannon_list[i].current_angle -= 2.5;
					}
					else if (current_angle < req_angle)
					{
						//cannon_list[i].cannon_sprite.rotate(2.5);
						cannon_list[i].current_angle += 2.5;
					}

				}
				//cout << "\n current angle=>" << cannon_list[2].current_angle;
				if ((int)req_angle == (int)current_angle)
				{

					if (c % 20 == 0)
					{
						int val = cannon_list[i].current_ship;
						if (val >= 0 && pl1[val]->died == 0 && (cannon_list[i].isShipInMyRadius(cannon_list[i].current_ship, ShipSide::FRONT)))
						{
							if (!gameOver && cannon_list[i].current_ship != -1 && pl1[cannon_list[i].current_ship]->died == 0)
							{
								cannon_list[i].fireCannon(cannon_list[i].current_ship);//this is the only place where we have to fire

								animation_list.add_rear(animator(sf::Vector2f(cannon_list[i].absolute_position.x + origin_x, cannon_list[i].absolute_position.y + origin_y), elapsed_time, ANIMATION_TYPE::CANNON_FIRE, cannon_list[i].cannon_id));
							}
						}
					}
				}

			}

			//setting the base sprite on the cannon
			for (int i = 0; i < cannon_list.howMany(); i++)
			{
				ship_packet.cannon_ob[i].cid = i;
				ship_packet.cannon_ob[i].angle = cannon_list[i].current_angle;
			
				ship_packet.cannon_ob[i].picture = 0;
				ship_packet.cannon_ob[i].cannon_health = cannon_list[i].health;
				ship_packet.cannon_ob[i].is_cannon_dead = cannon_list[i].isDead;
			}
	
			ship_packet.no_of_animation = animation_list.howMany();
			for (int i = 0; i < animation_list.howMany(); i++)
			{
				//cout << "\n second";
				   // cout << "\n bullet number==>" << i << " " << "ttl=>" << animation_list[i].ttl;
				if (animation_list[i].type == graphics::ANIMATION_TYPE::EXPLOSION)
				{
					if (animation_list[i].total_time <= animation_list[i].animation_time)
					{
						sf::Sprite explosion_sprite;
						//here i am using moving animations i.e the frame is changing along with the position of the sprite
						if (animation_list[i].sid != -1)//this is for the ship
						{
							sf::Vector2f pos((animation_list[i].animation_offset.x + pl1[animation_list[i].sid]->absolutePosition.x + origin_x), (animation_list[i].animation_offset.y + pl1[animation_list[i].sid]->absolutePosition.y + origin_y));
							ship_packet.animation_ob[i].x = pos.x;
							ship_packet.animation_ob[i].y = pos.y;
						}
						else
						{
							ship_packet.animation_ob[i].x = animation_list[i].animation_position.x;
							ship_packet.animation_ob[i].y = animation_list[i].animation_position.y;
						}

						//cout << "\n--------------------------------";
						//cout << "\n for i==>" << i;
						int f=animation_list[i].frame(elapsed_time, explosion_sprite);
						ship_packet.animation_ob[i].picture = f;
						//cout << "\n----------------------------------";
					}
					else
					{
						animation_list.delAt(i);
						i--;
					}
				}
				else if (animation_list[i].type == graphics::ANIMATION_TYPE::CANNON_FIRE)
				{


					if (animation_list[i].total_time <= animation_list[i].animation_time)
					{
						int h = animation_list[i].frame(elapsed_time, cannon_list[animation_list[i].cid].cannon_sprite);

						//ship_packet.cannon_ob[animation_list[i].cid].angle = cannon_list[animation_list[i].cid].current_angle;
						ship_packet.cannon_ob[animation_list[i].cid].picture = h;
					}
					else
					{

						animation_list.delAt(i);
						i--;
					}
				}


			}
			avg_processing2 += processing.getElapsedTime().asSeconds();
			//send the data over here to the display unit of the client 
			/* this has to be in a different set of sockets
			* this is a connection to the display unit of the client
			* 
			*/
			sending.restart();
			
			//apply this logic and then check if it is  working with 7 players or not
			for(int i=0;i<socket_id_display.size();i++)
			{
				auto it = socket_id_display.begin();
				advance(it, i);
				ship_packet.packet_no = total_time;
				ship_packet.total_secs = total_secs;
				
				prev_packet_display = ship_packet.packet_no;
				unique_lock<mutex> lk(mutx->send_display);
				display_data.push_back(pair<int, top_layer>(it->first, ship_packet));
				//cout << ship_packet.packet_no << ": " << ship_packet.ob.absolutePosition.x << " " << ship_packet.ob.absolutePosition.y << endl;
				
			}
			//notifying the vairable
			mutx->cond_display.notify_one();//notified...
			

      		//recv the data here and update for the next frame
			sf::Clock recv_data;
			recv_data.restart();
			
			for(int j=0;j<n;j++)
			{
				if (pl1[j]->died == 1)
				{
					
					continue;
				}
				
				select(recver+1, &read, 0, 0, &timeout);
			
				if (FD_ISSET(recver, &read))
				{
					send_data data2;
					memset((void*)&data2, 0, sizeof(data2));
					struct sockaddr_storage client_address;
					socklen_t client_len = sizeof(client_address);
					int bytes = recvfrom(recver, (char*)&data2, sizeof(data2), 0, (sockaddr*)&client_address, &client_len);
					//bytes will be recved by the recver socket
					if (bytes < 1)
					{
					    cout << "\n could not recv the data from the client==>" <<j<<" "<< GetLastErrorAsString();
					}
					
					
					//data is received now convert it to appropriate form
					int sid = data2.shipdata_forServer.ship_id;
					if (data2.shipdata_forServer.ship_id != sid)
					{
						cout << "\n fukk";
					}
					if (pl1[sid]->died == 1)
					{
						continue;
					}
					if (bytes > 0)
					{
						control.server_to_myData(data2.shipdata_forServer, pl1, sid, mutx);
						prev_packet_id[sid] = data2.packet_id;
						if (sid == 1)//for 1 only
						{
							auto now = std::chrono::system_clock::now();
							auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
							auto secs = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()) % 60;
							auto mins = std::chrono::duration_cast<std::chrono::minutes>(now.time_since_epoch()) % 60;
							auto hours = std::chrono::duration_cast<std::chrono::hours>(now.time_since_epoch());
							
							//cout << "\n recved data from client terminal=>" << data2.packet_id << " at the time==> " <<
							//hours.count() << ":" << mins.count() << ":" << secs.count() << ":" << ms.count() << endl;
						}

					}
					else if(bytes>0)
					{
						cout << "\n non verified user sending data..";
						cout << "\n the authenticated data sent is==>" << data2.user_cred.username << " " << data2.user_cred.password;

					}

					std::time_t result = std::time(nullptr);
					//cout << "\n time=>" << std::localtime(&result)->tm_hour << ":" << std::localtime(&result)->tm_min << ":" << std::localtime(&result)->tm_sec << " server frame=>" << total_time << " " << "received frame=>" << data2.packet_id;
				}
			}
			sf::Time time4 = recv_data.getElapsedTime();
			avg_recv += time4.asSeconds();

			if (gameOver   &&  game_over_frame+4 == total_time)//so that the packet reaches to everyone.
			{
				break;//break the loop
			} 
		
		}

	}
	//closing it for some time......
	
	//closing all the left sockets
	//sending to all the connected clients that the game is over
	recv_data last;
	last.packet_id = INT_MAX;
	last.gameOver = 1;
	for (int i = 0; i < socket_id.size(); i++)
	{
		auto it = socket_id.begin();
		advance(it, i);
		int bytes = sendto(socket_listen, (char*)&last, sizeof(last), 0, (sockaddr*)&it->second, sizeof(it->second));
		if (bytes < 1)
		{
			cout << "\n couldn't sent bytes to the client unit";
		}  
	}
	CLOSESOCKET(socket_listen);
	CLOSESOCKET(socket_listen2);
	CLOSESOCKET(recver);
	
	//CLOSESOCKET(socket_display[0]);

	cout << "\n average times are==>";
	double sending0 = 0;
	double sending1 = 0;
	double sending2 = 0;
	double processing1 = 0;
	double processing2 = 0;
	double receiving = 0;

	double fire_update1 = 0;
	double navigation_ship1 = 0;
	double fire_ship1 = 0;
	double fire_cannon1 = 0;
	for (int i = 0; i < de.size(); i++)
	{
		processing1 += de[i].avg_processing1;
		processing2 += de[i].avg_processing2;
		sending0 += de[i].avg_sending;
		receiving += de[i].avg_receiving;

		fire_update1 += de[i].fire_upgrade;
		navigation_ship1 += de[i].navigation_ship;
		fire_ship1 += de[i].fire_ship;
		fire_cannon1 += de[i].fire_cannon;
		sending1 += de[i].avg_send1;
		sending2 += de[i].avg_send2;
	
	}
	cout << "\n sending=>" << sending0 / total_frames;
	cout << "\n sending to the client terminal==>" << sending1 / total_frames;
	cout << "\n sending to the client display unit==>" << sending2 / total_frames;
	cout << "\n processing1=>" << processing1 / total_frames;
	cout << "\n processing2=>" << processing2 / total_frames;
	cout << "\n receiving=>" << receiving / total_frames;
	cout << "\n fire updgrade=>" << fire_update1 / total_frames;
	cout << "\n navigation_ship=>" << navigation_ship1 / total_frames;
	cout << "\n fire_ship=>" << fire_ship1 / total_frames;
	cout << "\n fire _cannon=>" << fire_cannon1 / total_frames;
}

void startup(int n,unordered_map<int,sockaddr_storage> &socket_id, int port)//here n is the max player
{
	//recving the client credentials from the lobby server
				
	printf("now waiting for the clients to connect...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo* bind_address;
	//convert port to string
	char port_str[10];
	// Convert port to string
	sprintf(port_str, "%d", port);
	char port_str1[10];
	char port_str2[10];
	sprintf(port_str1, "%d", (port + 1));
	sprintf(port_str2, "%d", (port + 2));
	//cout << "\n running on port=>" << port_str;
	getaddrinfo(0, port_str, &hints, &bind_address);
	struct addrinfo* bind_address1;
	getaddrinfo(0,port_str1, &hints, &bind_address1);
	struct addrinfo* bind_address2;
	getaddrinfo(0,port_str2, &hints, &bind_address2);
	

	//printf("Creating socket...\n");

	socket_listen = socket(bind_address->ai_family,bind_address->ai_socktype, bind_address->ai_protocol);
	socket_listen2 = socket(bind_address1->ai_family, bind_address1->ai_socktype, bind_address1->ai_protocol);
	recver = socket(bind_address2->ai_family, bind_address2->ai_socktype, bind_address2->ai_protocol);
	
	if (!ISVALIDSOCKET(socket_listen))
	{
		fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
		
	}
	if (!ISVALIDSOCKET(socket_listen2))
	{
		fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());

	}
	if (!ISVALIDSOCKET(recver))
	{
		fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());

	}
	//printf("Binding socket to local address...\n");
	if (::bind(socket_listen,
		bind_address->ai_addr, bind_address->ai_addrlen)) {
		fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
	
	}
	if (::bind(socket_listen2,
		bind_address1->ai_addr, bind_address1->ai_addrlen)) {
		fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());

	}
	if (::bind(recver,
		bind_address2->ai_addr, bind_address2->ai_addrlen)) {
		fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());

	}
	freeaddrinfo(bind_address);
	freeaddrinfo(bind_address1);
	freeaddrinfo(bind_address2);
	
	fd_set master;
	FD_ZERO(&master);
	FD_SET(socket_listen, &master);
	FD_SET(socket_listen2, &master);
	
	fd_set reads;
	FD_ZERO(&reads);
	int idc = 0;
	int nn = 0;
	SOCKET max_socket = max(socket_listen, socket_listen2);
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 10;
	int max_display = 0;
	int curdisp = 0;
	/*also add if..10 minutes are up, then no more waiting and we will start with whatever we have*/
	while (max_player > nn || max_display > curdisp)//this is when we are  using 2 computers for testing, so if there are n clients so the total clients including display unit is=>2*n
	{
		reads = master;
		select(max_socket + 1, &reads, 0, 0, &timeout);
		if(FD_ISSET(socket_listen,&reads))
		{
				greet_client gc;
				struct sockaddr_storage client_address;
				socklen_t client_len = sizeof(client_address);
				int read;
				RECVFROM(socket_listen, (char*)&gc, sizeof(gc),(struct sockaddr*)&client_address, client_len);
				int found = 0;				

				if (strcmp(gc.token, my_token.c_str()) == 0)//checking the correct code of the current game instance
				{
					read = gc.code;
					string sread = to_string(read);
					//here the code will be 0 for client algorithm unit, and 1 for display unit, after 1 we will have the id of the client
					//cout << "\n code recved is=>" << read;
					if (sread[0] == '0' || sread[0]=='2')
					{
							socket_id[idc] = client_address;
							//sending the id of the client to the client
							SENDTO(socket_listen, (char*)&idc, sizeof(idc),(struct sockaddr*)&client_address, client_len);
							user_cred[idc] = gc.user_cred;//setting the user credential
							idc++;
							nn++;
							cout << "\n connected with a player..";
							
					}
					if (sread[0] == '0')//2 is for when the client does not want its display unit to open
					{
						max_display++;
					}
						
				}
				else
				{
					cout << "\n client who did not have the correct game token tried to contact the game server..";
				}	
			
		}
		if (FD_ISSET(socket_listen2, &reads))
		{
			greet_client gc;
			struct sockaddr_storage client_address;
			socklen_t client_len = sizeof(client_address);
			int read;
			RECVFROM(socket_listen2, (char*)&gc, sizeof(gc),(struct sockaddr*)&client_address, client_len);
			//cout << "\n recved data=>" << gc.token;
				if (strcmp(gc.token, my_token.c_str()) == 0)//checking the correct code of the current game instance
				{
					read = gc.code;
					string sread = to_string(read);
					//here the code will be 0 for client algorithm unit, and 1 for display unit, after 1 we will have the id of the client
				//	cout << "\n code recved is=>" << read;
					if (sread[0] == '1')
					{
						//finding the id of the client through the code sent by the display unit
						int id = stoi(sread.substr(1, sread.length() - 1));
					//	cout << "\n id sent is==>" << id;
						socket_id_display[id] = client_address;
						//sending the max_player to the display unit
						SENDTO(socket_listen2, (char*)&max_player, sizeof(max_player),(sockaddr*)&client_address, client_len);
						curdisp++;
						cout << "\n connected with a client display unit...";
					}
				}
			
		}


	}

	int no_of_players = n;
	Control control;
	//creating an object of class Mutex: this object will be passed to every class using mutex
	Mutex mutx;

	//sf::RenderWindow window(sf::VideoMode(1920,1080),"testing window");
	int code[rows][columns];
	List<Greed::coords> opaque_coords;//coordinates of the land 
	Map map1(0);
	map1.initialize_map(code, opaque_coords);
	control.opaque_coords = opaque_coords;
	Greed ob;
	List<Greed::vertex>& l = ship::getGlobalMapref(2369);//no need to change this line
	ob.getMap(code, rows, columns, l); // here globalMap is set
	//initializing globalMap in ship class
	deque<shipInfo> silist;
	List<Greed::coords> l1;
	ship* player = new ship[no_of_players];


	deque<ship*> slist;
	//vector<int> iop;
	for (int i = 0; i < no_of_players; i++)
	{
		silist.push_back(shipInfo(&player[i]));
		slist.push_back(&player[i]);
	}
	//generating the coords for spawning the ship
	vector<Greed::coords> spawn;//spawning position for ships having different configurations
	if (no_of_players == 1)//for 0 players
	{
		spawn.push_back(Greed::coords(10, 11));

	}
	else if (no_of_players == 2)
	{

		spawn.push_back(Greed::coords(1, 1));
		spawn.push_back(Greed::coords(10, 23));

	}
	else if (no_of_players == 3)
	{
		spawn.push_back(Greed::coords(1, 0));
		spawn.push_back(Greed::coords(1, 23));
		spawn.push_back(Greed::coords(10, 11));
	}
	else if (no_of_players == 4)
	{
		spawn.push_back(Greed::coords(1, 0));
		spawn.push_back(Greed::coords(1, 23));
		spawn.push_back(Greed::coords(10, 0));
		//spawn.push_back(Greed::coords(0,0));
		spawn.push_back(Greed::coords(10, 23));
	}
	else if (no_of_players == 5)
	{
		spawn.push_back(Greed::coords(1, 0));
		spawn.push_back(Greed::coords(1, 23));
		spawn.push_back(Greed::coords(10, 0));
		spawn.push_back(Greed::coords(10, 23));
		spawn.push_back(Greed::coords(10, 11));
	}
	else if (no_of_players == 6)
	{
		spawn.push_back(Greed::coords(1, 0));
		spawn.push_back(Greed::coords(1, 23));
		spawn.push_back(Greed::coords(10, 0));
		spawn.push_back(Greed::coords(10, 23));
		spawn.push_back(Greed::coords(10, 11));
		spawn.push_back(Greed::coords(2, 11));
	}
	else if (no_of_players == 7)
	{
		spawn.push_back(Greed::coords(1, 0));
		spawn.push_back(Greed::coords(1, 23));
		spawn.push_back(Greed::coords(6, 0));
		spawn.push_back(Greed::coords(6, 23));
		spawn.push_back(Greed::coords(10, 0));
		spawn.push_back(Greed::coords(10, 11));
		spawn.push_back(Greed::coords(10, 23));
	}
	else if (no_of_players == 8)
	{
		spawn.push_back(Greed::coords(1, 0));
		spawn.push_back(Greed::coords(1, 23));
		spawn.push_back(Greed::coords(6, 0));
		spawn.push_back(Greed::coords(6, 23));
		spawn.push_back(Greed::coords(10, 0));
		spawn.push_back(Greed::coords(10, 11));
		spawn.push_back(Greed::coords(10, 23));
		spawn.push_back(Greed::coords(0, 5));

	}
	for (int i = 0; i < no_of_players; i++)
	{

		player[i].initialize_player(i, "hawk", &mutx, silist, code, 5, spawn[i]);//last element is the tile pos of the player


	}
	//sending the startup data to all the connected clients


	//connector_show(socket_display,socket_id_display, 1);//connecting with display unit of the client

	cout << "\n sending the data to the clients=>";
/*
	for (int i = 0; i < no_of_players; i++)
	{
		auto it = socket_id.begin();
		advance(it, i);
		Startup_info_client data1(60, no_of_players, it->first, spawn[it->first]);
		cout << "\n the id is==>" << it->first;
		//sendto(socket_listen, (char*)&data1, sizeof(data1), 0, (sockaddr*)&it->second, sizeof(it->second));
		SENDTO(socket_listen, (char*)&data1, sizeof(data1),(sockaddr*)&it->second, sizeof(it->second));
		
	}
	*/
	//starting the tcp connection with the clients for the connection

	struct addrinfo hints1;
	memset(&hints1, 0, sizeof(hints1));
	hints1.ai_family = AF_INET;
	hints1.ai_socktype = SOCK_STREAM;
	hints1.ai_flags = AI_PASSIVE;
	struct addrinfo* bind_addres;
	int port2 = port + 3;
	char port2_str[10];
	sprintf(port2_str, "%d", port2);
	getaddrinfo(0,port2_str, &hints1, &bind_addres);//this server is running on port 8080

	SOCKET tcp_socket = socket(bind_addres->ai_family, bind_addres->ai_socktype, bind_addres->ai_protocol);
	if (!ISVALIDSOCKET(tcp_socket))
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
	if (bind(tcp_socket, (const sockaddr*)bind_addres->ai_addr, (int)bind_addres->ai_addrlen))
	{
		cout << "\n failed to bind the socket==>" << GETSOCKETERRNO();
	}
	if (listen(tcp_socket, 20) < 0)
	{
		cout << "\n socket failed";
	}
	freeaddrinfo(bind_addres);
	fd_set all;
	FD_ZERO(&all);
	FD_SET(tcp_socket, &all);
	fd_set read;
	FD_ZERO(&read);
	int count = 0;
	max_socket = tcp_socket;
	while (count != max_player)
	{
		read = all;
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		select(max_socket + 1, &read, 0, 0, &timeout);
		for (int i = 1; i <= max_socket; i++)
		{

			if (FD_ISSET(i, &read))
			{
				if (i == tcp_socket)
				{
					//connect with the client
					struct sockaddr_storage client_address;
					socklen_t client_len = sizeof(client_address);
					SOCKET client = accept(tcp_socket, (sockaddr*)&client_address, &client_len);
					cout << "\n connection accepted=>" << client;
					FD_SET(client, &all);
					if (client > max_socket)
					{
						max_socket = client;
					}

				}
				else
				{
					int id;
					int bytes = recv(i, (char*)&id, sizeof(id), 0);
					if (bytes < 1)
					{
						cout << "\n cannot recv tcp id from the client terminal" << GetLastErrorAsString() << GETSOCKETERRNO();
						cout << "\n socket that got ready is==>" << i;
					}
					//sending the respected data to the client terminal
					else if (bytes > 1)
					{
						Startup_info_client data1(60, no_of_players, id, spawn[id]);
						bytes = send(i, (char*)&data1, sizeof(data1), 0);
						if (bytes < 1)
						{
							cout << "\n cannot send the data to the client" << GetLastErrorAsString();
						}
						count++;
					}
				}


			}
		}
	}
	
	//send to all the clients terminal to start the game now
	int start = 1;
	for (int i = 1; i <= max_socket; i++)
	{
		if (FD_ISSET(i, &all))
		{
			int bytes = send(i, (char*)&start, sizeof(start), 0);
			if (bytes < 1)
			{
				cout << "\n cannot send the data to the client" << GetLastErrorAsString();
			}
		}
	}
	//closing the sockets now
	for (int i = 1; i <= max_socket; i++)
	{
		if (FD_ISSET(i, &all))
		{
			CLOSESOCKET(i);
		}
	}
	
	cout << "\n data is sent to all the clients";
	control.setShipList(slist, 2369);

	List<Greed::cannon> cannon_list;
	for (int i = 1; i <= 3; i++)
	{
		cannon_list.add_rear(Greed::cannon());

	}

	cannon_list[0].initializer(0, Greed::coords(5, 11));
	cannon_list[1].initializer(1, Greed::coords(3, 6));
	cannon_list[2].initializer(2, Greed::coords(7, 8));

	control.cannon_list = cannon_list;
	ship::cannon_list = cannon_list;


	graphics cg;
	cg.callable(&mutx, code, map1,n,socket_id, socket_display);//dont call callable function its depricated
	//sending the message to the lobby server that i am free to start another game

	int status = 1;
	int bytes = send(lobby_socket, (char*)&status, sizeof(status), 0);
	if (bytes < 1)
	{
		cout << "\n could not send bytes=>" << GetLastErrorAsString();
	}
	cout << "\n starting the game, now you will see the frame rate of the game...ideally it should be arount 60 fps..";

}

int connect_with_lobby()
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	struct addrinfo* bind_address;
	getaddrinfo("127.0.0.1", "8080", &hints, &bind_address);
	lobby_socket = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
	if (!ISVALIDSOCKET(lobby_socket))
	{
		fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}
	while (connect(lobby_socket, bind_address->ai_addr, bind_address->ai_addrlen))
	{
		fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
		cout << GetLastErrorAsString();
		//return 1;
	}
	freeaddrinfo(bind_address);
	//sending hi to the server
	server_startup ob;
	int bytes = recv(lobby_socket, (char*)&ob, sizeof(ob), 0);
	if (bytes < 0)
	{
		cout << "\n couldnt recv the port==>" << GetLastErrorAsString();
	}
	cout << "\n connected with the lobby server...";
	//cout << "\n received port number=>" << ob.port;
	//cout << "\n received game token=>" << ob.token;
	my_token = ob.token;
	return ob.port;
	
}
int main(int argc, char* argv[])
{
	cout << "\n the current version of the game server  is=>" << version;
#if defined(_WIN32)
	WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d)) {
		fprintf(stderr, "Failed to initialize.\n");
		return 1;
	}
#endif

	

	cout << "\n enter the number of players=>";
	cin >> max_player;
	max_player = min(max_player, ALLOWED_MAX_PLAYER);
	cout << "\n number of players playing are==>" << max_player;
	
	int port = connect_with_lobby();//to connect with the lobby
	while (1)
	{
		cout << "\n staring the server.....";
		unordered_map<int, sockaddr_storage> socket_id;	//just a fake thing, so that i dont have to change the signature of startup in multiple classes
		socket_id_display.clear();//clearing the unordered map for reuse
		
		//resetting here some static variables
		Control::ship_list.clear();
		Control::cannon_list.erase();
		terminal_data.clear();
		display_data.clear();
		gameOver = 0;
		graphics::total_secs = 0;
		user_cred.clear();
		startup(max_player, socket_id, port);
	
		//all the sockets are closed before restarting the game'
		cout << "\n this round of game is over";
	}

#if defined(_WIN32)
	WSACleanup();
#endif

	printf("Finished.\n");
	return 0;

}
