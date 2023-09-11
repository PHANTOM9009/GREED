#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<future>
#include<thread>

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
#include "online_lib2.hpp"
#include "online_lib2.cpp"

SOCKET peer_socket;//the main socket that is used for handshaking only
SOCKET recver_socket;//socket for recving data from the server

int max_players;
char server_port[10];
int my_id;//id of the ship in the game
string game_token;//token of the game
string username;
string password;
string ip_address;
bool sound = true;//intiially the sound is on


bool SEND(SOCKET sock, char* buff, int length)
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
	int b = send(sock, buff, length, 0);
	if (b < 1)
	{
		cout << "\n cannot send the bytes.." << GETSOCKETERRNO();
		return false;
	}
	while (1)
	{
		read = master;
		select(sock + 1, &read, 0, 0, &timeout);
		if (FD_ISSET(sock, &read))
		{
			int ack;
			int bytes = recv(sock, (char*)&ack, sizeof(ack), 0);
			if (ack == 1)
			{
				return true;
			}
		}
		auto end = chrono::steady_clock::now();
		chrono::duration<double> iteration_time = chrono::duration_cast<chrono::duration<double>>(end - now);
		if (iteration_time.count() > 1)
		{
			//resend the packet
			int bytes = send(sock, buff, length, 0);
			if (bytes < 1)
			{
				cout << "\n cannot send the bytes again=>" << GETSOCKETERRNO();
				return false;
			}

			now = chrono::steady_clock::now();//changing the time since the last message sent
		}

	}
}
bool RECV(SOCKET sock, char* buff, int length)
{
	/*overload for UDP recv, to recv the data carefully*/
	while (1)
	{
		int b = recv(sock, buff, length, 0);
		if (b > 1)
		{
			int ack = 1;
			int bytes = send(sock, (char*)&ack, sizeof(ack), 0);
			if (bytes < 1)
			{
				cout << "\n cannot send the bytes. . " << GETSOCKETERRNO();
			}
			return true;
		}
	}

}
void update_frame(deque<ship*>& pl1, pack_ship& ob, int i)
{
	pl1[i]->ship_id = ob.ship_id;
	pl1[i]->seconds = ob.seconds;
	pl1[i]->minutes = ob.minutes;
	pl1[i]->killer_ship_id = ob.killer_ship_id;
	pl1[i]->killer_cannon_id = ob.killer_cannon_id;
	
	pl1[i]->killed_ships.clear();

	for (int j = 0; j < ob.killed_ships_size; j++)
	{
		pl1[i]->killed_ships.push_back(ob.killed_ships[j]);
	}
	
	
	pl1[i]->score = ob.score;
	pl1[i]->name = ob.name;
	pl1[i]->front_abs_pos = ob.front_abs_pos;
	pl1[i]->rear_abs_pos = ob.rear_abs_pos;
	pl1[i]->dir = ob.dir;
	pl1[i]->radius = ob.radius;
	pl1[i]->bullet_radius = ob.bullet_radius;
	pl1[i]->health = ob.health;
	pl1[i]->gold = ob.gold;
	pl1[i]->died = ob.died;
	pl1[i]->motion = ob.motion;
	pl1[i]->fuel = ob.fuel;
	pl1[i]->invisible = ob.invisible;
	pl1[i]->ammo = ob.ammo;
	//pl1[i]->bullet_pointer = ob.bullet_pointer;
	pl1[i]->tile_pos_front = ob.tile_pos_front;
	pl1[i]->tile_pos_rear = ob.tile_pos_rear;
	pl1[i]->absolutePosition = ob.absolutePosition;

	if (ob.isthere == true)
	{
		pl1[i]->time_line.push_back(ob.time_line);

	}
	//data ends
}
SOCKET connect_to_server()//first connection to the server
{

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	int res = 0;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	struct addrinfo* server_add;
	char buff[1000];
	struct addrinfo* server_add1;
	getaddrinfo(ip_address.c_str(), server_port, &hints, &server_add);
	//convert server_port to int
	int port = atoi(server_port);
	port += 1;
	char server_port1[10];
	sprintf(server_port1, "%d", port);
	getaddrinfo(ip_address.c_str(), server_port1, &hints, &server_add1);
	
	getnameinfo(server_add->ai_addr, server_add->ai_addrlen, buff, sizeof(buff), 0, 0, NI_NUMERICHOST);
	cout << "\n the server address is==>" << buff;
	cout << endl;
	SOCKET peer_socket = socket(server_add->ai_family, server_add->ai_socktype, server_add->ai_protocol);

	recver_socket = socket(server_add1->ai_family, server_add1->ai_socktype, server_add1->ai_protocol);
	
	
	if (!ISVALIDSOCKET(peer_socket))
	{
		cout << "\n socket not created==>" << GETSOCKETERRNO();
	}
	if (!ISVALIDSOCKET(recver_socket))
	{
		cout << "\n socket not created==>" << GETSOCKETERRNO();
	}
	/**/
	connect(peer_socket, server_add->ai_addr, server_add->ai_addrlen);
	
	connect(recver_socket, server_add1->ai_addr, server_add1->ai_addrlen);
	//sending the bytes to the server
	
	string msg = "1";//1 means that  this is client_v2 process
	string tot = msg + to_string(my_id);
	int var = stoi(tot);
	greet_client ob;
	ob.code = var;
	ob.user_cred = user_credentials(username, password);
	strcpy(ob.token, game_token.c_str());

	SEND(recver_socket, (char*)&ob, sizeof(ob));//using the 2nd socket for everything
	
	//receiving from the server how many players will play
	RECV(recver_socket, (char*)&max_players, sizeof(max_players));
	
	cout << "\n data recved from server max player playing are=>" << max_players;
	freeaddrinfo(server_add);
	freeaddrinfo(server_add1);

	return peer_socket;
}
sf::RenderWindow window(sf::VideoMode(::cx(1970), ::cy(1190)), "GREED");
void graphics::GuiRenderer::sound_button_function(graphics::GuiRenderer& gui_renderer, sf::Texture& sound_on, sf::Texture& sound_off)
{
	if (sound)//sound is on
	{
		sound = false;
		gui_renderer.sound_button->getRenderer()->setTexture(sound_off);
	}
	else
	{
		sound = true;
		gui_renderer.sound_button->getRenderer()->setTexture(sound_on);
	}
}
void graphics::callable_clientShow(Mutex* mutx, int code[rows][columns], Map& map_ob)//taking the ship object so as to access the list of the player
{
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

	//sf::RenderWindow window(sf::VideoMode(1970, 1190), "graphics testing");
	tgui::Gui gui(window);

	window.setFramerateLimit(60);
	////////////////////////////////////////////////////////////////////////////////////////////////
	sf::Texture explosion_tex;
	explosion_tex.loadFromFile("map_1 attributes/explosion_set.png");//loading the animation for the explosion
	resizeTexture(explosion_tex);//resizing the textures

	sf::Sprite explosion_sprite;
	explosion_sprite.setTexture(explosion_tex);
	////////////////////////////////////////////////////////////////////////////////////////////////
	List<graphics::animator> animation_list;
	//sf::Sprite rect[2];
	sf::Texture tex_s;
	tex_s.loadFromFile("map_1 attributes/ship_s.png");
	resizeTexture(tex_s);
	cout << "\n size is==>" << tex_s.getSize().x << " " << tex_s.getSize().y;
	sf::Texture tex_n;//texture of north
	tex_n.loadFromFile("map_1 attributes/ship_n.png");
	resizeTexture(tex_n);
	sf::Texture tex_e;
	tex_e.loadFromFile("map_1 attributes/ship_e.png");
	resizeTexture(tex_e);
	sf::Texture tex_w;
	tex_w.loadFromFile("map_1 attributes/ship_w.png");
	resizeTexture(tex_w);
	sf::Texture tex_ne;
	tex_ne.loadFromFile("map_1 attributes/ship_ne.png");
	resizeTexture(tex_ne);
	sf::Texture tex_nw;
	tex_nw.loadFromFile("map_1 attributes/ship_nw.png");
	resizeTexture(tex_nw);
	sf::Texture tex_se;
	tex_se.loadFromFile("map_1 attributes/ship_se.png");
	resizeTexture(tex_se);
	sf::Texture tex_sw;
	tex_sw.loadFromFile("map_1 attributes/ship_sw.png");
	resizeTexture(tex_sw);
	sf::Texture cannon_tex;
	cannon_tex.loadFromFile("map_1 attributes/c0.png");//defualt cannon texture
	resizeTexture(cannon_tex);
	sf::Texture cannon_tex_pack;
	cannon_tex_pack.loadFromFile("map_1 attributes/cannon_set.png");
	resizeTexture(cannon_tex_pack);

	for (int i = 0; i < cannon_list.howMany(); i++)//graphic initializer for the cannon of the map
	{
		cannon_list[i].graphic_initializer(cannon_tex, sf::Vector2f(::cx(cannon_list[i].tile.c * 80 + 40 + origin_x), ::cy(cannon_list[i].tile.r * 80 + 40 + origin_y)));
	}


	for (int i = 0; i < pl1.size(); i++)//graphic initializer for the ship of the user
	{
		pl1[i]->graphics_initializer(tex_n);
	}


	sf::Texture tex;
	map_ob.intialize_graphics(tex);//to set the textures on the map
	sf::RenderStates s;
	s.texture = &tex;
	tileMap ob;
	ob.setScene(columns, rows, origin_x, origin_y, tex, code);

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
	Greed::coords* prev = new Greed::coords[pl1.size()];
	for (int i = 0; i < pl1.size(); i++)//setting the previous position
	{
		prev[i] = pl1[i]->tile_pos_front;
	}
	total_time = 0;
	//handling the gui part from here
	//-----------------GUI----------------------------------
	tgui::Font font_bold("Fonts/bold.ttf");
	tgui::Font font1("Fonts/semiBoldItalic.ttf");
	sf::Font font2;
	font2.loadFromFile("Fonts/text.ttf");
	sf::Texture setting1;
	setting1.loadFromFile("map_1 attributes/settings_blue.png");
	resizeTexture(setting1);
	sf::Texture setting2;
	setting2.loadFromFile("map_1 attributes/settings_red.png");
	resizeTexture(setting2);
	sf::Texture health;
	health.loadFromFile("map_1 attributes/health.png");
	resizeTexture(health);
	sf::Texture fire;
	fire.loadFromFile("map_1 attributes/fire.png");
	resizeTexture(fire);
	sf::Texture gold;
	gold.loadFromFile("map_1 attributes/money1.png");
	resizeTexture(gold);
	sf::Texture fuel;
	fuel.loadFromFile("map_1 attributes/fuel1.png");
	resizeTexture(fuel);
	sf::Texture menu_texture;
	menu_texture.loadFromFile("map_1 attributes/menu.png");
	resizeTexture(menu_texture);
	sf::Texture close_button;
	close_button.loadFromFile("map_1 attributes/close-window.png");
	resizeTexture(close_button);
	GuiRenderer gui_renderer(pl1.size(), &window);
	tgui::Theme::setDefault("TransparentGrey.txt");
	sf::Texture sound_on_tex;
	sound_on_tex.loadFromFile("map_1 attributes/sound-on.png");
	sf::Texture sound_off_tex;
	sound_off_tex.loadFromFile("map_1 attributes/sound-off.png");
	auto group1 = tgui::Group::create();//group for the top stats view 
	auto group2 = tgui::Group::create();//group for the panel view of the stats

	sf::CircleShape bullet_entity;//entity for all the bullets
	bullet_entity.setRadius((cx(5) + cy(5)) / 2);
	bullet_entity.setFillColor(sf::Color::Black);


	//gui function call starts

	gui_renderer.sp_renderer(group1);
	gui_renderer.p_renderer();
	gui_renderer.player_name_renderer(font1);
	gui_renderer.score_renderer1();
	gui_renderer.fire_renderer1(fire);
	gui_renderer.health_renderer(health);
	gui_renderer.gold_renderer1(gold);
	gui_renderer.fuel_renderer1(fuel);
	gui_renderer.score_renderer2();

	gui_renderer.fire_value_renderer();
	gui_renderer.gold_value_renderer();

	gui_renderer.health_value_renderer();
	gui_renderer.fuel_value_renderer();
	gui_renderer.menu_button_renderer(menu_texture, gui);
	gui_renderer.closeButton_renderer(close_button);
	gui_renderer.stats_view_renderer();
	gui_renderer.stats_view_panel_renderer(group2);
	gui_renderer.name_id_renderer(gui, font1);
	gui_renderer.time_line_text_renderer(gui, font_bold);
	gui_renderer.gameOverRenderer(font2);
	gui_renderer.waitRenderer(font2);
	gui_renderer.winner_renderer(font2);
	gui_renderer.timer_renderer(gui);
	gui_renderer.setting_button_renderer(setting1, setting2, gui);

	gui_renderer.health_of_cannon_renderer(gui, font_bold);
	gui_renderer.final_window_renderer(gui);
	
	gui_renderer.sound_button_renderer(sound_on_tex);
	//gui_renderer.sound_button->onPress(&graphics::GuiRenderer::sound_button_function,this, gui_renderer, sound_on_tex, sound_off_tex);
	gui_renderer.sound_button->onPress([&gui_renderer, &sound_on_tex, &sound_off_tex]() {
		if (sound == true) {
			sound = false;
			gui_renderer.sound_button->getRenderer()->setTexture(sound_off_tex);
		}
		else if (sound == false)
		{
			sound = true;
			gui_renderer.sound_button->getRenderer()->setTexture(sound_on_tex);
		}
		});

	//gui_renderer.childWindowRenderer(gui);
	//gui function calls ends
	for (int i = 0; i < pl1.size(); i++)
	{
		gui_renderer.player_name[i]->setText(pl1[i]->name + "-" + std::to_string(pl1[i]->ship_id));

	}
	//call stats_view_renderer
	gui_renderer.time_line_renderer(gui);

	gui.add(group2);
	gui.add(group1, "first group of widgets");
	//mover_tgui<tgui::RadioButton::Ptr> moveit;
	sf::Text name_of_ship;
	sf::Font daemon;
	daemon.loadFromFile("Fonts/text.ttf");
	name_of_ship.setFont(daemon);
	name_of_ship.setCharacterSize((::cx(30) + ::cy(30)) / 2);
	//moveit.moving(window,gui,gui_renderer.tb1);
	//write a function to check if a number is even or odd
	deque<int> dead_ship;//id of the dead ships
	bool gameOver = false;
	bool display = true;
	vector<int> winner;//id's of the winner of the game, this is a vector in case of a tie
	int var = 0;
	deque<int> deColor;//to decolor the tiles after illuminating them
	deque<ship*> duplicate_pl1;//duplicate for pl1

	//starting the networking part from here
	//calling to connect to the server
	

	fd_set master;
	FD_ZERO(&master);
	FD_SET(recver_socket, &master);
	fd_set temp_set;
	FD_ZERO(&temp_set);
	
	//Greed::abs_pos prev1 = Greed::abs_pos(pl1[0]->rect.getPosition().x,pl1[0]->rect.getPosition().y);
	Greed::abs_pos prev1 = pl1[0]->getAbsolutePosition();
	
	window.setFramerateLimit(60);
	double previous = -1;
	double previous_angle = 0;
	double avg_bullet = 0;
	top_layer ship_data;

	int previous_packet = -1;
	bool started = false;//to check if the game has started or not;
	int prev_x=0, prev_y = 0;
	int prev_packet = 0;

	//sending something from recver_socket to check the connection endpoint
	sf::Music music;
	if (!music.openFromFile("solo_by_river.wav"))
	{
		cout << "\n error in loading the music file..";
	}
	music.setLoop(true);
	music.play();
	while (window.isOpen())
	{
		if (!sound)
		{
			music.pause();
		}
		if (sound && music.getStatus() == sf::Music::Paused)
		{
			music.play();
		}
		temp_set = master;
		

		c++;
		frames++;
		sf::Event event;
		sf::Time deltime = clock.restart();
		elapsed_time += deltime.asSeconds();
		total_secs += deltime.asSeconds();
		//total_time += deltime.asSeconds();
		total_time++;
		t2 += deltime.asSeconds();
		if (t2 >= 1)
		{
			cout << "\n frame rate is==>" << c;
					
			c = 0;
			t2 = 0;
		}
		
		while (window.pollEvent(event))
		{
			gui.handleEvent(event);
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				window.close();

			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
			{
				window.setSize(sf::Vector2u(1920, 1080));
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));

			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
			{
				window.setSize(sf::Vector2u(1970, 1190));
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));

			}

			if (event.type == sf::Event::Resized)
			{
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));

			}


		}

		//setting the timer here
		//take the data from the server
		/*
		* data to be taken...
		* 1. location of every ship
		* 2. time left(time will work according to the server
		*
		*/
	
		int gone = 0;
		int found = 0;
		for (int i = 0; i < cannon_list.howMany(); i++)
		{
			if (cannon_list[i].isDead == false)
			{
				if (cannon_list[i].cannon_sprite.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					gui_renderer.hoc->setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y - 20);
					gui_renderer.hoc->setText("Health: " + to_string((int)cannon_list[i].health));
					gui_renderer.hoc->setVisible(true);
					found = 1;

				}


			}
		}
		if (found == 0)
		{
			gui_renderer.hoc->setVisible(false);
		}
		for (int i = 0; i < pl1.size(); i++)
		{
			if (pl1[i]->getDiedStatus() == 0)
			{
				if (pl1[i]->rect.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) == true)
				{
					gui_renderer.player_panels[i]->getRenderer()->setBackgroundColor(sf::Color(0, 204, 122));//change the panel color to green
					//now illuminating the radius of the ship chosen
				}
				else
				{
					//gui_renderer.player_panels[i]->getRenderer()->setOpacity(1);
					gui_renderer.player_panels[i]->getRenderer()->setBackgroundColor(sf::Color::Black);
					int s = deColor.size();

				}
			}
		}

			//receiving the data


			struct timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 10;

			if (select(recver_socket + 1, &temp_set, NULL, NULL, &timeout) < 0)
			{
				cout << "\n error in select";
			}
			
			int bytes_received = -1;
			if (FD_ISSET(recver_socket,&temp_set))
			{
				memset((void*)&ship_data, 0, sizeof(ship_data));
				//receiving the data
				gone = 1;
				bytes_received = recv(recver_socket, (char*)&ship_data, sizeof(ship_data), 0);
				if (bytes_received < 1)
				{
					cout << "\n server disconnected the connection";
					CLOSESOCKET(peer_socket);
					continue;
				}
				
				if (1)
				{
					if (abs(ship_data.ob[1].absolutePosition.x - prev_x) > 2 || abs(ship_data.ob[1].absolutePosition.y - prev_y) > 2)
					{
						cout << "\n discrepency in position of the ship at==>" << ship_data.packet_no << " packet difference is==>" << ship_data.packet_no - previous;

					}
					//cout << "\n position of 0th ship==>" << ship_data.ob[0].absolutePosition.x << " " << ship_data.ob[0].absolutePosition.y;
					prev_x = ship_data.ob[1].absolutePosition.x;
					prev_y = ship_data.ob[1].absolutePosition.y;

					double diff = ship_data.packet_no - previous;
					if (diff > 1)
					{
						//cout << "\n difference is greater than 1=>" << diff << " " << previous;
					}

					previous = ship_data.packet_no;
					gameOver = ship_data.gameOver;
					started = true;
					auto now = std::chrono::system_clock::now();
					auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
					auto secs = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()) % 60;
					auto mins = std::chrono::duration_cast<std::chrono::minutes>(now.time_since_epoch()) % 60;
					auto hours = std::chrono::duration_cast<std::chrono::hours>(now.time_since_epoch());

					//	cout << "\n recved data from the server==>" << ship_data.packet_no << " at the time==> " <<
							//hours.count() << ":" << mins.count() << ":" << secs.count() << ":" << ms.count() << endl;
				}
				
			}

			/*code to update the timer*/
			int min = ship_data.total_secs / 60;
			int sec = (int)ship_data.total_secs % 60;
			gui_renderer.timer->setText(std::to_string(min) + ":" + std::to_string(sec));
			/*code to update the timer ends*/
			for (int i = 0; i < pl1.size(); i++)
			{

				if (bytes_received > 0)
				{
					update_frame(pl1, ship_data.ob[i], i);//to unwrap the data into the ship object
					previous_packet = ship_data.packet_no;
				}


				pl1[i]->rect.setPosition(::cx(pl1[i]->absolutePosition.x + origin_x), ::cy(pl1[i]->absolutePosition.y + origin_y));


				if (pl1[i]->dir == Direction::NORTH)
				{
					pl1[i]->rect.setTexture(tex_n);
				}
				else if (pl1[i]->dir == Direction::SOUTH)
				{
					pl1[i]->rect.setTexture(tex_s);
				}
				else if (pl1[i]->dir == Direction::EAST)
				{
					pl1[i]->rect.setTexture(tex_e);
				}
				else if (pl1[i]->dir == Direction::WEST)
				{
					pl1[i]->rect.setTexture(tex_w);
				}
				else if (pl1[i]->dir == Direction::NORTH_EAST)
				{
					pl1[i]->rect.setTexture(tex_ne);
				}
				else if (pl1[i]->dir == Direction::SOUTH_EAST)
				{
					pl1[i]->rect.setTexture(tex_se);

				}
				else if (pl1[i]->dir == Direction::NORTH_WEST)
				{
					pl1[i]->rect.setTexture(tex_nw);
				}
				else if (pl1[i]->dir == Direction::SOUTH_WEST)
				{
					pl1[i]->rect.setTexture(tex_sw);
				}
				if (pl1[i]->died == 1)
				{
					gui_renderer.player_panels[i]->getRenderer()->setBackgroundColor(sf::Color::Red);
					gui_renderer.player_panels[i]->getRenderer()->setOpacity(0.6);
				}


			}
			//for rendering and updating the visual stats in the game


			//bullet working for the cannon



			for (int i = 0; i < pl1.size(); i++)
			{
				string s = to_string((int)pl1[i]->getCurrentHealth());
				//updating the gui
				gui_renderer.score_value[i]->setText(to_string(pl1[i]->score));
				gui_renderer.health_value[i]->setText(s);
				gui_renderer.fire_value[i]->setText(to_string((int)pl1[i]->getCurrentAmmo()));
				gui_renderer.gold_value[i]->setText(to_string((int)pl1[i]->getCurrentGold()));
				gui_renderer.fuel_value[i]->setText(to_string((int)pl1[i]->getCurrentFuel()));
				gui_renderer.player_name[i]->setText(pl1[i]->name + "-" + std::to_string(pl1[i]->ship_id));
				if (pl1[i]->died == 1)
				{
					gui_renderer.player_panels[i]->getRenderer()->setBackgroundColor(sf::Color::Red);
					gui_renderer.player_panels[i]->getRenderer()->setOpacity(0.6);
				}
			}//for updating the upper boxes
			//to update the timeline listbox

			//writing code to implement the stats feature
			if (gui_renderer.m.try_lock())
			{
				if (gui_renderer.s_id != -1)
				{
					//cout << "\n panel opened for==>" << gui_renderer.s_id;
					string health_str = to_string(pl1[gui_renderer.s_id]->getCurrentHealth());
					const tgui::String h = static_cast<tgui::String>(health_str);
					gui_renderer.name_id->setText(pl1[gui_renderer.s_id]->name + "-" + to_string(pl1[gui_renderer.s_id]->ship_id));

					gui_renderer.stats_view->changeSubItem(0, 2, h);
					//updating gold value
					string gold_str = to_string(pl1[gui_renderer.s_id]->getCurrentGold());
					const tgui::String g = static_cast<tgui::String>(gold_str);
					gui_renderer.stats_view->changeSubItem(1, 2, gold_str);
					//updating Fuel value
					string fuel_str = to_string(pl1[gui_renderer.s_id]->getCurrentFuel());
					const tgui::String f = static_cast<tgui::String>(fuel_str);
					gui_renderer.stats_view->changeSubItem(2, 2, f);
					//updating ammo value
					string ammo_str = to_string(pl1[gui_renderer.s_id]->getCurrentAmmo());
					const tgui::String a = static_cast<tgui::String>(ammo_str);
					gui_renderer.stats_view->changeSubItem(3, 2, a);
					//updating tile coords(front)
					string tcfr = to_string(pl1[gui_renderer.s_id]->getCurrentTile().r);
					string tcfc = to_string(pl1[gui_renderer.s_id]->getCurrentTile().c);
					string fin = "Row: " + tcfr + " " + "Col.: " + tcfc;
					const tgui::String tcf = static_cast<tgui::String>(fin);
					gui_renderer.stats_view->changeSubItem(4, 2, tcf);
					//updating tile coords(back)
					string tcbfr = to_string(pl1[gui_renderer.s_id]->getCurrentRearTile().r);
					string tcbfc = to_string(pl1[gui_renderer.s_id]->getCurrentRearTile().c);
					string fin2 = "Row: " + tcbfr + " " + "Col.: " + tcbfc;
					const tgui::String tcb = static_cast<tgui::String>(fin2);
					gui_renderer.stats_view->changeSubItem(5, 2, tcb);
					//updating abs pos front
					string apfr = to_string((int)pl1[gui_renderer.s_id]->getAbsolutePosition(ShipSide::FRONT).x);
					string apfc = to_string((int)pl1[gui_renderer.s_id]->getAbsolutePosition(ShipSide::FRONT).y);
					string fin3 = "X: " + apfr + " " + "Y: " + apfc;
					const tgui::String apf = static_cast<tgui::String>(fin3);
					gui_renderer.stats_view->changeSubItem(6, 2, apf);
					//updating abs pos back
					string apbfr = to_string((int)pl1[gui_renderer.s_id]->getAbsolutePosition(ShipSide::REAR).x);
					string apbfc = to_string((int)pl1[gui_renderer.s_id]->getAbsolutePosition(ShipSide::REAR).y);
					string fin4 = "X: " + apbfr + " " + "Y: " + apbfc;
					const tgui::String apb = static_cast<tgui::String>(fin4);
					gui_renderer.stats_view->changeSubItem(7, 2, apb);
					//updating direction

					if (pl1[gui_renderer.s_id]->getShipDirection() == Direction::NORTH)
					{
						const tgui::String dir = static_cast<tgui::String>("NORTH");
						gui_renderer.stats_view->changeSubItem(8, 2, dir);
					}
					else if (pl1[gui_renderer.s_id]->getShipDirection() == Direction::EAST)
					{
						const tgui::String dir = static_cast<tgui::String>("EAST");
						gui_renderer.stats_view->changeSubItem(8, 2, dir);
					}
					else if (pl1[gui_renderer.s_id]->getShipDirection() == Direction::WEST)
					{
						const tgui::String dir = static_cast<tgui::String>("WEST");
						gui_renderer.stats_view->changeSubItem(8, 2, dir);
					}
					else if (pl1[gui_renderer.s_id]->getShipDirection() == Direction::SOUTH)
					{
						const tgui::String dir = static_cast<tgui::String>("SOUTH");
						gui_renderer.stats_view->changeSubItem(8, 2, dir);
					}
					else if (pl1[gui_renderer.s_id]->getShipDirection() == Direction::NORTH_EAST)
					{
						const tgui::String dir = static_cast<tgui::String>("NORTH_EAST");
						gui_renderer.stats_view->changeSubItem(8, 2, dir);
					}
					else if (pl1[gui_renderer.s_id]->getShipDirection() == Direction::NORTH_WEST)
					{
						const tgui::String dir = static_cast<tgui::String>("NORTH_WEST");
						gui_renderer.stats_view->changeSubItem(8, 2, dir);
					}
					else if (pl1[gui_renderer.s_id]->getShipDirection() == Direction::SOUTH_EAST)
					{
						const tgui::String dir = static_cast<tgui::String>("SOUTH_EAST");
						gui_renderer.stats_view->changeSubItem(8, 2, dir);
					}
					else if (pl1[gui_renderer.s_id]->getShipDirection() == Direction::SOUTH_WEST)
					{
						const tgui::String dir = static_cast<tgui::String>("SOUTH_WEST");
						gui_renderer.stats_view->changeSubItem(8, 2, dir);
					}
					//updating the radius
					string rad = to_string(pl1[gui_renderer.s_id]->getShipRadius());
					const tgui::String r = static_cast<tgui::String>(rad);
					gui_renderer.stats_view->changeSubItem(9, 2, r);
					//updating cannon radius
					string crad = to_string(pl1[gui_renderer.s_id]->bullet_radius);
					const tgui::String cr = static_cast<tgui::String>(crad);
					gui_renderer.stats_view->changeSubItem(10, 2, cr);
					//updating cannnon power
					string cpow = to_string(pl1[gui_renderer.s_id]->cannon_ob.power);
					const tgui::String cpo = static_cast<tgui::String>(cpow);
					gui_renderer.stats_view->changeSubItem(11, 2, cpo);
					//updating the died status
					if (pl1[gui_renderer.s_id]->getDiedStatus() == 1)
					{
						const tgui::String str = "YES";
						gui_renderer.stats_view->changeSubItem(12, 2, str);
					}
					else if (pl1[gui_renderer.s_id]->getDiedStatus() == 0)
					{
						const tgui::String str = "NO";
						gui_renderer.stats_view->changeSubItem(12, 2, str);
					}
					//updating the shield status
					if (pl1[gui_renderer.s_id]->shield == 1)
					{
						const tgui::String str = "YES";
						gui_renderer.stats_view->changeSubItem(13, 2, str);
					}
					else if (pl1[gui_renderer.s_id]->shield == 0)
					{
						const tgui::String str = "NO";
						gui_renderer.stats_view->changeSubItem(13, 2, str);
					}
					//done updating the stats about the ship
					if (mutx->timeMutex[gui_renderer.s_id].try_lock())//no need to lock the mutex here
					{

						int i = gui_renderer.s_id;
						if (pl1[i]->time_line.size() > 0)
						{
							if (pl1[i]->time_line[0].eventype == timeline::EventType::GOLD_TO_HEALTH)
							{
								vector<tgui::String> arr;
								arr.push_back(static_cast<tgui::String>(to_string(pl1[i]->time_line[0].timestamp)));
								string s = "Gold->Health. n=>";
								string s1 = to_string((int)pl1[i]->time_line[0].h.n) + " o=>" + to_string((int)pl1[i]->time_line[0].h.old);
								arr.push_back(static_cast<tgui::String>(s + s1));
								gui_renderer.timeline[i]->addItem(arr);
							}
							else if (pl1[i]->time_line[0].eventype == timeline::EventType::GOLD_TO_FUEL)
							{
								vector<tgui::String> arr;
								arr.push_back(static_cast<tgui::String>(to_string(pl1[i]->time_line[0].timestamp)));
								string s = "Gold->Fuel. n=>";
								string s1 = to_string((int)pl1[i]->time_line[0].f.n) + " o=>" + to_string((int)pl1[i]->time_line[0].f.old);
								arr.push_back(static_cast<tgui::String>(s + s1));
								gui_renderer.timeline[i]->addItem(arr);
							}
							else if (pl1[i]->time_line[0].eventype == timeline::EventType::GOLD_TO_AMMO)
							{
								vector<tgui::String> arr;
								arr.push_back(static_cast<tgui::String>(to_string(pl1[i]->time_line[0].timestamp)));
								string s = "Gold->Ammo. n=>";
								string s1 = to_string((int)pl1[i]->time_line[0].a.n) + " o=>" + to_string((int)pl1[i]->time_line[0].a.old);
								arr.push_back(static_cast<tgui::String>(s + s1));
								gui_renderer.timeline[i]->addItem(arr);
							}
							else if (pl1[i]->time_line[0].eventype == timeline::EventType::DEFEATED_SHIP)
							{
								vector<tgui::String> arr;
								arr.push_back(static_cast<tgui::String>(to_string(pl1[i]->time_line[0].timestamp)));
								string s = "Defeated ship=>" + pl1[pl1[i]->time_line[0].d.id]->name + "(" + to_string(pl1[pl1[i]->time_line[0].d.id]->ship_id) + ")";
								arr.push_back(s);
								gui_renderer.timeline[i]->addItem(arr);
							}
							else if (pl1[i]->time_line[0].eventype == timeline::EventType::ANCHOR_SHIP)
							{
								vector<tgui::String> arr;
								arr.push_back(static_cast<tgui::String>(to_string(pl1[i]->time_line[0].timestamp)));
								string s = "Ship is anchored";
								arr.push_back(s);
								gui_renderer.timeline[i]->addItem(arr);
							}
							else if (pl1[i]->time_line[0].eventype == timeline::EventType::SHIP_DIED)
							{
								vector<tgui::String> arr;
								arr.push_back(static_cast<tgui::String>(to_string(pl1[i]->time_line[0].timestamp)));
								string s = "Ship is defeated";
								arr.push_back(s);
								gui_renderer.timeline[i]->addItem(arr);
							}
							pl1[i]->time_line.pop_front();

						}
						mutx->timeMutex[i].unlock();
					}

				}
				gui_renderer.m.unlock();
			}//for updating the timeline boxes



			//drawing and animation part starts here
			//cout << "\n testing hello";
			window.clear();

			window.draw(ob.tile, s);
			for (int i = 0; i < pl1.size(); i++)
			{
				if (pl1[i]->died == 0)
				{
					window.draw(pl1[i]->rect);
					//drawing the names of the ships over them 
					name_of_ship.setPosition(::cx(pl1[i]->absolutePosition.x - 5 + origin_x), ::cy(pl1[i]->absolutePosition.y - 10 + origin_y));
					name_of_ship.setString(static_cast<sf::String>(pl1[i]->name));
					window.draw(name_of_ship);
				}

			}
			//rendering the bullet here

			for (int i = 0; i < ship_data.no_of_bullets; i++)
			{
				if (!(ship_data.bullet_pos[i].x == 0 && ship_data.bullet_pos[i].y == 0))
				{
					avg_bullet += ship_data.no_of_bullets;
					bullet_entity.setPosition(::cx(ship_data.bullet_pos[i].x + origin_x), ::cy(ship_data.bullet_pos[i].y + origin_y));

					//cout << "\n mistake caught at=>" << i;
				}

				window.draw(bullet_entity);
			}



			//setting the base sprite on the cannon


			//printing the cannon animation
			for (int i = 0; i < cannon_list.howMany(); i++)
			{
				//setting cannon data here
				cannon_list[i].current_angle = ship_data.cannon_ob[i].angle;
				cannon_list[i].health = ship_data.cannon_ob[i].cannon_health;
				cannon_list[i].isDead = ship_data.cannon_ob[i].is_cannon_dead;

			}

			for (int i = 0; i < cannon_list.howMany(); i++)
			{
				if (cannon_list[i].isDead == 0)
				{
					//cannon_list[i].cannon_sprite.setRotation(cannon_list[i].current_angle);
					cannon_list[i].cannon_sprite.setTexture(cannon_tex_pack);
					cannon_list[i].cannon_sprite.setTextureRect(sf::IntRect(::cx(ship_data.cannon_ob[i].picture * 80), 0, ::cx(80), ::cy(80)));

					cannon_list[i].cannon_sprite.setRotation(ship_data.cannon_ob[i].angle);
					//cout << "\n angle is-=>" << ship_data.cannon_ob[2].angle;
					window.draw(cannon_list[i].cannon_sprite);
				}
			}

			for (int i = 0; i < ship_data.no_of_animation; i++)//printing the animations
			{
				explosion_sprite.setPosition(ship_data.animation_ob[i].x, ship_data.animation_ob[i].y);
				explosion_sprite.setTextureRect(sf::IntRect(::cx(ship_data.animation_ob[i].picture * 30), 0, ::cx(30), ::cy(30)));
				window.draw(explosion_sprite);
			}
			ob.setScene(columns, rows, origin_x, origin_y, tex, code);
			if (gui_renderer.showRadius)//to show radius of all the ships
			{

				for (int i = 0; i < pl1.size(); i++)
				{
					if (pl1[i]->getDiedStatus() == 0)
					{
						vector<Greed::coords> l = pl1[i]->getRadiusCoords_ship(i);
						for (int j = 0; j < l.size(); j++)
						{
							int ind = columns * l[j].r * 4 + 4 * l[j].c;
							ob.tile[ind].color = sf::Color::Green;
							ob.tile[ind + 2].color = sf::Color::Green;
							deColor.push_back(ind);

						}
					}
				}
			}
		
			if (gameOver && ran == 0)
			{
				findWinner(pl1);

				gui_renderer.final_window->setVisible(true);
				for (int i = 0; i < pl1.size(); i++)
				{
					vector<tgui::String> data;
					data.push_back(static_cast<tgui::String>(i + 1));
					data.push_back(static_cast<tgui::String>(pl1[i]->name));
					data.push_back(static_cast<tgui::String>(pl1[i]->score));
					data.push_back(static_cast<tgui::String>(pl1[i]->killed_ships.size()));
					if (pl1[i]->minutes == INT_MAX && pl1[i]->seconds == INT_MAX)
					{
						data.push_back("Not Defeated");
					}
					else
						data.push_back(static_cast<tgui::String>(to_string(pl1[i]->minutes) + ":" + to_string(pl1[i]->seconds)));
					gui_renderer.list1->addItem(data);
				}
				ran = 1;


			}
			if (!started)
			{
				window.draw(gui_renderer.wait);
			}
			if (gameOver)
			{
				window.draw(gui_renderer.gameOver);
			}
			gui.draw();


			window.display();

		

	}
	CLOSESOCKET(peer_socket);
	CLOSESOCKET(recver_socket);
	cout << "\n avg bullet is=>" << avg_bullet / total_time;
}

int main(int argc,char* argv[])//1st is port, 2nd is id, 3rd is username, 4th is password
{
#if defined(_WIN32)
	WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d))
	{
		cout << "\n failed to initialize";
	}
#endif // defined
	//extracting the data
	
	//setting the port number of the server that has to connected with
	
	if (argc > 5)
	{
		strcpy(server_port, argv[1]);
			std::cout << "Received value from the parent process: " << server_port << std::endl;
			cout << "\n id is=>" << argv[2];
			my_id = stoi(argv[2]);
			username = argv[3];
			password = argv[4];
			game_token = argv[5];
			ip_address = argv[6];
	}
	/*
	* to run this file manually
	strcpy(server_port, "8081");
	my_id = 1;
	username = "username";
	password = "password";
	cout << "\n enter the game token=>";
	cin >> game_token;
	ip_address = "127.0.0.1";
	*/
	//connecting with the server_v3 here
	peer_socket = connect_to_server();

	cout << "\n max player are==>" << max_players;
	const int no_of_players = max_players;
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
	ship player[11];


	//player[1].updateCost(Map::coords(10,0), 50);

	//player[1].localMap[10 * columns + 0].b.cost = 50;
	deque<ship*> slist;

	for (int i = 0; i < no_of_players; i++)
	{
		silist.push_back(shipInfo(&player[i]));
		slist.push_back(&player[i]);
	}
	//generating the coords for spawning the ship
	vector<Greed::coords> spawn(max_players,Greed::coords(0,0));//spawning position for ships having different configurations
	
	for (int i = 0; i < no_of_players; i++)
	{
		player[i].initialize_player(i,"hawk", &mutx, silist, code, 5, spawn[i]);//last element is the tile pos of the player

	}
	
	control.setShipList(slist, 2369);
	// player[1].localMap[10 * columns + 0].b.cost = 50;
	//making the cannon lists:
	//let there be 1 cannon only 
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
	cg.callable_clientShow(&mutx, code, map1);
	//display();

	
}
