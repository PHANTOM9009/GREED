
#pragma once
#define NOMINMAX

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

#include<thread>
#include<mutex>
#include<conio.h>
#include<math.h>
#include<string>
#include<regex>
#include<Windows.h>
#include<fstream>
#include<list>
#include<vector>
#include<iterator>
#include<map>
#include<algorithm>
#include<deque>
#include<unordered_map>
#include<windows.h>

#include<chrono>
#include<condition_variable>
#pragma once

#define columns 24
#define rows 12
#define WATER_COST 1
#define STORM_COST 5
#define len 80
#define breadth 80
#define origin_x 16
#define origin_y 200//200
#define fire_offset 20
#define PI 3.14159265
#define CANNON_POWER 5
#define DEF_FUEL 50
#define DEF_AMMO 50
#define EVENT_T 0.0498
#define origin_offset_screen_x 16
#define origin_offset_screen_y 19
#define INITIAL_GOLD 1000//initial gold that everyone is having
#define GOLD_OFF_A_BULLET 2//money that is gone at hit of a bullet..
//these are the total shifts in the origin

/*REQUIRED:
* Tile has to be a square unless navigation functions wont work
* Origin is (17,19)
*/
#define MAX_LENGTH 1000
//list class is incoming here::
using namespace std;

enum iterate
{
	forward = 0, backward = 1
};

template<typename T>
class List
{
public:

	List* next;//storing the address of the next record: NULL if its the first record
	List* previous;//storing the address of the previous record
	T s;//object to be stored
	List* Front;//stores the address of the first entry of the structure
	List* rear;// stores the address of the last entry of the structure

	List()//default ctor
	{

		next = previous = Front = rear = NULL;
	}
	bool add_rear(const T& ob)//returns true if it appends properly
	{
		List* node = new List;
		if (node == NULL)
		{
			cout << "\n error: memory not given";
			return false;
		}
		if (Front == NULL && rear == NULL)
		{
			Front = node;
		}
		node->next = NULL;
		node->previous = rear;
		node->s = ob;
		if (rear != NULL)
			rear->next = node;

		rear = node;
		return true;
	}
	bool add_front(const T& ob)
	{
		List* node = new List;
		if (node == NULL)
		{
			cout << "\n error: memory not given";
			return false;
		}
		if (Front == NULL && rear == NULL)
		{
			rear = node;
		}
		node->previous = NULL;
		node->next = Front;
		node->s = ob;
		if (Front != NULL)
			Front->previous = node;

		Front = node;
		return true;
	}
	//testing function
	void display(iterate where)//to display the entire structre
	{
		if (where == iterate::forward)
		{
			List* temp = Front;
			while (temp != NULL)
			{
				cout << temp->s << endl;
				temp = temp->next;
			}
			delete temp;
		}
		else if (where == iterate::backward)
		{
			List* temp = rear;
			while (temp != NULL)
			{
				cout << temp->s << endl;
				temp = temp->previous;
			}
			delete temp;
		}

	}
	int howMany()//function to tell the number of entries in the structure
	{
		List* temp = Front;
		int i = 0;
		while (temp != NULL)
		{
			temp = temp->next;
			i++;
		}
		delete temp;
		return i;
	}
	bool addAt(const T& ob, int pos)
	{

		if (pos == 0)
		{
			add_front(ob);
			return true;
		}

		else if ((howMany() + 1) == (pos + 1))
		{

			add_rear(ob);
			return true;
		}

		else if (pos > 0 && pos < howMany())
		{
			int i = 0;
			List* temp = Front;
			while (i <= (pos - 2))
			{
				temp = temp->next;
				i++;

			}
			//temp has the address of the preceding entry

			List* node = new List;
			node->next = temp->next;
			node->previous = temp;
			node->s = ob;

			temp->next = node;
			node->next->previous = node;
			//cout<<temp->next<<endl;

			temp = NULL;
			delete temp;
			//cout<<Front->next;
			return true;
		}
		else
		{

			cout << "\n error: wrong position\n value not added\n";
			return false;
		}
	}
	void del_front()//deleting elements from the front
	{
		if (Front == NULL)
		{
			cout << "\n error: nothing to delete";
		}
		List* temp = Front;
		if (howMany() == 1)
		{
			Front = rear = NULL;
		}
		else
		{
			Front = Front->next;
			Front->previous = NULL;
		}
		delete temp;


	}
	void del_rear()//deleting elements from the rear side
	{
		if (rear == NULL)
		{
			cout << "\n error:nothing to delete";

		}
		List* temp = rear;
		if (howMany() == 1)
		{
			rear = Front = NULL;
		}
		else
		{
			rear = rear->previous;
			rear->next = NULL;
		}
		delete temp;
	}
	void delAt(int pos)//position starts from zero
	{
		if (Front == NULL)
		{
			cout << "\n error: nothing to delete";
			return;
		}
		if (pos == 0)
		{
			del_front();
			return;
		}
		else if (howMany() - 1 == pos)
		{
			del_rear();
			return;
		}
		else if (pos > 0 && pos < howMany() - 1)
		{
			List* temp = Front;
			int i = 0;
			while (i <= (pos - 1))
			{
				temp = temp->next;
				i++;
			}
			temp->previous->next = temp->next;
			temp->next->previous = temp->previous;
			temp = NULL;
			delete temp;
			return;
		}
		else
		{
			cout << "\n error: wrong position!";
		}

	}
	size_t sizeOf()
	{
		return howMany() * sizeof(s);
	}
	T& operator[](int n)
	{
		if (howMany() - 1 < n || n < 0)
		{
			cout << "\n error: index out of bounds";

		}
		else
		{
			List* temp = Front;
			int i = 0;
			while (i <= (n - 1))
			{
				temp = temp->next;
				i++;
			}
			return temp->s;
		}
	}
	void operator = (List<T> ob)//making a copy
	{
		for (int i = 0; i < ob.howMany(); i++)
		{
			this->add_rear(ob[i]);
		}


	}
	List(const List& ob)
	{
		Front = NULL;
		rear = NULL;
		List<T>* temp = ob.Front;
		while (temp != NULL)
		{
			add_rear(temp->s);
			temp = temp->next;
		}
	}
	~List()
	{
		int temp = howMany();
		for (int i = 0; i < temp; i++)
		{
			del_front();
		}
		delete Front;
		delete rear;
	}
	void erase()
	{
		while (howMany() != 0)
		{
			this->del_rear();
		}
	}


};
////////////////////
class user_credentials
{
public:
	int type;// type 0 for login, and type 1 for signup //2 for checking the if the username is taken or not. based on this type we will write different code that will hangle it
	char username[20];
	char password[20];
	char email[30];
	char role[30];
	char country[30];
	char match_type_id[30];//this is the id of the tournament if there, if empty it is a normal ranked match.
	user_credentials() { }
	user_credentials(int type, string user, string pass, string mid)
	{
		this->type = type;
		strcpy(username, user.c_str());
		strcpy(password, pass.c_str());
		strcpy(match_type_id, mid.c_str());
	}
	user_credentials(int type, string user, string pass)
	{
		this->type = type;
		strcpy(username, user.c_str());
		strcpy(password, pass.c_str());

	}

	user_credentials(string user, string pass, string mid)
	{
		strcpy(username, user.c_str());
		strcpy(password, pass.c_str());
		strcpy(match_type_id, mid.c_str());
	}
	user_credentials(string user, string pass)
	{
		strcpy(username, user.c_str());
		strcpy(password, pass.c_str());
	}
	user_credentials(string user, string pass, string email, string role, string country)
	{
		strcpy(username, user.c_str());
		strcpy(password, pass.c_str());
		strcpy(this->email, email.c_str());
		strcpy(this->role, role.c_str());
		strcpy(this->country, country.c_str());
	}
	user_credentials(int type, string user, string pass, string email, string role, string country)
	{
		this->type = type;
		strcpy(username, user.c_str());
		strcpy(password, pass.c_str());
		strcpy(this->email, email.c_str());
		strcpy(this->role, role.c_str());
		strcpy(this->country, country.c_str());
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
	char token[20];
	int isBot;//true if the this is a bot
};
int cx(int x);
int cy(int y);
int rx(int x);
int ry(int y);

//universal function for finding stuff from the list, pass the list and the element to be found and get the index
template<typename Y>
int getIndex(List<Y>& l, Y val);

class ship;
class graphics;
class Control;
/*some networking libs*/
void startup(int n, unordered_map<int, sockaddr_storage>& socket_id, int port);
class control1;
void chaseShip1(int s_id, ship& ob);
enum class map
{
	globalMap, localMap
};

enum class Direction
{
	NORTH, SOUTH, EAST, WEST, NORTH_WEST, NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NA = -1
};
enum class cannon
{
	FRONT, REAR, NA = -1
};
enum class ShipSide
{
	FRONT, REAR, MIDDLE, NA = -1
};
enum  class Entity
{
	WATER = 0, STORM = 1, LAND = 2, CANNON = 3, BONUS = 4, SHIP = 5, NA = -1
};
enum  class Bonus
{
	//define appropriate bonus here
	RESOURCE, HEALTH, BULLETS, GOLD, NA = -1
};
class attribute//attribute of the tile i.e what does the tile have: storm, bonus,land,cannons,ship ///  a list of the objects of this class will be returned.
{
protected:
	Entity entity;//type
	double cost;//in case of a water or storm only
	int ship_id;// if its a ship
	int cannon_id;//if its a cannon

	void update(double c, int sid, int cid, int key)
	{
		if (key == 2369)
		{

			cost = c;
			ship_id = sid;
			cannon_id = cid;

		}
	}
	void updateShipStatus(int sid, int key)
	{
		if (key == 2369)
		{
			ship_id = sid;
		}
	}
public:

	attribute()
	{

		cost = -1;
		ship_id = -1;
		cannon_id = -1;

	}
	attribute(Entity ent, double c, int sid, int cid)// ent=NA,c=-1,id=-1,b=NA if any of these doesnt exists
	{

		cost = c;
		ship_id = sid;
		cannon_id = cid;

	}


	double getCost()
	{
		return cost;
	}
	int getShipId()
	{
		return ship_id;
	}
	int getCannonId()
	{
		return cannon_id;
	}
	Entity getEntity()
	{
		return entity;
	}
	friend ship;

};
class Greed //main class for the game
{

public:

	class abs_pos//holding the exact coordinates of the ship in the global world
	{
	public:
		double x;
		double y;
		abs_pos()
		{

		}
		abs_pos(double xx, double yy)
		{
			x = xx;
			y = yy;
		}
	};
	class uCoords//a class for the coordinates of the user
	{
	public:
		int r;
		int c;
		uCoords(int rr, int cc)
		{
			r = rr;
			c = cc;
		}
	};
	class coords//a container class for the  node // this class is wrapper for all the data to be used for a individual node
	{
	public:
		int r;
		int c;
		int cost;//cost of a tile
		double heuristic;
		Entity tile_id;//tells if it is a water , land and cannon only
		int cannon_id;//gives the cannon id if exists else -1
		Bonus bonus;//in case its a bonus tile
		///here are some of the new parameters.

		coords()
		{
			cost = 1;//default cost is 1
			cannon_id = -1;


		}
		coords(int rr, int cc, int co = 1)
		{
			r = rr;
			c = cc;
			cost = co;
		}
		coords(int rr, int cc, int co, Entity tile, int c_id, Bonus b)
		{
			r = rr;
			c = cc;
			cost = co;
			tile_id = tile;
			cannon_id = c_id;
			bonus = b;
		}
		bool operator==(coords two)
		{
			if (r == two.r && c == two.c)
				return true;
			else
				return false;
		}
		bool operator!=(coords two)
		{
			if (r != two.r || c != two.c)
			{
				return true;
			}
			else
				return false;
		}
	};
	//a fire class for armory
protected:

	class point// class for holding pointer to coords
	{
	public:
		coords* ob;
		point()
		{

		}
		point(coords* obb)
		{
			ob = obb;
		}
	};

public:
	void deleteDuplicate(List<abs_pos>& ob);
	List<abs_pos> makeList(List<coords> ob);
	List<abs_pos> makeTrajectory(double x1, double y1, double x2, double y2);

	class vertex//object for the map //or known  as the node in a graph
	{
	public:
		coords b;
		List<point> vert;//a list of pointers to the vertex of the map
		vertex()
		{


		}
		vertex(coords x)
		{
			b = x;
		}
	};
	class A
	{
	public:
		coords v;
		double cost;
		double heuristic;
		double sum;//sum=cost+heuristic
		coords path;
		A()
		{


		}
		A(coords v1, double c, double h, coords p)
		{
			v = v1;
			cost = c;
			heuristic = h;
			sum = cost + heuristic;
			path = p;

		}

	};
	double calc_heuristic(coords origin, coords dest)//using another technique to find the heuristic
	{
		//return abs(dest.r-origin.r)+abs(dest.c-origin.c);
		return abs(dest.r - origin.r) + abs(dest.c - origin.c);
	}
	int smallest_entry(List<A>& open);
	int isThere(coords b, List<A>& closed);//returns the index //for class A
public:
	class path_attribute
	{
		List<Greed::abs_pos> way;
	public:
		double resources;
		List<Greed::coords> target;

		path_attribute()//default ctor for initial NULL values
		{
			resources = 0;
		}

		double getPathCost()//user function to get the total cost of the path returned
		{
			return resources;
		}
		List<Greed::coords> getPath()//errorsome stuff
		{
			List<Greed::coords> cop;
			for (int i = 0; i < target.howMany(); i++)
			{
				cop.add_rear(target[i]);
			}
			return target;
		}
		void operator=(path_attribute ob)
		{
			resources = ob.resources;
			int temp = target.howMany();
			for (int i = 0; i < temp; i++)
			{
				target.del_front();
			}
			for (int i = 0; i < ob.target.howMany(); i++)
			{
				target.add_rear(ob.target[i]);
			}
		}



	};
protected:
	void find_path(coords origin, coords goal, List<A>& closed, path_attribute& ob, List<vertex>& rmap);
public:
	int find_index(coords b, List<Greed::vertex> graph);//for vertex class only
	void path(coords origin, coords goal, List<vertex>& graph, path_attribute& ob, List<vertex>& rgraph);//the famous A* algorithm resides here

	bool isValidCoords(coords ob)
	{
		if (ob.r >= 0 && ob.r <= rows - 1 && ob.c >= 0 && ob.c <= columns - 1)
		{
			return true;
		}
		return false;
	}
	void getMap(int code[rows][columns], int r, int c, List<vertex>& graph);//takes in pointer to the code of the maze // here its 3*3 but has to be changed
	class bullet;
	class shipCannon;
	class cannon;

};
class Mutex//class protecting all the mutexes
{
public:
	mutex bullet_mutex[20];//for the complete list of bullets of the player
	mutex m_global_map;//mutex for syncrhonization of globalMap variable

	mutex m[20];//mutex for the ship
	mutex timeMutex[20];//mutex for protecting the data structure of timeline
	mutex updateTime;//mutex for updating and reading total time of the game
	///mutex m is for protecting the path data structure
	mutex event_mutex[20];
	mutex mchase[20];//mutex for follow up function of ship class
	mutex updating_data;//for server only
	mutex gameOver_check;

	mutex send_terminal;//for sending the data to the client terminal
	mutex send_display;//for sending the data to the client display unit
	condition_variable cond_terminal;//to check if terminal is ready to recv the data
	condition_variable cond_display;//to check if the display is ready to recv the data
	condition_variable cond_input_terminal;//to check if the input queue has the correct data to process
	condition_variable cond_updating_data;//updating data for

	mutex recv_terminal;//to recv the data from the client terminal side
	mutex recv_display;//to recv the data from the display client side

	mutex game_tick_mutex;//mutex used to protect the game tick of the server
	mutex game_tick_mutex_client;



	Mutex()
	{

	}
	friend class ship;
	friend class graphics;
	friend class Control;
	friend class Greed::bullet;
	friend class Greed::shipCannon;
	friend void chaseShip1(int, ship&);
	friend void send_data_terminal(unordered_map<int, sockaddr_storage>, Mutex*);
	friend void send_data_display(unordered_map<int, sockaddr_storage>, Mutex*);
	friend class Event;
	friend class control1;

};
class Greed::bullet
{
public:
	int id;
	double power;//power of the bullet
	double damage;
	int launch_ship;//if bullet is fired by ship then id of ship is stored else -1
	int launch_cannon;//if bullet is fired by cannon then id of the cannon is stored else -1
	int target_ship;//id of the ship that launched the bullet this is the id of the ship provided by the launch ship but the bullet might not strike at that ship since everything is in motion
	int hit_ship;//id of the ship that really hit the bullet
	int target_cannon;//if the target is cannon then id of cannon is stored else -1
	int hit_cannon;
	bool isActive;//is the bullet active
	bool isSuccess;//if the bullet hit the target
	::cannon can;//side of the firing ship
	ShipSide s;//side of the enemy ship
private:
	Greed::abs_pos absolute_position;//position of the bullet according to the game world
	int ttl;
	bool hit_or_not;

	Mutex* mutx;//mutex m will be locked
	int mid;//mutex id which is same as ship_id

	double slope;

	Greed::coords starting_tile;//tile from where the bullet started
	Greed::coords ending_tile;//tile where the bullet will end
	Greed::coords some1;
	int some2;

	vector<Greed::abs_pos> bullet_trajectory;// this is for setting the bullet position in the world
	// List<Greed::abs_pos> bullet_trajectory_cont;//traces the trajectory of the bullet

	void set_after_data(double dam, int hit_ship, bool isA)//set_after_data for ship to ship fire
	{
		damage = dam;
		this->hit_ship = hit_ship;
		isActive = isA;
		if (target_ship == hit_ship)
		{
			isSuccess = true;
		}
		else
		{
			isSuccess = false;
		}
	}
	void set_after_data_for_cannon(double dam, int hit_cannon, bool isA)//to be called when a ship bullet hits a cannon
	{
		damage = dam;
		this->hit_cannon = hit_cannon;
		isActive = isA;
		hit_ship = -1;
		if (target_cannon == hit_cannon)
		{
			isSuccess = true;
		}
		else
		{
			isSuccess = false;
		}
	}
	void initialize(bool isA, Mutex* m, int id, int mid, double power, int launch_cannon, int launc_ship, int target_cannon, int target_ship, bool isActive, ::cannon can, ShipSide s)

	{
		this->can = can;
		this->s = s;
		isActive = isA;
		this->mid = mid;
		mutx = m;
		this->id = id;
		this->power = power;
		this->launch_ship = launc_ship;
		this->target_ship = target_ship;
		hit_or_not = false;
		this->isActive = isActive;
		this->launch_cannon = launch_cannon;
		this->target_cannon = target_cannon;
	}
public:
	
	
	bullet()
	{
		ttl = 0;
		power = 0;
		damage = 0;
		launch_ship = -1;
		target_ship = -1;
		isActive = false;//a bullet will become active soon after its initialized
		slope = -1;
		isSuccess = false;
		
		hit_ship = -1;
		target_cannon = -1;
		hit_cannon = -1;
	}

	bool isBulletActive()
	{

		return isActive;
	}
	bool didBulletHit()
	{

		return isSuccess;
	}


	int getBulletId()
	{
		return id;
	}
	double getBulletPower()
	{
		return power;
	}
	double getBulletDamage()
	{

		return damage;
	}
	int getLaunchShipId()
	{
		return launch_ship;
	}
	int getTargetShipId()
	{
		return target_ship;
	}
	friend class ship;
	friend class graphics;
	friend class Greed::cannon;
	friend int main(int argc, char* argv[]);
	friend class control1;

};
class control1;
class Greed::cannon//no need to lock this class with any mutex, no concurrency is performed here
{
public:
	int getCannonId()
	{
		return cannon_id;
	}
	double getCannonHealth()
	{
		return health;
	}
	Greed::coords getCannonPosition()
	{
		return tile;
	}
	double getCannonMoney()
	{
		return money;
	}
	bool isCannonDead()
	{
		return isDead;
	}

private:
	
	int cannon_id;
	double health;
	Greed::coords tile;
	Greed::abs_pos absolute_position;//abs pos of the middle of the cannon tile for the purpose of finding the slope.
	double money;
	bool isDead;//0 for dead 1 for alive
	int radius;
	bool busy;//to check if the cannon is currently busy
	double current_angle;//current angle of the cannon
	double req_angle;//angle required for the cannon 
	List<int> target_ships;//id of the ship that were targeted by the cannon, the last ship being the ship under attack by the cannon
	int current_ship;//id of the current attacking ship
	ShipSide ss;
	vector<Greed::bullet> bullet_list;
	List<Greed::bullet> allBullets;

	void filter(List<Greed::abs_pos>& ob);
	
	void initializer(int cid, Greed::coords tile)
	{
		cannon_id = cid;
		this->tile = tile;
		absolute_position = Greed::abs_pos((int)tile.c * len / 2, (int)tile.r * len / 2);
	}
	bool isShipInMyRadius(int s_id, ShipSide myside);
	int getVictimShip();
	bool fireCannon(int s_id);

	double get_required_angle();
public:

	Greed::coords getCannonTile()
	{
		return tile;
	}
	cannon()
	{
		cannon_id = -1;
		health = 500;
		money = 1000;
		isDead = false;
		radius = 2;
		busy = false;
		current_angle = 0;
		current_ship = -1;
	}
	friend control1;
	friend graphics;
	friend int main(int argc, char* argv[]);
	friend class ship;
	friend void startup(int n, unordered_map<int, sockaddr_storage>& socket_id, int port);

};
class Greed::shipCannon
{
	Mutex* mutx;
	static long double current_bullet;
	int mid;//here it will be equal to the ship id of the ship owning the cannon
	//here id of the cannon will be same as the id of ship 
	double power;
	vector<Greed::bullet> activeBullets;//(this is for online version of the game) the bullets that are not yet legalized by graphics::callable and just called by the player yet
	vector<Greed::bullet> allBullets;//this shit is useless for now. it has nothing in it.
	unordered_map<int, Greed::bullet> legal_bullets;


	/*
	List<Greed::bullet> getAllBullets()
	{
		unique_lock<mutex> lk(mutx->m[mid]);
		return allBullets;
	}
	*/

public:

	shipCannon()
	{
		power = 5;

	}
	shipCannon(int s_id, int pow)
	{
		mid = s_id;
		power = pow;
	}
	void initialize(Mutex* m, int pow)
	{
		power = pow;
		mutx = m;
	}
	bool setCannonPower(double pow)
	{
		unique_lock<mutex> lk(mutx->m[mid]);
		power = pow;
	}

	double getCannonPower()
	{
		unique_lock<mutex> lk(mutx->m[mid]);
		return power;
	}

	friend ship;
	friend graphics;
	friend control1;

	friend int main(int argc, char* argv[]);

};


class shipInfo
{

	ship* ob;
	Greed::coords getCurrentTile_withoutMutex();

	Greed::coords getCurrentRearTile_withoutMutex();

public:

	shipInfo()
	{

	}
	shipInfo(ship* ob)
	{
		this->ob = ob;
	}
	int getTotalShips();
	int getShipId();
	int getShipRadius();
	double getCurrentHealth();
	double getCurrentGold();
	double getCurrentAmmo();
	double getCurrentResources();
	double getCurrentFuel();
	int  getDiedStatus();
	Direction getShipDirection();
	Greed::coords getCurrentTile();
	Greed::coords getCurrentRearTile();
	Greed::abs_pos getRealAbsolutePosition();

	friend class ship;


};
class radar_output
{
	/*documentation
	*its work is to tell with which part of my ship i can fire another ship
	*
	*/
public:
	shipInfo ob;
	bool front;//my side
	bool rear;//my side
	radar_output()
	{
		front = false;
		rear = false;
	}
	radar_output(shipInfo ob, bool f, bool r)
	{
		this->ob = ob;
		front = f;
		rear = r;
	}
	bool operator==(radar_output& s)
	{
		if (this->ob.getShipId() == s.ob.getShipId() && (this->front == s.front && this->rear == s.rear))
		{
			return true;
		}
		else
			return false;
	}


};
class Map//class for every map
{
	/*documentation
	* this class will work as a template for map
	* characteristics of map:
	* 1. bitmap of the code
	* 2.textures have to be loaded
	*/
	int map_id;//id number of the map or which map has to be called
	class file
	{
	public:
		int id;
		float x;
		float y;

	};
public:
	Map()
	{

	}
	Map(int mid)
	{
		map_id = mid;
	}
	
	void initialize_map(int code[12][24], List<Greed::coords>& opaque);//list of opaque tiles
};
///////////////class for timeline/////////////////////////////////
class timeline
{
public:
	enum class EventType
	{
		SHIP_DIED,
		GOLD_TO_HEALTH,
		GOLD_TO_FUEL,
		GOLD_TO_AMMO,
		DEFEATED_SHIP,
		ANCHOR_SHIP,
		NA = -1

	};
	class health
	{
	public:
		double old;//value of old health
		double n;//value of new health
	};
	class fuel
	{
	public:
		double old;
		double n;
	};
	class ammo
	{
	public:
		double old;
		double n;
	};
	class defeated
	{
	public:
		int id;
	};
private:
	long double timestamp;
	EventType eventype;
	health h;
	fuel f;
	ammo a;
	defeated d;
	friend class ship;
	friend class graphics;
	friend class control1;
};
class Event
{
	static long  count;

	long event_id;
	int sid;//sid represents the id of the ship for which an event is created
	long double ttl;
public:
	long double timestamp;//time stamp of every event

	Event()
	{
		eventType = EventType::NA;
	}


	void setEventId()
	{
		event_id = count;
		count++;
	}

	Event(double timestamp, int event_id, int sid, double ttl)
	{
		this->timestamp = timestamp;
		this->event_id = event_id;
		this->sid = sid;
		this->ttl = ttl;
		eventType = EventType::NA;
		count++;
	}
	enum class EventType
	{
		ShipsInMyRadius,//0
		IamInShipRadius,
		IamInCannonRadius,
		CannonsInMyRadius,
		ShipFire,//when a ship fires
		CannonFire,//when a cannon fires 
		LowHealth,//when health is really low
		NoAmmo,
		LowAmmo,
		NoFuel,
		LowFuel,
		ShipCollision,//if a ship collides with another
		NA = -1
	}eventType;//object for the event class


	void initialize(double timestamp, EventType et, int sid)
	{
		this->timestamp = timestamp;
		this->sid = sid;

		eventType = et;
		ttl = timestamp;

	}
	class ShipCollision
	{
		vector<int> sid;
	public:
		ShipCollision() {}
		ShipCollision(vector<int>& s)
		{
			sid = s;
		}
		vector<int> getShipId()
		{
			return sid;
		}

		friend class ship;
		friend class graphics;
	};
	class LowFuel
	{
		double fuel;
	public:
		LowFuel() {}
		LowFuel(double f)
		{
			fuel = f;
		}
		double getFuel()
		{
			return fuel;
		}

		friend class ship;
		friend class graphics;
	};
	class LowAmmo
	{
		double ammo;
	public:
		LowAmmo() {}
		LowAmmo(double a)
		{
			ammo = a;
		}
		double getAmmo()
		{
			return ammo;
		}

		friend class ship;
		friend class graphics;
	};
	class LowHealth
	{

		double health;
	public:
		LowHealth() {}
		LowHealth(double h)
		{
			health = h;
		}
		double getHealth()
		{
			return health;
		}

		friend class ship;
		friend class graphics;
	};
	class CannonFire
	{
		unordered_map<int, Greed::bullet> cid;//vector of the cannnon ids
	public:
		CannonFire() {}
		CannonFire(unordered_map<int, Greed::bullet>& c)
		{
			cid = c;
		}
		unordered_map<int, Greed::bullet> getInfo()
		{
			return cid;
		}

		friend class ship;
		friend class graphics;
	};
	class ShipFire//SOME other ships fire at you
	{
		unordered_map<int, Greed::bullet> ob;//int is the id of the ship that fired the bullet
	public:
		ShipFire()
		{

		}
		ShipFire(unordered_map<int, Greed::bullet>& s)
		{
			ob = s;
		}
		unordered_map<int, Greed::bullet> getShipId()
		{
			return ob;
		}

		friend class ship;
		friend class graphics;
	};
	class Radius_cannon
	{
		vector<int> cid;
	public:
		Radius_cannon() {}
		Radius_cannon(vector<int>& c)
		{
			cid = c;
		}
		vector<int> getCannonId()
		{
			return cid;
		}

		friend class ship;
		friend class graphics;

	};
	class Radius_ship//this class contains data about the first two events in the list
	{

		vector<int> sid;//id of the ship in question
	public:

		Radius_ship()
		{

		}

		Radius_ship(vector<int>& s)
		{
			sid = s;
		}
		vector<int> getShipId()
		{
			return sid;
		}
		friend class ship;
		friend class graphics;

	};

	//declaring the objects 

	Radius_ship radiusShip;
	Radius_cannon radiusCannon;
	ShipFire shipFire;
	CannonFire cannonFire;
	LowHealth lowHealth;
	LowAmmo lowAmmo;
	LowFuel lowFuel;
	ShipCollision shipCollision;
	friend class ship;
	friend class graphics;


};
class upgrade_data
{
public:
	int id;
	int type;
	/*
	* 0 for ammo
	* 1 for health
	* 2 for fuel
	*/
	int n;//upgrade by number

	upgrade_data() {
		id = -1;
	}
	upgrade_data(int t, int n)
	{
		type = t;
		this->n = n;
		id = -1;
	}
	friend class control1;
	friend class graphics;
};
class navigation
{
	int id;
	int type;
	/*
	* 0 for target type
	* 1 for sailing
	* 2 for chasing
	* 3 for aborting
	*/
	Greed::coords target;
	int s_id;//for target type and chasing
	int n;//number of tiles
	Direction dir;
public:
	navigation() {
		id = -1;
	}
	navigation(int t, Greed::coords tar, int sid, int nn, Direction d)
	{
		id = -1;
		type = t;
		target = tar;
		s_id = sid;
		n = nn;
		dir = d;
	}
	friend class graphics;
	friend class control1;

};

class Startup_info_client
{
	/*
	* configurations server will send to the client for the initialization of the game
	*/
	int framerateLimit;
	int no_of_players;
	int ship_id;
	Greed::coords starting_pos;
public:
	Startup_info_client()
	{

	}
	Startup_info_client(int f, int n, int s, Greed::coords& pos)
	{
		framerateLimit = f;
		no_of_players = n;
		ship_id = s;
		starting_pos = pos;
	}
	friend int main(int argc, char* argv[]);
	friend class control1;
	friend class graphcis;
};
class GameState
{
	/*this class is now useless the work of this class will be done by shipData_forMe*/
	/* this class has the format of data that the server will send to the client*/
	bool collision;
	int size_collide_ship;//size of the array below
	int collide_ship[20];//id's of the ships that collided
	int size_hit;//size of the array below
	int hit[100];//the array of id's of the bullets that has hit any opaque object, cannon or a ship
	friend class graphics;

};
class bullet_data_client
{
	/*bullet data for client by the server*/
	int id;
	double power;
	double damage;
	int launch_ship;
	int launch_cannon;//if bullet is fired by cannon then id of the cannon is stored else -1
	int target_ship;//id of the ship that launched the bullet this is the id of the ship provided by the launch ship but the bullet might not strike at that ship since everything is in motion
	int hit_ship;//id of the ship that really hit the bullet
	int target_cannon;//if the target is cannon then id of cannon is stored else -1
	int hit_cannon;
	bool isActive;//is the bullet active
	bool isSuccess;//if the bullet hit the target
	::cannon can;//side of the firing ship
	ShipSide s;//side of the enemy ship
	friend class control1;
	friend class graphics;
};
class bullet_data//new bullet data
{


public:
	int bullet_id;//unique id given to every bullet
	int type;
	/*
	* 0 for fireCannon or fire at ship
	* 1 for fireAtCannon or fire at cannon
	*/
	cannon can;
	int s_id;
	ShipSide s;
	int c_id;

	bullet_data()
	{
		bullet_id = -1;
	}
	bullet_data(int type, cannon can, int s_id, ShipSide s, int c_id)
	{
		bullet_id = -1;
		this->type = type;
		this->can = can;
		this->s_id = s_id;
		this->s = s;
		this->c_id = c_id;
	}
	friend class graphics;
	friend class control1;
};

extern deque<navigation> resend_navigation;//to store the navigation requests that has be resent
extern deque<pair<bullet_data, int>> resend_bullet;//firing data that has to be resent
class map_cost//class to send updateCost of the map tile data to the server over the network
{
public:

	double new_cost;
	Greed::coords ob;
	map_cost()
	{

	}
	map_cost(double n, Greed::coords o)
	{

		new_cost = n;
		ob = o;
	}
};
class old_bullet_data//bullet data for old bullets that are clinging in the field
{
	//for such bullets we only want to send only the position of the bullets
	int id;
	Greed::abs_pos absolute_position;
public:
	old_bullet_data()
	{

	}
	old_bullet_data(int id, Greed::abs_pos abs)
	{
		this->id = id;
		this->absolute_position = abs;
	}
	friend class control1;

};
class shipData_forServer
{
	/* this class has the format of data that the client will send to the server
	*/
public:
	int ship_id;//id of the ship
	bool navigation_promise;
	double threshold_health;
	double threshold_ammo;
	double threshold_fuel;

	int client_fire;

	int radius;//square radius

	int size_navigation;
	navigation nav_data[4];//to be merged

	int size_bulletData;
	bullet_data b_data[15];//to be merged

	int size_upgrade_data;
	upgrade_data udata[10];//to be merged

	int cost_id;//this is the id of the map cost data
	int size_update_cost;//to update the cost of the local map of the user
	map_cost cdata[80];//to update the data
	//to be merged

	shipData_forServer()
	{

	}
	friend class graphics;
	friend class control1;
};
class cannon_info//this data is used by the ships and this data has to be sent to the clients over the wire
{
	int cannon_id;//cannon id
	double health;//health of the cannon
	Greed::coords location;//current location of the cannon
	bool is_cannon_dead;
public:
	cannon_info()
	{

	}
	cannon_info(int cid, double health, Greed::coords loc, int is_cannon_dead)
	{
		this->cannon_id = cid;
		this->health = health;
		this->location = loc;
		this->is_cannon_dead = is_cannon_dead;
	}
	int getCannonHealth()
	{
		return health;

	}
	int getCannonId()
	{
		return cannon_id;

	}
	Greed::coords getCannonLocation()
	{
		return location;
	}
	bool isCannonDead()
	{
		return is_cannon_dead;

	}
};
class shipData_forMe
{
	/*data that the server will send to the client after computing the state for that frame*/
	/*
	* it has data like after getting hit by a bullet
	*/
public:
	bool navigation_promise;
	int autopilot;
	int ship_id;
	int seconds;
	int minutes;

	int killer_ship_id;
	int killed_ships_size;
	int killed_ships[10];//to be merged.
	int score;
	int radius;//square radius

	double health; //to be changed
	double gold;// to be changed
	int died;//1 for dead, 0 for alive //to be changed
	int ammo;
	int fuel;//fuel is used for moving the ship around, once its over the ship cant move
	int invisible;//to be changed
	int server_fire;
	int collided_size;
	int bullet_hit_size;

	Greed::coords front_tile;
	Greed::coords rear_tile;
	Greed::abs_pos front_abs_pos;
	Greed::abs_pos rear_abs_pos;
	Greed::abs_pos absolute_position;
	Direction dir;
	int motion;

	int size_cannon_data;
	cannon_info cannon_data[5];//data of the cannon(health etc.)

	int size_collided_ships;
	int collided_ships[10];

	int size_hit_bullet;
	bullet_data_client hit_bullet[10];

	int size_unlock;
	int unlock[5];

	int size_bullet_recved;
	int bullet_recved[50];//id's of the last 50 bullets that the server has recved 

	int size_upgrade_recved;
	int upgrade_recved[10];

	int size_cost_id;
	int cost_recved[3];

	int size_nav_id;
	int nav_recved[6];

	friend class graphics;
	friend class control1;
};
class shipData_exceptMe //updated data that the server will send for the client
{
	/*
	* This data will be sent by the server to every client. this contains data about ship except themselves
	* I will use other ship's data in following ways(I is the current ship)
	* 1. algorithm may use information about other ships
	* 2. to find the shipInRadius etc. events where we need some information about the position of the enemy ships
	*/

	int ammo;
	int died;
	Direction dir;
	int ship_id;
	bool isFiring;
	int score;

	double health;
	double gold;
	int motion;
	int fuel;
	int invisible;
	Greed::coords tile_pos_front;
	Greed::coords tile_pos_rear;
	Greed::abs_pos front_abs_pos;
	Greed::abs_pos rear_abs_pos;
	Greed::abs_pos absolutePosition;
	friend class control1;

};
void send_data_terminal(unordered_map<int, sockaddr_storage> addr_info, Mutex* m);
class recv_data//to be recv by the client and to be  sent by the server
{
public:
	int st;//to check the authenticity of the incoming packet
	int packet_id; //for debugging purpose only
	char token[20];//current token for the game
	int s1;
	shipData_exceptMe shipdata_exceptMe[10];

	shipData_forMe shipdata_forMe;
	int gameOver;//0 for game not over, 1 for over;
	int end;//to check the authenticity of the incoming packet
	friend class graphics;
	friend void send_data_terminal(unordered_map<int, sockaddr_storage> addr_info, Mutex* m);
};
class send_data// to be sent by the client and to be received by the server
{
public:
	int st;
	int packet_id; //for debugging purpose only
	user_credentials user_cred;

	shipData_forServer shipdata_forServer;//my data to be sent to the server

	int end;
	send_data()
	{

	}
	friend class graphics;
};


bool get(ship* a, ship* b);
class pack_ship;
void update_frame(deque<ship*>& pl1, pack_ship& ob, int i);
//some networking classes*/

class upgrade_keeping//class for bookeeping the upgrade data
{
public:
	int active;//if it is active or not
	int id;
	int type;//type of the upgrade 0 for ammo, 1 for health and 2 for fuel
	int current_value;//current value of the object at the time of creation
	int new_value;//should be value of the object
	int timer;//when the timer is over it will be resent
	upgrade_data udata;
	upgrade_keeping()
	{
		timer = 0;
	}
	upgrade_keeping(int active, int type, int current_value, int new_val, upgrade_data udata)
	{
		timer = 0;
		this->active = active;
		this->type = type;
		this->current_value = current_value;
		this->new_value = new_val;
		this->udata = udata;
	}

};

class ship//this class will be used to initialize the incoming player and give it a ship. and then keep tracking of that ship
{

protected:
	bool isAuthenticPath(List<Greed::abs_pos> ob);
private:
	bool navigation_promise;//a variable that tells upon checking all the conditions if the ship can move or not, if yes, then we send a premature promise to
	//the client that your ship is now in motion, but it will be upon 2-3 cycles,this way client can stay live with the server and abstain from making false decisions.
	bool anchor_promise;//promise to get anchored..
	bool solid_motion;//a variable which is used because the ship moves to complete the tile whenver halt is called in that time motion is still 1 so 
	//we need to keep a proxy a fucking proxy....
	Mutex* mutx;
	Event events;// a double sided queue of the events;
	
	static int total;//total ships
	static List<Greed::vertex> globalMap;//global map variable
	int mutex_id;//id of the mutex to be used
	Greed map_ob;//to use the facilities of the class Greed
	int ship_id;//id of the ship
	deque<timeline> time_line;

	//these variables are used to track the frame count of the algorithm of the user, the algorithm will only run if these variables mismatch
	//initially these values will be 0, the client game loop will update the value of current_count, and prev_count will be increased by algorithm game loop
	//these variables will be protected by a mutex.
	int prev_count;
	int current_count;

	vector<int> collided_ships;
	vector<::cannon_info> cannon_info;//data of the cannons present in the environment
	deque<upgrade_data> udata;
	deque<map_cost> map_cost_data;//only to be used at the client side;

	vector<int> unlock;
	void update_pos_collision();//function to update tile_pos and abs_pos of the ship after the collision occured
	//for maintaining the frame rate of the user function
	std::chrono::high_resolution_clock::time_point current_time;
	chrono::steady_clock::time_point starting_time_limiter;//starting time when frame rate limiter started
	bool hector;//to check if frame_rate_limiter is running for the first time or not
	
	int current_frame_no;
	double elapsed_time;
	int frame_rate_limit;//value of the frame rate to be kept for the user function
	//ends
	//armory of the ship

	int lock_ammo;
	int lock_fuel;
	int lock_health;
	int lock_chase_ship;

public: //this will be public the user will be able to access this object freely
	//object 
	std::map<int, int> bullet_id_count;//used by the server
	std::map<int, int> upgrade_id_count;//to count the id's that have arrived to upgrade the data used by the server
	std::map<int, deque<map_cost>> upgrade_cost;//used by the client
	std::map<int, int> cost_id_count;//used by the server
	std::map<int, int> nav_id_count;//to be used by the server
	int navigation_count;//to give the id to the new navigation command

	int collide_count;
	int hit_bullet_count;
	int cost_upgrade_count;
	int bullet_count;
	int client_fire;
	int server_fire;
	bool gameOver;
	bool isFiring;
	bool isShipInMotion()//this function has to be used
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);

		return solid_motion;
	}
	List<Greed::abs_pos> path;
	bool frame_rate_limiter();//function to maintain the frame rate of the user function
	double threshold_health;
	double threshold_ammo;
	double threshold_fuel;
	Greed::shipCannon cannon_ob;
	deque<Event> current_event;//current events
	deque<Event> passive_event;//lingering events
	bool getNextCurrentEvent(Event& ob);
	deque<Event> getPassiveEvent()
	{
		unique_lock<mutex> lk(mutx->event_mutex[ship_id]);
		return passive_event;
	}//returns the passive event queue 
	int passive_event_point;//pointer to getNextPassiveEvent..to tell which event to send
private:
	int seconds;//seconds lived

	deque<Greed::bullet> hit_bullet;//stores the hit bullets on the ship for that frame
	int minutes;//minutes lived
	int killer_ship_id;//the ship that killed you if that is the case
	int killer_cannon_id;//the cannon that killed you if that is the case
	deque<int> killed_ships;//the list of all the ships killed by you..
	int score;
	string name;
	int current_event_point;//pointer to current_event
	deque<navigation> nav_data;
	deque<navigation> nav_data_temp;//to store the nav_data temorarily
	deque<navigation> nav_data_final;//data that will be sent
	List<Greed::bullet> bullet_hit;//the list of the bullets that had ever hit the ship
	deque<Greed::bullet> bullet_hit_tempo;//made solely for the purpose of events, here the objects will be deleted as they are inserted into the events
	deque<bullet_data> bullet_info;//
	static List<Greed::cannon> cannon_list;
	Greed::abs_pos front_abs_pos;//topmost coordinates of the tip of the ship:: will be updated in update_tile_pos
	Greed::abs_pos rear_abs_pos;//endmost coordinates of the ship ==> will be updated in update_tile_pos
	//Map::abs_pos absolutePosition;//always stores the top left coordinate of the ship tile
	Direction dir;
	//position is defined by the pointing part of the ship..look for it


	int radius;//square radius
	int bullet_radius;//radius of the bullet...initially this value is equal to the radius of the ship
	double health; //to be changed
	double gold;// to be changed
	double resources;// to be changed
	int died;//1 for dead, 0 for alive //to be changed
	int motion; //1 for moving, 0 for stop
	int fuel;//fuel is used for moving the ship around, once its over the ship cant move
	int invisible;//to be changed
	int ammo; // to be updated
	int bullet_pointer;
	int xbow; // to be updated
	int shield;//if this is on, then no bullet can damage the ship...this is given for a limited time
	//data structure responsible for movement

	List<Greed::coords> tile_path;// path of the user tile by tile //path returned by A*

	Greed::coords tile_pos_front;
	Greed::coords tile_pos_rear;

	// List<Map::abs_pos> path;//abs_pos path list
	int pointPath;
	int pointTilePath;
	Greed::abs_pos absolutePosition;
	int autopilot = 0;//bit to check if the ship is moving in autopilot or not



	List<Greed::vertex> localMap;
	deque<shipInfo> shipInfoList;

	//for armory
	//now some functions
private:
	void updateFuel(int n)
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		fuel = n;
	}
	Greed::abs_pos getAbsolutePosition1(ShipSide s = ShipSide::FRONT)//this function is without the mutex
	{

		if (s == ShipSide::FRONT)
			return front_abs_pos;
		else if (s == ShipSide::REAR)
		{
			return rear_abs_pos;
		}


	}
	bool anchorShip_collision();//special function to stop and reset the ship in case of a collision
	void setPath_collision();//to zero out the path in case of a collision
	bool collide(int s);
public:
	
	Greed::abs_pos getRealAbsolutePosition()//returns the top left corner of the ship sprite
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		return absolutePosition;
	}
	Greed::abs_pos getAbsolutePosition(ShipSide s = ShipSide::FRONT)
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		if (s == ShipSide::FRONT)
			return front_abs_pos;
		else if (s == ShipSide::REAR)
		{
			return rear_abs_pos;
		}


	}
private:
	void initialize_player(int ship_id, string name, Mutex* mutx, deque<shipInfo>& ob, int code[rows][columns], int power, Greed::coords tile_pos)//considering that the default direction is south
	{
		this->ship_id = ship_id;
		this->mutex_id = ship_id;
		//first half of the initializer
		this->name = name;
		this->mutx = mutx;
		shipInfoList = ob;
		map_ob.getMap(code, rows, columns, localMap);

		//here initializing the cannon object of the ship
		cannon_ob.initialize(mutx, power);

		//second half of the initializer
		//setting the positions here only
		updateTilePos(tile_pos, 2369);//updating the position of the user
		updateTileRearPos(tile_pos);


		absolutePosition = Greed::abs_pos(getCurrentTile().c * len, getCurrentTile().r * len);
		//here the default direction is NORTH so rear_abs_pos and front_abs_pos has to be set according to that direction
		front_abs_pos = absolutePosition;
		rear_abs_pos = Greed::abs_pos(absolutePosition.x, absolutePosition.y + len);


	}
	void setBullet_forCannon(Greed::bullet& bull);
	void initializer(Mutex* mutx, deque<shipInfo>& ob, int code[rows][columns], int power)
	{


		this->mutx = mutx;
		shipInfoList = ob;
		map_ob.getMap(code, rows, columns, localMap);

		//here initializing the cannon object of the ship
		cannon_ob.initialize(mutx, power);

	}
	
	static List<Greed::vertex>& getGlobalMapref(int key)//to be used only once in main only
	{
		if (key == 2369)
		{
			// unique_lock<mutex> lk(mutx->m_global_map);
			return globalMap;
		}
	}

	List<Greed::vertex>& getGlobalMap()
	{
		unique_lock<mutex> lk(mutx->m_global_map);
		return globalMap;
	}
	int getPointPath(int key)
	{
		if (key == 2369)
		{

			return pointPath;

		}
	}
	int getPointTilePath(int key)
	{
		if (key == 2369)
		{

			return pointTilePath;

		}
	}
	void update_pointPath(int n, int key)
	{
		if (key == 2369)
		{
			pointPath = n;
		}
	}
	void update_pointTilePath(int n, int key)
	{
		if (key == 2369)
		{
			pointTilePath = n;
		}
	}
	List<Greed::abs_pos>& getPathList(int key)
	{
		if (key == 2369)
		{
			return path;
		}
	}

	void updateTilePos(Greed::coords ob, int key)
	{
		if (key == 2369)
		{
			tile_pos_front = ob;
		}
	}
	void updateTileRearPos(Greed::coords ob)
	{
		tile_pos_rear = ob;
	}
	int getMutexId(int key)
	{
		if (key == 2369)
		{
			return mutex_id;
		}
	}

	void setLocalMap(List<Greed::vertex>& ob, int key)
	{
		if (key == 2369)
			localMap = ob;
	}
	void update_tile_pos(double x, double y);//this function has to be called after updating the pixel coordinates of the player
	//void chaseShip1(int s_id);//funtion to chase a ship of the given ship_id:: autopilot mode will be on here
	bool updateCost(Entity e, double new_cost, Bonus b);
	bool updateCost(Greed::coords ob, double new_cost);//making the function private
public:
	void Greed_updateCost(Greed::coords ob, double new_cost);//new function to update the stuff


private:
	class boundingEntity
	{
	public:
		double x1;
		double x2;
		double y1;
		double y2;

		int c1;
		int c2;
		int r1;
		int r2;

	};
	class shipEntity
	{
	public:
		/*
		* class enclosing the bounding entity of anything

		*/
		Greed::abs_pos c1;//top left corner
		Greed::abs_pos c2;//top right corner
		Greed::abs_pos c3;//bottom right corner
		Greed::abs_pos c4;//bottom left corner
		//order is in clockwise starting from top left corner

	};
	void filter(vector<Greed::abs_pos>& l, ship::boundingEntity& ob);
	boundingEntity getRadiBound(Greed::coords position);//returns the entity bound of the radii 
	shipEntity getShipEntity1()//returns freshly prepared shipEntity //this cant be locked by a mutex
	{
		shipEntity ob;

		ob.c1 = absolutePosition;
		ob.c2 = Greed::abs_pos(absolutePosition.x + len, absolutePosition.y);
		ob.c4 = Greed::abs_pos(absolutePosition.x, absolutePosition.y + len);
		ob.c3 = Greed::abs_pos(absolutePosition.x + len, absolutePosition.y + len);
		return ob;
	}
	shipEntity getShipEntity_without_mutex()
	{
		shipEntity ob;
		//unique_lock<mutex> lk(mutx->m[ship_id]);
		ob.c1 = absolutePosition;
		ob.c2 = Greed::abs_pos(absolutePosition.x + len, absolutePosition.y);
		ob.c4 = Greed::abs_pos(absolutePosition.x, absolutePosition.y + len);
		ob.c3 = Greed::abs_pos(absolutePosition.x + len, absolutePosition.y + len);
		return ob;
	}
	shipEntity getShipEntity()//returns freshly prepared shipEntity //this cant be locked by a mutex
	{
		shipEntity ob;
		unique_lock<mutex> lk(mutx->m[ship_id]);
		ob.c1 = absolutePosition;
		ob.c2 = Greed::abs_pos(absolutePosition.x + len, absolutePosition.y);
		ob.c4 = Greed::abs_pos(absolutePosition.x, absolutePosition.y + len);
		ob.c3 = Greed::abs_pos(absolutePosition.x + len, absolutePosition.y + len);
		return ob;
	}
	bool collide(int s, Greed::coords& pos);//function to check if this->ship_id collided with a ship having ship id s
	bool isShipInMyRadius_forFire(int s_id, cannon myside, ShipSide oppSide);
	void setBullet(Greed::bullet& bull, cannon can, int s_id, ShipSide s);//used to set the location and trajectory of the bullet in the graphics part
	bool isOverlapping(shipEntity, shipEntity, int);

	//making some functions private, these are not to be used by the end user

	bool sail(Direction d, int tiles);

	Greed::path_attribute setTarget(Greed::coords ob);

	Greed::path_attribute setTarget(int s_id);//overloaded function for returning the object of path_attribute
	//Greed::path_attribute setTarget(Greed::abs_pos ob) ;//overloaded function

	bool setPath(List<Greed::coords> ob, int state = 1); //set the path of coords
	bool fireCannon(cannon can, int s_id, ShipSide ship);//target is true for ships and false for cannons
	//defining the ctor
	bool fireAtCannon(int c_id, cannon can);//fire at any cannon: can is the side of the ship's cannon
	bool upgradeHealth(int n);
	bool upgradeAmmo(int n);
	bool upgradeFuel(int n);
public:

	vector<int> cannonsInMyRadius();
	vector<int> shipsInMyRadius();
	vector<int> cannonsIamInRadiusOf();//this function returns the cannon id having ship in radius.
	vector<int> shipsIamInRadiusOf();//returns the list of the ships who are overshadowing the current ship


	vector<Greed::coords> getRadiusCoords_ship(int s_id);//function to return the tiles that are in the radius of a particular entity, just pass the id of the ship
	vector<Greed::coords> getRadiusCoords_cannon(int c_id);//function to return the tiles that are in the radius of the cannon
	ship();
	
	//this class will have the API for  the user, some functions will be inaccessible to the user, inaccessible functions will be protected by an id
	///functions for returning things.
	deque<shipInfo> getShipList()
	{
		return shipInfoList;
	}

	int getShipRadius()
	{
		//unique_lock<mutex> lk(mutx->m[mutex_id]);
		return radius;
	}
	double getCurrentFuel()
	{
		unique_lock<mutex> lk(mutx->m[mutex_id]);
		return fuel;
	}
	Direction getShipDirection()
	{
		unique_lock<mutex> lk(mutx->m[mutex_id]);
		return dir;
	}
	int getShipId()
	{
		return ship_id;
	}
	Greed::coords getCurrentTile()//returns the front tile
	{
		unique_lock<mutex> lk(mutx->m[mutex_id]);
		return tile_pos_front;

	}
	Greed::coords getCurrenntTile_withoutMutex()
	{
		return tile_pos_front;
	}
	Greed::coords getCurrentRearTile_withoutMutex()
	{
		return tile_pos_rear;
	}
	Greed::coords getCurrentRearTile()//returns the rear tile of the ship
	{
		unique_lock<mutex> lk(mutx->m[mutex_id]);
		return tile_pos_rear;

	}

	double getcurrentRadius()
	{
		unique_lock<mutex> lk(mutx->m[mutex_id]);
		return radius;
	}
	double getCurrentHealth()
	{
		unique_lock<std::mutex> lk(mutx->m[mutex_id]);
		return health;
	}

	int getDiedStatus()
	{
		unique_lock<std::mutex> lk(mutx->m[mutex_id]);
		return died;
	}

	int getCurrentAmmo()
	{

		unique_lock<std::mutex> lk(mutx->m[mutex_id]);
		return ammo;
	}

	int getTotalShips()
	{
		return total;
	}
	double getCurrentGold()
	{
		unique_lock<mutex> lk(mutx->m[mutex_id]);
		return gold;
	}
	//functions for updating some values of the game

	//starting of the API
	attribute whatsHere(Greed::coords ob, int m = 0);//leave this for now
	// List<attribute> whatsHere(Map::abs_pos ob);


private:
	//checkCollision is to check collision between a ship and the bullet
	bool checkCollision(int sid, const Greed::bullet& ob);//sid is the ship id of  the victim ship.
	bool anchorShip();//function to halt the ship

	bool chaseShip(int s_id);//driver for calling chaseShip1 in a thread
public:
	// bool updateCost(Greed::abs_pos ob,double new_cost);
	bool Greed_sail(Direction d, int tiles = 1)
	{

		unique_lock<mutex> lk(mutx->m[ship_id]);
		if (solid_motion == 0 && tiles > 0 && fuel > 0 && d != Direction::NA)
		{
			navigation nav(1, Greed::coords(-1, -1), -1, tiles, d);
			nav_data.push_back(nav);
			solid_motion = 1;
			navigation_promise = true;
			return true;
		}
		return false;
	}
	int count_upgrade;//number of times the agent has sent the upgrading request
	bool isCannonInRadius(int c_id, ShipSide side = ShipSide::FRONT);

	bool isInShipRadius(int s_id, Greed::coords ob, ShipSide side = ShipSide::FRONT);//check for a poitn
	bool isInShipRadius(int s_id, ShipSide myside = ShipSide::FRONT, ShipSide opponent_side = ShipSide::FRONT);//overloaded function for isInShipRadius
	//make a function which checks if a tile is in a ships radius or not
	bool isShipInMyRadius(int s_id, ShipSide myside = ShipSide::FRONT);//check for self

	// armory starts from here


	vector<Greed::cannon> getCannonList()
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		vector<Greed::cannon> can;
		for (int i = 0; i < cannon_list.howMany(); i++)
		{
			can.push_back(cannon_list[i]);
		}
		return can;
	}





	int getIndex(List<Greed::coords> ob, Greed::coords val)
	{
		int index = -1;
		for (int i = 0; i < ob.howMany(); i++)
		{
			if (val.c == ob[i].c && val.r == ob[i].r)
			{
				index = i;
				return index;
			}
		}
		return index;

	}
	bool isShipMoving(int s_id)// tells if another player's ship is moving
	{
		ship* ob = shipInfoList[s_id].ob; //assuming that no ship entry is deleted and every ship is in increasing order of their ship_id
		if (ob->motion == 1)
		{
			return true;
		}
		else if (ob->motion == 0)
		{
			return false;
		}
	}
	bool isShipMoving()
	{
		//return isShipMoving(this->ship_id);
		 //current mechanism is to check if the list of absolute coords is >0
		unique_lock<mutex> lk(mutx->m[ship_id]);
		return motion;

	}
	

	double getDistance(int s_id);//returns the distance of s_id ship from the this->ship

	//introducing the new functions

	bool Greed_setPath(int s_id)
	{

		unique_lock<mutex> lk(mutx->m[ship_id]);

		if (solid_motion == 0 && fuel > 0 && s_id != -1 && s_id != ship_id && getShipList()[s_id].getDiedStatus() == 0)
		{
			navigation nav(0, Greed::coords(-1, -1), s_id, -1, Direction::NA);
			nav_data.push_back(nav);
			solid_motion = 1;
			this->navigation_promise = true;
			return true;
		}
		return false;
	}
	bool Greed_setPath(Greed::coords ob)
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);

		if (solid_motion == 0 && fuel > 0 && ob.r != -1 && ob.c != -1 && whatsHere(ob).entity != Entity::CANNON && whatsHere(ob).entity != Entity::LAND)
		{
			navigation nav(0, ob, -1, -1, Direction::NA);
			solid_motion = 1;
			this->navigation_promise = true;
			nav_data.push_back(nav);
			return true;
		}
		return false;

	}
	bool Greed_chaseShip(int s_id)
	{

		unique_lock<mutex> lk(mutx->m[ship_id]);

		if (this->solid_motion == 0 && fuel > 0 && s_id != ship_id && s_id != -1 && autopilot == 0)
		{
			lk.unlock();
			if (!isInShipRadius(s_id))
			{
				lk.lock();
				navigation nav(2, Greed::coords(-1, -1), s_id, -1, Direction::NA);
				nav_data.push_back(nav);
				solid_motion = 1;
				navigation_promise = true;
				lock_chase_ship = 1;
				return true;
			}
		}
		return false;


	}
	bool Greed_anchorShip()
	{
		if (solid_motion == 1)
		{
			unique_lock<mutex> lk(mutx->m[ship_id]);
			navigation nav(3, Greed::coords(-1, -1), -1, -1, Direction::NA);
			nav_data.push_back(nav);
			return true;
		}
		return false;

	}
	//entity conversion functions
	void Greed_fireCannon(cannon can, int s_id, ShipSide s)
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);

		bullet_info.push_back(bullet_data(0, can, s_id, s, -1));

	}
	void Greed_fireAtCannon(int c_id, cannon can = cannon::FRONT)
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);

		bullet_info.push_back(bullet_data(1, can, -1, ShipSide::NA, c_id));
	}

	deque<upgrade_keeping> upgrade_queue;
	void Greed_upgradeHealth(int n)
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		if (lock_health == 0)
		{

			udata.push_back(upgrade_data(1, n));
			lock_health = 1;


		}
	}
	void Greed_upgradeAmmo(int n)
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		if (lock_ammo == 0)
		{

			udata.push_back(upgrade_data(0, n));
			lock_ammo = 1;

		}
	}
	void Greed_upgradeFuel(int n)
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		if (lock_fuel == 0)
		{

			udata.push_back(upgrade_data(2, n));
			lock_fuel = 1;

		}
	}

	friend class Event;
	friend class graphics;
	friend class Control;

	friend class Greed::cannon;

	friend void chaseShip1(int s_id, ship& ob);
	friend bool get(ship* a, ship* b);
	friend class shipInfo;
	friend void update_frame(deque<ship*>& pl1, pack_ship& ob, int i);
	friend class control1;
	friend void startup(int n, unordered_map<int, sockaddr_storage>& socket_id, int port);
	friend int main(int argc, char* argv[]);
};
class id_port_class//to carry the information about the client who is giving the port
{
public:
	int id;
	char username1[20];

	id_port_class()
	{

	}
	id_port_class(int id, string username)
	{
		this->id = id;
		strcpy(username1, username.c_str());
	}
};

class Control//this class will control everything regarding the game
{
public:
	mutex m1;//mutex lock for ship_list
	mutex m2;//mutex lock for cannon_list;
	mutex m3;//mutex for bonus
	mutex m4;//mutex for storm
	static deque<ship*> ship_list;
	static List<Greed::cannon> cannon_list;
	static List<Greed::coords> bonus;//list of the coords having bonus
	static List<Greed::coords> storm;//list of the coords having storm right now
	static List<Greed::coords> opaque_coords;

	void setBonusList(List<Greed::coords>& bonus)
	{
		unique_lock<mutex> lk(m3);
		this->bonus = bonus;
	}
	void setStormList(List<Greed::coords>& storm)
	{
		unique_lock<mutex> lk(m4);
		this->storm = storm;
	}
	List<Greed::coords> getOpaqueCoords()
	{
		return opaque_coords;
	}

	deque<ship*>& getShipList(int key)
	{
		if (key == 2369)
		{
			unique_lock<mutex> lk(m1);
			return ship_list;
		}


	}
	List<Greed::cannon>& getCannonList(int key = 0)
	{
		unique_lock<mutex> lk(m2);
		return cannon_list;



	}
	void setShipList(deque<ship*>& ob, int key)//function to set the ship List
	{
		if (key == 2369)
		{
			unique_lock<mutex> lk(m1);
			ship_list = ob;

		}

	}


	List <Greed::coords>& getBonusList()
	{
		unique_lock<mutex> lk(m3);
		return bonus;
	}
	List<Greed::coords>& getStormList()
	{
		unique_lock<mutex> lk(m4);
		return storm;
	}


public:

	friend class ship;//declaring ship class as the friend such that mutex m1 and m2 can be used by its functions
	friend class graphics;

	friend class attribute;
	friend int main(int argc, char* argv[]);
	
	friend class Greed::cannon;
	friend void startup(int n, unordered_map<int, sockaddr_storage>& socket_id, int port);

};
class control1
{
public:
	void bullet_to_data(Greed::bullet& ob, bullet_data_client& ob1)
	{
		ob1.id = ob.id;
		ob1.power = ob.id;//power of the bullet
		ob1.damage = ob.damage;
		ob1.launch_ship = ob.launch_ship;//if bullet is fired by ship then id of ship is stored else -1
		ob1.launch_cannon = ob.launch_cannon;//if bullet is fired by cannon then id of the cannon is stored else -1
		ob1.target_ship = ob.target_ship;//id of the ship that launched the bullet this is the id of the ship provided by the launch ship but the bullet might not strike at that ship since everything is in motion
		ob1.hit_ship = ob.hit_ship;//id of the ship that really hit the bullet
		ob1.target_cannon = ob.target_cannon;//if the target is cannon then id of cannon is stored else -1
		ob1.hit_cannon = ob.hit_cannon;
		ob1.isActive = ob.isActive;//is the bullet active
		ob1.isSuccess = ob.isSuccess;//if the bullet hit the target
		ob1.can = ob.can;//side of the firing ship
		ob1.s = ob.s;//side of the enemy ship
	}
	void data_to_bullet(Greed::bullet& ob1, bullet_data_client& ob)
	{
		ob1.id = ob.id;
		ob1.power = ob.id;//power of the bullet
		ob1.damage = ob.damage;
		ob1.launch_ship = ob.launch_ship;//if bullet is fired by ship then id of ship is stored else -1
		ob1.launch_cannon = ob.launch_cannon;//if bullet is fired by cannon then id of the cannon is stored else -1
		ob1.target_ship = ob.target_ship;//id of the ship that launched the bullet this is the id of the ship provided by the launch ship but the bullet might not strike at that ship since everything is in motion
		ob1.hit_ship = ob.hit_ship;//id of the ship that really hit the bullet
		ob1.target_cannon = ob.target_cannon;//if the target is cannon then id of cannon is stored else -1
		ob1.hit_cannon = ob.hit_cannon;
		ob1.isActive = ob.isActive;//is the bullet active
		ob1.isSuccess = ob.isSuccess;//if the bullet hit the target
		ob1.can = ob.can;//side of the firing ship
		ob1.s = ob.s;//side of the enemy ship
	}
	void nav_data_processor(deque<ship*>& pl1, Mutex* mutx);
	void packet_to_pl(shipData_exceptMe ob[20], int s, int ship_id, deque<ship*>& pl1)//from network structure to game structure
	{
		for (int i = 0; i < s; i++)
		{
			if (i != ship_id)
			{
				unique_lock<mutex> lk(pl1[i]->mutx->m[i]);
				pl1[i]->ammo = ob[i].ammo;
				pl1[i]->died = ob[i].died;
				pl1[i]->dir = ob[i].dir;

				pl1[i]->isFiring = ob[i].isFiring;
				pl1[i]->score = ob[i].score;
				pl1[i]->health = ob[i].health;
				pl1[i]->gold = ob[i].gold;
				pl1[i]->solid_motion = ob[i].motion;
				pl1[i]->fuel = ob[i].fuel;
				pl1[i]->invisible = ob[i].invisible;
				pl1[i]->tile_pos_front = ob[i].tile_pos_front;
				pl1[i]->tile_pos_rear = ob[i].tile_pos_rear;
				pl1[i]->front_abs_pos = ob[i].front_abs_pos;
				pl1[i]->rear_abs_pos = ob[i].rear_abs_pos;
				pl1[i]->absolutePosition = ob[i].absolutePosition;

				//cout << "\n position of the other ship==>" << ob[i].tile_pos_front.r << " " << ob[i].tile_pos_front.c;


			}
		}

	}
	void pl_to_packet(shipData_exceptMe ob[20], deque<ship*>& pl1)//for the server
	{
		for (int i = 0; i < pl1.size(); i++)
		{
			//unique_lock<mutex> lk(pl1[i]->mutx->m[i]);
			ob[i].ammo = pl1[i]->ammo;
			ob[i].died = pl1[i]->died;
			ob[i].dir = pl1[i]->dir;
			ob[i].ship_id = pl1[i]->ship_id;
			ob[i].isFiring = pl1[i]->isFiring;
			ob[i].score = pl1[i]->score;
			ob[i].health = pl1[i]->health;
			ob[i].gold = pl1[i]->gold;
			ob[i].motion = pl1[i]->solid_motion;
			ob[i].fuel = pl1[i]->fuel;
			ob[i].invisible = pl1[i]->invisible;
			ob[i].tile_pos_front = pl1[i]->tile_pos_front;
			ob[i].tile_pos_rear = pl1[i]->tile_pos_rear;
			ob[i].front_abs_pos = pl1[i]->front_abs_pos;
			ob[i].rear_abs_pos = pl1[i]->rear_abs_pos;
			ob[i].absolutePosition = pl1[i]->absolutePosition;
		}
	}

	void packet_to_me(shipData_forMe& ob, int id, deque<ship*>& pl1)//recved by the client from the server
	{
		unique_lock<mutex> lk(pl1[id]->mutx->m[id]);
		//transfer each data member from ob to pl1
		pl1[id]->cannon_info.clear();
		for (int i = 0; i < ob.size_cannon_data; i++)
		{
			for (int j = 0; j < pl1[id]->cannon_list.howMany(); j++)
			{
				if (ob.cannon_data[i].getCannonId() == pl1[id]->cannon_list[j].cannon_id)
				{
					pl1[id]->cannon_list[j].health = ob.cannon_data[i].getCannonHealth();
				}
			}
		}


		pl1[id]->seconds = ob.seconds;
		pl1[id]->minutes = ob.minutes;

		pl1[id]->killer_ship_id = ob.killer_ship_id;
		pl1[id]->killed_ships.clear();//clearing the previous killed ship so that no duplicate entry is there
		for (int i = 0; i < ob.killed_ships_size; i++)

		{
			pl1[id]->killed_ships.push_back(ob.killed_ships[i]);
		}

		pl1[id]->score = ob.score;

		pl1[id]->server_fire = ob.server_fire;
		pl1[id]->radius = ob.radius;
		pl1[id]->health = ob.health;
		pl1[id]->gold = ob.gold;
		pl1[id]->died = ob.died;
		pl1[id]->ammo = ob.ammo;
		pl1[id]->fuel = ob.fuel;
		pl1[id]->invisible = ob.invisible;

		pl1[id]->tile_pos_front = ob.front_tile;
		pl1[id]->tile_pos_rear = ob.rear_tile;
		if (ob.front_abs_pos.x != pl1[id]->front_abs_pos.x || ob.front_abs_pos.y != pl1[id]->front_abs_pos.y)
		{

			pl1[id]->navigation_promise = false;
		}
		pl1[id]->front_abs_pos = ob.front_abs_pos;
		pl1[id]->rear_abs_pos = ob.rear_abs_pos;
		pl1[id]->dir = ob.dir;
		pl1[id]->autopilot = ob.autopilot;

		if (!pl1[id]->navigation_promise)
		{
			pl1[id]->solid_motion = ob.motion;
		}
		pl1[id]->absolutePosition = ob.absolute_position;

		pl1[id]->collided_ships.clear();
		/*
		for (int i = 0; i < ob.size_collided_ships; i++)
		{
			pl1[id]->collided_ships.push_back(ob.collided_ships[i]);
		}
		for (int i = 0; i < ob.size_hit_bullet; i++)
		{
			Greed::bullet b;
			data_to_bullet(b, ob.hit_bullet[i]);
			pl1[id]->bullet_hit_tempo.push_back(b);
			pl1[id]->hit_bullet_count++;
		}
		*/

		//	cout << "\n my ship position is==>" << ob.front_tile.r << " " << ob.front_tile.c;

		if (ob.collided_size > pl1[id]->collide_count)
		{
			int col_size = ob.size_collided_ships - min(10, ob.collided_size - pl1[id]->collide_count);
			for (int i = col_size; i < ob.size_collided_ships; i++)
			{
				pl1[id]->collided_ships.push_back(ob.collided_ships[i]);
				pl1[id]->collide_count++;
			}
		}
		if (ob.bullet_hit_size > pl1[id]->hit_bullet_count)
		{
			int bullet_size = ob.size_hit_bullet - min(10, ob.bullet_hit_size - pl1[id]->hit_bullet_count);
			if (ob.size_hit_bullet > 0)
			{
				//cout << "\n server bullet hit count is==>" << ob.bullet_hit_size << "  client bullet hit count is=>" << pl1[id]->hit_bullet_count;
				//cout << "\n size of hit bullet==>" << ob.size_hit_bullet;
			}
			for (int i = bullet_size; i < ob.size_hit_bullet; i++)
			{
				Greed::bullet b;
				data_to_bullet(b, ob.hit_bullet[i]);
				pl1[id]->bullet_hit_tempo.push_back(b);
				pl1[id]->hit_bullet_count++;
			}
			//cout << "\nafter updating the client bullet count is==>" << pl1[id]->hit_bullet_count;
		}

		if (ob.size_unlock > 3)
		{
			cout << "\n received more than 3 unlock requests";
		}
		for (int i = 0; i < ob.size_unlock; i++)
		{
			if (ob.unlock[i] == 0)
			{
				cout << "\n lock unlocked came for ammo";
				pl1[id]->lock_ammo = 0;
			}
			else if (ob.unlock[i] == 1)
			{
				cout << "\n lock unlocked came for health";
				pl1[id]->lock_health = 0;

			}
			else if (ob.unlock[i] == 2)
			{
				pl1[id]->lock_fuel = 0;
			}

		}
		//now one by one empty the resend queue based on the detail that we got:
		if (ob.size_bullet_recved > 0)
		{
			//cout << "\n to be deleted recved by the server is==>";
			for (int i = 0; i < ob.size_bullet_recved; i++)
			{
				//cout << ob.bullet_recved[i] << " ";
			}
		}

		for (int i = 0; i < ob.size_bullet_recved; i++)
		{

			int size = resend_bullet.size();
			for (int j = 0; j < resend_bullet.size(); j++)
			{
				if (resend_bullet[j].first.bullet_id == ob.bullet_recved[i])
				{
					//cout << "\n deleted==>" << ob.bullet_recved[i];
					auto it = resend_bullet.begin();
					advance(it, j);
					resend_bullet.erase(it);
					j--;
				}
			}
		}
		//removing the upgrade data from the queue
		for (int i = 0; i < ob.size_upgrade_recved; i++)
		{
			for (int j = 0; j < pl1[id]->upgrade_queue.size(); j++)
			{
				if (ob.upgrade_recved[i] == pl1[id]->upgrade_queue[j].udata.id)
				{
					//cout << "\n deleting from the queue...";
					auto it = pl1[id]->upgrade_queue.begin();
					advance(it, j);
					pl1[id]->upgrade_queue.erase(it);
					j--;
				}
			}
		}
		for (int i = 0; i < ob.size_cost_id; i++)
		{
			for (int j = 0; j < pl1[id]->upgrade_cost.size(); j++)
			{
				auto it = pl1[id]->upgrade_cost.begin();
				advance(it, j);
				if (ob.cost_recved[i] == it->first)
				{
					pl1[id]->upgrade_cost.erase(it);
					j--;
					//cout << "\n deleted with the id==>" << ob.cost_recved[i];
				}
			}
		}
		for (int i = 0; i < ob.size_nav_id; i++)
		{
			for (int j = 0; j < resend_navigation.size(); j++)
			{
				auto it = resend_navigation.begin();
				advance(it, j);
				if (ob.nav_recved[i] == it->id)
				{
					resend_navigation.erase(it);
					j--;
					//	cout << "\n deleted with the id==>" << ob.nav_recved[i];
				}
			}
		}
	}
	void me_to_packet(shipData_forMe& ob, int id, deque<ship*>& pl1)
	{
		unique_lock<mutex> lk(pl1[id]->mutx->mchase[id]);
		ob.autopilot = pl1[id]->autopilot;
		lk.unlock();
		Control control;
		List<Greed::cannon> cannon_list = control.getCannonList();
		ob.size_cannon_data = cannon_list.howMany();
		for (int i = 0; i < ob.size_cannon_data; i++)
		{
			ob.cannon_data[i] = cannon_info(cannon_list[i].getCannonId(), cannon_list[i].getCannonHealth(), cannon_list[i].getCannonTile(), cannon_list[i].isCannonDead());
		}

		ob.server_fire = pl1[id]->server_fire;
		ob.seconds = pl1[id]->seconds;
		ob.minutes = pl1[id]->minutes;
		ob.killer_ship_id = pl1[id]->killer_ship_id;
		if (pl1[id]->killed_ships.size() <= 20)
		{
			ob.killed_ships_size = pl1[id]->killed_ships.size();
		}
		else
		{
			ob.killed_ships_size = 20;
		}
		for (int i = 0; i < ob.killed_ships_size; i++)
		{
			ob.killed_ships[i] = pl1[id]->killed_ships[i];
		}


		ob.score = pl1[id]->score;

		ob.radius = pl1[id]->radius;
		ob.health = pl1[id]->health;
		ob.gold = pl1[id]->gold;
		ob.died = pl1[id]->died;
		ob.ammo = pl1[id]->ammo;
		ob.fuel = pl1[id]->fuel;
		ob.invisible = pl1[id]->invisible;

		ob.front_tile = pl1[id]->tile_pos_front;
		ob.rear_tile = pl1[id]->tile_pos_rear;
		ob.front_abs_pos = pl1[id]->front_abs_pos;
		ob.rear_abs_pos = pl1[id]->rear_abs_pos;
		ob.dir = pl1[id]->dir;
		ob.motion = pl1[id]->solid_motion;
		ob.absolute_position = pl1[id]->absolutePosition;

		ob.size_collided_ships = std::min(10, (int)pl1[id]->collided_ships.size());
		int p = 0;
		for (int i = 0; i < pl1[id]->collided_ships.size(); i++)
		{
			ob.collided_ships[p] = pl1[id]->collided_ships[i];

			p++;
		}
		ob.collided_size = pl1[id]->collide_count;
		//pl1[id]->collided_ships.clear();

		ob.size_hit_bullet = std::min(10, (int)pl1[id]->bullet_hit_tempo.size());
		p = 0;
		for (int i = std::max(0, (int)pl1[id]->bullet_hit_tempo.size() - 10); i < pl1[id]->bullet_hit_tempo.size(); i++)
		{
			bullet_data_client b;
			bullet_to_data(pl1[id]->bullet_hit_tempo[i], b);
			ob.hit_bullet[p] = b;
			p++;
		}
		ob.bullet_hit_size = pl1[id]->hit_bullet_count;
		//pl1[id]->bullet_hit_tempo.clear();

		if (pl1[id]->unlock.size() <= 5)
		{
			ob.size_unlock = pl1[id]->unlock.size();

		}
		else
		{
			ob.size_unlock = 5;
		}
		for (int i = 0; i < ob.size_unlock; i++)
		{
			ob.unlock[i] = pl1[id]->unlock[i];

		}
		ob.size_bullet_recved = std::min(50, (int)pl1[id]->bullet_id_count.size());
		int k = 0;
		for (int i = max(0, (int)pl1[id]->bullet_id_count.size() - 50); i < pl1[id]->bullet_id_count.size(); i++)
		{
			auto it = pl1[id]->bullet_id_count.begin();
			advance(it, i);
			ob.bullet_recved[k] = it->first;
			k++;
		}

		ob.size_upgrade_recved = std::min(10, (int)pl1[id]->upgrade_id_count.size());
		k = 0;
		for (int i = max(0, (int)pl1[id]->upgrade_id_count.size() - 10); i < pl1[id]->upgrade_id_count.size(); i++)
		{
			auto it = pl1[id]->upgrade_id_count.begin();
			advance(it, i);
			ob.upgrade_recved[k] = it->first;
			k++;
		}
		pl1[id]->unlock.clear();

		ob.size_cost_id = std::min(3, (int)pl1[id]->cost_id_count.size());
		k = 0;
		for (int i = max(0, (int)pl1[id]->cost_id_count.size() - 3); i < pl1[id]->cost_id_count.size(); i++)
		{
			auto it = pl1[id]->cost_id_count.begin();
			advance(it, i);
			ob.cost_recved[k] = it->first;
			k++;
		}

		ob.size_nav_id = std::min(6, (int)pl1[id]->nav_id_count.size());
		k = 0;
		for (int i = max(0, (int)pl1[id]->nav_id_count.size() - 6); i < pl1[id]->nav_id_count.size(); i++)
		{
			auto it = pl1[id]->nav_id_count.begin();
			advance(it, i);
			ob.nav_recved[k] = it->first;
			k++;
		}

	}
	// from client to the server
	void mydata_to_server(deque<ship*>& pl1, int ship_id, shipData_forServer& ob, vector<Greed::bullet>& newBullets, Mutex* mutx, int& nav_res_count, int& bullet_res_count, int& no_bullet_resend)
	{
		//transfer data members from pl1 to shipData_forServer
		unique_lock<mutex> lk(mutx->m[ship_id]);
		ob.ship_id = pl1[ship_id]->ship_id;


		ob.threshold_health = pl1[ship_id]->threshold_health;
		ob.threshold_ammo = pl1[ship_id]->threshold_ammo;
		ob.threshold_fuel = pl1[ship_id]->threshold_fuel;



		ob.radius = pl1[ship_id]->radius;
		ob.size_navigation = min(4, (int)pl1[ship_id]->nav_data_final.size());
		for (int i = 0; i < ob.size_navigation; i++)
		{
			if (pl1[ship_id]->nav_data_final[0].id == -1)//its a new command add in the resend queue
			{
				pl1[ship_id]->nav_data_final[0].id = pl1[ship_id]->navigation_count;
				resend_navigation.push_back(pl1[ship_id]->nav_data_final[0]);
				pl1[ship_id]->navigation_count++;
			}
			ob.nav_data[i] = pl1[ship_id]->nav_data_final[0];
			pl1[ship_id]->nav_data_final.pop_front();
		}



		//ob.size_bulletData = pl1[ship_id]->bullet_info.size();
		if (pl1[ship_id]->bullet_info.size() <= 15)
		{

			ob.size_bulletData = pl1[ship_id]->bullet_info.size();
		}
		else
		{
			ob.size_bulletData = 15;
		}
		if (ob.size_bulletData > 0)
		{
			if (resend_bullet.size() == 0)
			{
				bullet_res_count = 1;
			}

		}
		for (int i = 0; i < ob.size_bulletData && i < 15; i++)
		{
			//the id will be pre assigned for the bullet which is being sent for a resend;
			if (pl1[ship_id]->bullet_info[i].bullet_id == -1)//assigning the id of the bullet if the id is not preassigned
			{
				pl1[ship_id]->bullet_info[i].bullet_id = pl1[ship_id]->bullet_count;
				pl1[ship_id]->bullet_count++;
				resend_bullet.push_back(pair<bullet_data, int>(pl1[ship_id]->bullet_info[i], 1));
				pl1[ship_id]->client_fire++;
			}
			ob.b_data[i] = pl1[ship_id]->bullet_info[i];


		}

		ob.client_fire = pl1[ship_id]->client_fire;
		pl1[ship_id]->bullet_info.clear();

		if (pl1[ship_id]->udata.size() <= 10)
		{
			ob.size_upgrade_data = pl1[ship_id]->udata.size();

		}
		else
		{
			ob.size_upgrade_data = 10;
		}

		if (pl1[ship_id]->udata.size() > 3)
		{
			cout << "\n sending more than 3 locks to update the stuff==>" << pl1[ship_id]->udata.size();
		}
		//cout << "\n came in to send the data==>" << pl1[ship_id]->udata.size();
		for (int i = 0; i < ob.size_upgrade_data; i++)
		{
			if (pl1[ship_id]->udata[i].id == -1)
			{
				pl1[ship_id]->udata[i].id = pl1[ship_id]->count_upgrade;
				pl1[ship_id]->count_upgrade++;
				if (pl1[ship_id]->udata[i].type == 0)//for ammo
				{
					pl1[ship_id]->upgrade_queue.push_back(upgrade_keeping(1, 0, pl1[ship_id]->ammo, pl1[ship_id]->ammo + pl1[ship_id]->udata[i].n, pl1[ship_id]->udata[i]));
				}
				if (pl1[ship_id]->udata[i].type == 1)//for ammo
				{
					pl1[ship_id]->upgrade_queue.push_back(upgrade_keeping(1, 1, pl1[ship_id]->health, pl1[ship_id]->health + pl1[ship_id]->udata[i].n, pl1[ship_id]->udata[i]));
				}
				if (pl1[ship_id]->udata[i].type == 2)//for ammo
				{
					pl1[ship_id]->upgrade_queue.push_back(upgrade_keeping(1, 2, pl1[ship_id]->fuel, pl1[ship_id]->fuel + pl1[ship_id]->udata[i].n, pl1[ship_id]->udata[i]));
				}
				//add in the queue
			}
			cout << "\n sending request to upgrade with the id==>" << pl1[ship_id]->udata[i].id << " with the type==>" << pl1[ship_id]->udata[i].type;
			ob.udata[i] = pl1[ship_id]->udata[i];
		}

		pl1[ship_id]->udata.clear();
		if (pl1[ship_id]->upgrade_cost.size() > 0)
		{

			auto it = pl1[ship_id]->upgrade_cost.begin();
			ob.cost_id = it->first;
			ob.size_update_cost = min(80, (int)it->second.size());
			for (int i = 0; i < ob.size_update_cost; i++)
			{
				ob.cdata[i] = it->second[i];
			}
		}
		else if (pl1[ship_id]->upgrade_cost.size() == 0)
		{
			if (pl1[ship_id]->map_cost_data.size() <= 80)
			{
				ob.size_update_cost = pl1[ship_id]->map_cost_data.size();
			}
			else
			{
				ob.size_update_cost = 80;
			}
			if (pl1[ship_id]->map_cost_data.size() > 0)
			{
				pl1[ship_id]->cost_upgrade_count++;
				ob.cost_id = pl1[ship_id]->cost_upgrade_count;

			}
			for (int i = 0; i < ob.size_update_cost; i++)
			{

				ob.cdata[i] = pl1[ship_id]->map_cost_data[0];
				pl1[ship_id]->upgrade_cost[pl1[ship_id]->cost_upgrade_count].push_back(ob.cdata[i]);
				pl1[ship_id]->map_cost_data.pop_front();
			}
			//putting in the queue for resending this stuff
		}





	}

	void server_to_myData(shipData_forServer& ob, deque<ship*>& pl1, int ship_id, Mutex* mutx)
	{

		for (int i = 0; i < ob.size_navigation; i++)
		{
			if (pl1[ship_id]->nav_id_count.find(ob.nav_data[i].id) == pl1[ship_id]->nav_id_count.end())
			{
				pl1[ship_id]->nav_data_temp.push_back(ob.nav_data[i]);
				pl1[ship_id]->nav_id_count[ob.nav_data[i].id] = 1;
			}

		}

		pl1[ship_id]->ship_id = ob.ship_id;
		pl1[ship_id]->threshold_health = ob.threshold_health;
		pl1[ship_id]->threshold_ammo = ob.threshold_ammo;
		pl1[ship_id]->threshold_fuel = ob.threshold_fuel;
		pl1[ship_id]->radius = ob.radius;
		pl1[ship_id]->bullet_info.clear();
		pl1[ship_id]->client_fire = ob.client_fire;
		

		for (int i = 0; i < ob.size_bulletData; i++)//it has to be merged
		{
			//cout << "\n in convertor, fired by=>" << ob.ship_id;

			if (pl1[ship_id]->bullet_id_count.find(ob.b_data[i].bullet_id) == pl1[ship_id]->bullet_id_count.end())
			{
				//cout << "\n bullet fired by==>" << ship_id << " with the id==>" << ob.b_data[i].bullet_id;
				pl1[ship_id]->bullet_id_count[ob.b_data[i].bullet_id] = 1;
				pl1[ship_id]->bullet_info.push_back(ob.b_data[i]);
				pl1[ship_id]->server_fire++;
			}
			//cout << "\n firing";
		}
		int found = 0;
		for (int i = 0; i < ob.size_upgrade_data; i++)
		{
			if (pl1[ship_id]->upgrade_id_count.find(ob.udata[i].id) == pl1[ship_id]->upgrade_id_count.end())
			{
				pl1[ship_id]->udata.push_back(ob.udata[i]);
				if (ob.udata[i].type == 0)
				{
					cout << "\n update came for ammo==>" << ship_id << " witht he id==>" << ob.udata[i].id;
				}
				else if (ob.udata[i].type == 1)
				{
					cout << "\n update came for health==>" << ship_id << " with the id==>" << ob.udata[i].id;
				}

				found = 1;
				pl1[ship_id]->upgrade_id_count[ob.udata[i].id] = 1;
			}
		}
		if (ob.size_update_cost > 0)
		{
			if (pl1[ship_id]->cost_id_count.find(ob.cost_id) == pl1[ship_id]->cost_id_count.end())
			{
				cout << "\n recved request to update the cost by the ship==>" << ship_id << " with the id==>" << ob.cost_id;
				pl1[ship_id]->cost_id_count[ob.cost_id] = 1;
				for (int i = 0; i < ob.size_update_cost; i++)
				{
					pl1[ship_id]->map_cost_data.push_back(ob.cdata[i]);
				}
			}
		}



	}
	friend class graphics;
};

void filter(List<Greed::coords>& ob);

/*networking libs*/
class pack_ship
{
private:
	int ship_id;//id of the ship
	int seconds;//seconds lived
	int minutes;//minutes lived
	int killer_ship_id;//the ship that killed you if that is the case
	int killer_cannon_id;//the cannon that killed you if that is the case
	int killed_ships_size;
	int killed_ships[10];//the ships that you killed
	int score;
	char name[15];
	Greed::abs_pos front_abs_pos;//topmost coordinates of the tip of the ship:: will be updated in update_tile_pos
	Greed::abs_pos rear_abs_pos;//endmost coordinates of the ship ==> will be updated in update_tile_pos
	//Map::abs_pos absolutePosition;//always stores the top left coordinate of the ship tile
	Direction dir;
	//position is defined by the pointing part of the ship..look for it


	int radius;//square radius
	int bullet_radius;//radius of the bullet...initially this value is equal to the radius of the ship
	double health; //to be changed
	double gold;// to be changed
	int died;//1 for dead, 0 for alive //to be changed
	int motion; //1 for moving, 0 for stop
	int fuel;//fuel is used for moving the ship around, once its over the ship cant move
	int invisible;//to be changed
	int ammo; // to be updated
	int bullet_pointer;


	Greed::coords tile_pos_front;
	Greed::coords tile_pos_rear;
	Greed::abs_pos absolutePosition;
	bool isthere;//to tell if the time_line object contains anything

	timeline time_line;
public:
	pack_ship()
	{
		isthere = false;
	}

	friend class graphics;
	friend void update_frame(deque<ship*>&, pack_ship&, int);
};

class cannon_data
{
public:
	int cid;
	double angle;
	int picture;//which picture to display
	double cannon_health;
	int is_cannon_dead;
	cannon_data()
	{

	}
	cannon_data(int c, double a, int pic, double h, int dead)
	{
		cid = c;
		angle = a;
		picture = pic;
		cannon_health = h;
		is_cannon_dead = dead;

	}
};
class animation_data
{
public:
	double x;
	double y;
	int picture;
};


class top_layer
{
public:
	int st;
	double long total_secs;
	double long packet_no;//no. of the packet that is sent, this is used to keep track of the packet sent 
	//for the ships
	int no_of_players;
	pack_ship ob[10];
	//for the bullets
	int no_of_bullets;
	Greed::abs_pos bullet_pos[50];
	//for cannons
	cannon_data cannon_ob[3];//object for cannon
	int no_of_animation;
	animation_data animation_ob[50];//only for explosion 
	int water_image;//which number image has to be shown of the water.
	int gameOver;//0 for no, 1 for yes;
	int end;

	top_layer()
	{
		no_of_bullets = 0;
		no_of_animation = 0;
	}



};
/* networking libs ends */

int getIndex(List<Greed::abs_pos> ob, Greed::abs_pos val);

int aux1(List<Greed::cannon>& cannon, Greed::coords ob);
void filter(List<Greed::coords>& ob, List<Greed::coords> opaque_coords, ship sob);//function for removing the coords which are out of range
void chaseShip1(int s_id, ship& ob);//the famous chasing  a ship function
bool get(ship* a, ship* b);
class graphics
{
public:
	static bool showPatterns;
	Mutex* mutx;
	bool show_radius;//to show the radius of the ships
	graphics()
	{
		show_radius = false;

	}
	static long double total_secs;//total time since epoch of the game this is in seconds
	bool find(List<Greed::cannon> l, Greed::coords ob)
	{
		for (int i = 0; i < l.howMany(); i++)
		{
			if (l[i].tile == ob)
			{
				return true;
			}
		}
		return false;
	}
	enum class ANIMATION_TYPE
	{
		EXPLOSION, CANNON_FIRE, WATER_ANIMATION, NA = -1
	};

	

	bool checkCollision(int sid, const Greed::bullet& ob);
	static long double getTotalTime();
	bool check_game_over(deque<ship*>& pl1);
	deque<ship*> findWinner(deque<ship*>& l);
	void callable(Mutex* mutx, int code[rows][columns], Map& map_ob, int n, unordered_map<int, sockaddr_storage>& socker_id, vector<int>& socket_display);//taking the ship object so as to access the list of the player
	void callable_client(int ship_id, Mutex* mutx, int code[rows][columns], Map& map_ob, int, ship&);
	void callable_server(Mutex* mutx, int code[rows][columns], Map& map_ob, vector<SOCKET>& sockets, unordered_map<SOCKET, int>& socket_id, vector<SOCKET>& socket_display);//taking the ship object so as to access the list of the player

	void callable_clientShow(Mutex* mutx, int code[rows][columns], Map& map_ob);

};

