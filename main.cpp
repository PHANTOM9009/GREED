
int main()
{
	//extracting the data

	ifstream in("user_info.dat", ios::in | ios::binary);
	vector<fileData> data;
	while (in)
	{
		fileData temp;
		in.read((char*)&temp, sizeof(temp));
		if (in.eof())
		{
			break;
		}

		data.push_back(temp);

	}
	const int no_of_players = data[0].no_of_players;
	cout << "\n number of players==>" << no_of_players;
	for (int i = 0; i < data.size(); i++)
	{
		cout << data[i].player_name;
	}
	in.close();

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
	vector<Greed::coords> spawn;//spawning position for ships having different configurations
	if (no_of_players == 1)//for 0 players
	{
		spawn.push_back(Greed::coords(10, 11));

	}
	else if (no_of_players == 2)
	{

		spawn.push_back(Greed::coords(1, 0));
		spawn.push_back(Greed::coords(1, 23));

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
	for (int i = 0; i < no_of_players; i++)
	{
		player[i].initialize_player(data[i].player_name, &mutx, silist, code, 5, spawn[i]);//last element is the tile pos of the player

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
	thread t[10];
	if (no_of_players == 1)
	{
		t[0] = thread(user1::GreedMain, ref(player[0]));

	}
	else if (no_of_players == 2)
	{
		t[0] = thread(user1::GreedMain, ref(player[0]));
		t[1] = thread(user2::GreedMain, ref(player[1]));
	}
	else if (no_of_players == 3)
	{
		t[0] = thread(user1::GreedMain, ref(player[0]));
		t[1] = thread(user2::GreedMain, ref(player[1]));
		t[2] = thread(user3::GreedMain, ref(player[2]));
	}
	else if (no_of_players == 4)
	{
		t[0] = thread(user1::GreedMain, ref(player[0]));
		t[1] = thread(user2::GreedMain, ref(player[1]));
		t[2] = thread(user3::GreedMain, ref(player[2]));
		t[3] = thread(user4::GreedMain, ref(player[3]));
	}
	else if (no_of_players == 5)
	{
		t[0] = thread(user1::GreedMain, ref(player[0]));
		t[1] = thread(user2::GreedMain, ref(player[1]));
		t[2] = thread(user3::GreedMain, ref(player[2]));
		t[3] = thread(user4::GreedMain, ref(player[3]));
		t[4] = thread(user5::GreedMain, ref(player[4]));
	}
	else if (no_of_players == 6)
	{
		t[0] = thread(user1::GreedMain, ref(player[0]));
		t[1] = thread(user2::GreedMain, ref(player[1]));
		t[2] = thread(user3::GreedMain, ref(player[2]));
		t[3] = thread(user4::GreedMain, ref(player[3]));
		t[4] = thread(user5::GreedMain, ref(player[4]));
		t[5] = thread(user6::GreedMain, ref(player[5]));
	}
	else if (no_of_players == 7)
	{
		t[0] = thread(user1::GreedMain, ref(player[0]));
		t[1] = thread(user2::GreedMain, ref(player[1]));
		t[2] = thread(user3::GreedMain, ref(player[2]));
		t[3] = thread(user4::GreedMain, ref(player[3]));
		t[4] = thread(user5::GreedMain, ref(player[4]));
		t[5] = thread(user6::GreedMain, ref(player[5]));
		t[6] = thread(user7::GreedMain, ref(player[6]));
	}
	else if (no_of_players == 8)
	{
		t[0] = thread(user1::GreedMain, ref(player[0]));
		t[1] = thread(user2::GreedMain, ref(player[1]));
		t[2] = thread(user3::GreedMain, ref(player[2]));
		t[3] = thread(user4::GreedMain, ref(player[3]));
		t[4] = thread(user5::GreedMain, ref(player[4]));
		t[5] = thread(user6::GreedMain, ref(player[5]));
		t[6] = thread(user7::GreedMain, ref(player[6]));
		t[7] = thread(user8::GreedMain, ref(player[7]));
	}
	else if (no_of_players == 9)
	{
		t[0] = thread(user1::GreedMain, ref(player[0]));
		t[1] = thread(user2::GreedMain, ref(player[1]));
		t[2] = thread(user3::GreedMain, ref(player[2]));
		t[3] = thread(user4::GreedMain, ref(player[3]));
		t[4] = thread(user5::GreedMain, ref(player[4]));
		t[5] = thread(user6::GreedMain, ref(player[5]));
		t[6] = thread(user7::GreedMain, ref(player[6]));
		t[7] = thread(user8::GreedMain, ref(player[7]));
		t[8] = thread(user9::GreedMain, ref(player[8]));
	}
	else if (no_of_players == 10)
	{
		t[0] = thread(user1::GreedMain, ref(player[0]));
		t[1] = thread(user2::GreedMain, ref(player[1]));
		t[2] = thread(user3::GreedMain, ref(player[2]));
		t[3] = thread(user4::GreedMain, ref(player[3]));
		t[4] = thread(user5::GreedMain, ref(player[4]));
		t[5] = thread(user6::GreedMain, ref(player[5]));
		t[6] = thread(user7::GreedMain, ref(player[6]));
		t[7] = thread(user8::GreedMain, ref(player[7]));
		t[8] = thread(user9::GreedMain, ref(player[8]));
		t[9] = thread(user10::GreedMain, ref(player[9]));
	}
	for (int i = 0; i < no_of_players; i++)
	{
		t[i].detach();
	}





	graphics cg;
	cg.callable(&mutx, code, map1);


	char pBuf[256];
	size_t length = sizeof(pBuf);
	int bytes = GetModuleFileNameA(NULL, pBuf, length);
	cout << "\n file path is==>" << pBuf;

	//code to extract the path of the directory in which the exe is
	string path;
	path.append(pBuf);
	for (int i = path.length() - 1; i >= 0; i--)
	{
		if (path[i] == '\\')
		{
			path.erase(i, path.length());
			break;
		}
	}
	cout << "\n directory is==>" << path;



}
