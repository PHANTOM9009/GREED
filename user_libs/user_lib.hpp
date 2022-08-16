#include<iostream>
#include "List.h"
#include<thread>
#include<mutex>
#include<math.h>
#include<string>
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
int cx(int x);
int cy(int y);

//universal function for finding stuff from the list, pass the list and the element to be found and get the index
template<typename Y>
int getIndex(List<Y>& l, Y val);

class ship;
class graphics;
class Control;

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
	Entity entity;//whats the thing? is it a ship,bonus or etc
	double cost;//in case of a water or storm only
	int ship_id;// if its a ship
	int cannon_id;//if its a cannon
	Bonus bonus;//type of bonus, if a bonus.
	void update(Entity ent, double c, int sid, int cid, Bonus b, int key)
	{
		if (key == 2369)
		{
			entity = ent;
			cost = c;
			ship_id = sid;
			cannon_id = cid;
			bonus = b;
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
		entity = Entity::NA;
		cost = -1;
		ship_id = -1;
		cannon_id = -1;
		bonus = Bonus::NA;
	}
	attribute(Entity ent, double c, int sid, int cid, Bonus b)// ent=NA,c=-1,id=-1,b=NA if any of these doesnt exists
	{
		entity = ent;
		cost = c;
		ship_id = sid;
		cannon_id = cid;
		bonus = b;
	}

	Entity getEntity()
	{
		return entity;
	}
	double getCost()
	{
		return cost;
	}
	int getShipId()
	{
		return ship_id;
	}
	Bonus getBonus()
	{
		return bonus;
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

	mutex bullet_mutex[20];//for the complete list of bullets of the player
	mutex m_global_map;//mutex for syncrhonization of globalMap variable

	mutex m[20];//mutex for the ship
	mutex timeMutex[20];//mutex for protecting the data structure of timeline
	mutex updateTime;//mutex for updating and reading total time of the game
///mutex m is for protecting the path data structure
	mutex event_mutex[20];
	mutex mchase[20];//mutex for follow up function of ship class




public:
	Mutex()
	{

	}
	friend class ship;
	friend class graphics;
	friend class Control;
	friend class Greed::bullet;
	friend class Greed::shipCannon;
	friend void chaseShip1(int, ship&);
	friend class Event;

};
class Greed::bullet
{
	int ttl;
	bool hit_or_not;
	
	Mutex* mutx;//mutex m will be locked
	int mid;//mutex id which is same as ship_id
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
	double slope;
	bool isSuccess;//if the bullet hit the target
	Greed::coords starting_tile;//tile from where the bullet started
	Greed::coords ending_tile;//tile where the bullet will end
	Greed::coords some1;
	int some2;
	::cannon can;//side of the firing ship
	ShipSide s;//side of the enemy ship
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
		isSuccess = -1;
		
		hit_ship = -1;
		target_cannon = -1;
		hit_cannon = -1;
	}

	bool isBulletActive()
	{
		unique_lock<mutex> lk(mutx->m[mid]);
		return isActive;
	}
	bool didBulletHit()
	{
		unique_lock<mutex> lk(mutx->m[mid]);
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
		unique_lock<mutex> lk(mutx->m[mid]);
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
	friend int main();

};
class Greed::cannon//no need to lock this class with any mutex, no concurrency is performed here
{
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
	friend graphics;
	friend int main();
	friend class ship;

};
class Greed::shipCannon
{
	Mutex* mutx;
	int mid;//here it will be equal to the ship id of the ship owning the cannon
	//here id of the cannon will be same as the id of ship 
	double power;
	vector<Greed::bullet> activeBullets;
	vector<Greed::bullet> allBullets;

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

	friend int main();

};
void user2(ship& ob);

class shipInfo
{

	ship* ob;
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
	double getCurrentResources();
	double getCurrentFuel();
	int  getDiedStatus();
	Direction getShipDirection();
	Greed::coords getCurrentTile();
	Greed::coords getCurrentRearTile();
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
};
class Event
{
	static long  count;

	long event_id;
	int sid;//sid represents the id of the ship for which an event is created

public:
	long double timestamp;//time stamp of every event
	long double ttl;
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

bool get(ship* a, ship* b);
class ship//this class will be used to initialize the incoming player and give it a ship. and then keep tracking of that ship
{

protected:
	bool isAuthenticPath(List<Greed::abs_pos> ob);
private:
	Mutex* mutx;
	Event events;// a double sided queue of the events;
	
	static int total;//total ships
	static List<Greed::vertex> globalMap;//global map variable
	int mutex_id;//id of the mutex to be used
	Greed map_ob;//to use the facilities of the class Map
	int ship_id;//id of the ship
	deque<timeline> time_line;
	//armory of the ship
public: //this will be public the user will be able to access this object freely
	//object 

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

	int minutes;//minutes lived
	int killer_ship_id;//the ship that killed you if that is the case
	int killer_cannon_id;//the cannon that killed you if that is the case
	deque<int> killed_ships;//the list of all the ships killed by you..
	int score;
	string name;
	int current_event_point;//pointer to current_event

	List<Greed::bullet> bullet_hit;//the list of the bullets that had ever hit the ship
	deque<Greed::bullet> bullet_hit_tempo;//made solely for the purpose of events, here the objects will be deleted as they are inserted into the events

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


	List<Greed::abs_pos> path;
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
	Greed::abs_pos getAbsolutePosition1(ShipSide s = ShipSide::FRONT)
	{

		if (s == ShipSide::FRONT)
			return front_abs_pos;
		else if (s == ShipSide::REAR)
		{
			return rear_abs_pos;
		}


	}
public:
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
	void initialize_player(string name, Mutex* mutx, deque<shipInfo>& ob, int code[rows][columns], int power, Greed::coords tile_pos)//considering that the default direction is south
	{

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
	bool updateCost(Greed::coords ob, double new_cost);

	bool updateCost(Entity e, double new_cost, Bonus b);

	void update_tile_pos(double x, double y);//this function has to be called after updating the pixel coordinates of the player
	//void chaseShip1(int s_id);//funtion to chase a ship of the given ship_id:: autopilot mode will be on here

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
	bool collide(int s, double x, double y);//function to check if this->ship_id collided with a ship having ship id s
	bool isShipInMyRadius_forFire(int s_id, cannon myside, ShipSide oppSide);
public:
	vector<int> cannonsInMyRadius();
	vector<int> shipsInMyRadius();
	vector<int> cannonsIamInRadiusOf();//this function returns the cannon id having ship in radius.
	vector<int> shipsIamInRadiusOf();//returns the list of the ships who are overshadowing the current ship

	bool fireCannon(cannon can, int s_id, ShipSide ship);//target is true for ships and false for cannons
	//defining the ctor
	bool fireAtCannon(int c_id, cannon can);//fire at any cannon: can is the side of the ship's cannon
	vector<Greed::coords> getRadiusCoords_ship(int s_id);//function to return the tiles that are in the radius of a particular entity, just pass the id of the ship
	ship();
	List<Greed::bullet>& getBulletHitList()
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		return bullet_hit;
	}
	int getTotalBulletsHit()
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		return bullet_hit.howMany();
	}
	/*
	int getTotalBulletsFired()
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		return cannon_ob.allBullets.howMany();
	}
	*/

	Greed::bullet getLastHitBullet()//returns the last bullet that had hit the ship
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		return bullet_hit[bullet_hit.howMany() - 1];
	}
	Greed::bullet getLastBulletFired()//returns the last fired bullet
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		return cannon_ob.activeBullets[cannon_ob.activeBullets.size() - 1];
	}
	List<Greed::bullet> getHitBulletList()//list of the bullets that had hit the ship
	{
		unique_lock<mutex> lk(mutx->m[ship_id]);
		return bullet_hit;
	}
	void ceaseFire()//this function when called deletes all entries in the activeBulletList
	{
		mutx->m[ship_id].lock();
		cannon_ob.activeBullets.clear();
		bullet_pointer = -1;
		mutx->m[ship_id].unlock();
	}
	//this class will have the API for  the user, some functions will be inaccessible to the user, inaccessible functions will be protected by an id
	///functions for returning things.
	deque<shipInfo> getShipList()
	{
		return shipInfoList;
	}

	int getShipRadius()
	{
		unique_lock<mutex> lk(mutx->m[mutex_id]);
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
	double getCurrentResources()
	{
		unique_lock<std::mutex> lk(mutx->m[mutex_id]);
		return resources;
	}
	int getDiedStatus()
	{
		unique_lock<std::mutex> lk(mutx->m[mutex_id]);
		return died;
	}
	int getInvisibleStatus()
	{
		unique_lock<std::mutex> lk(mutx->m[mutex_id]);
		return invisible;
	}
	int getCurrentAmmo()
	{

		unique_lock<std::mutex> lk(mutx->m[mutex_id]);
		return ammo;
	}
	int getCurrentXbow()
	{
		unique_lock<std::mutex> lk(mutx->m[mutex_id]);
		return xbow;
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
	bool setPath(List<Greed::coords> ob, int state = 1); //set the path of coords





	attribute whatsHere(Greed::coords ob, int m = 0);
	// List<attribute> whatsHere(Map::abs_pos ob);

	bool sail(Direction d, int tiles);

	Greed::path_attribute setTarget(Greed::coords ob);

	Greed::path_attribute setTarget(int s_id);//overloaded function for returning the object of path_attribute
	//Greed::path_attribute setTarget(Greed::abs_pos ob) ;//overloaded function

	//checkCollision is to check collision between a ship and the bullet
	bool checkCollision(int sid, const Greed::bullet& ob);//sid is the ship id of  the victim ship.

// bool updateCost(Greed::abs_pos ob,double new_cost);
	bool isCannonInRadius(int c_id, ShipSide side = ShipSide::FRONT);

	bool isInShipRadius(int s_id, Greed::coords ob, ShipSide side = ShipSide::FRONT);//check for a poitn
	bool isInShipRadius(int s_id, ShipSide myside = ShipSide::FRONT, ShipSide opponent_side = ShipSide::FRONT);//overloaded function for isInShipRadius
	//make a function which checks if a tile is in a ships radius or not
	bool isShipInMyRadius(int s_id, ShipSide myside = ShipSide::FRONT);//check for self

		// armory starts from here


	List<Greed::cannon> getCannonList();



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

		if (path.howMany() - 1 == pointPath)
			return false;
		else
			return true;

	}
	bool anchorShip();//function to halt the ship

	bool chaseShip(int s_id);//driver for calling chaseShip1 in a thread

	double getDistance(int s_id);//returns the distance of s_id ship from the this->ship
	void setBullet(Greed::bullet& bull, cannon can, int s_id, ShipSide s);//used to set the location and trajectory of the bullet in the graphics part

	//entity conversion functions

	bool upgradeHealth(int n);
	bool upgradeAmmo(int n);
	bool upgradeFuel(int n);


	friend class Event;
	friend class graphics;
	friend class Control;
	friend int main();


	friend void chaseShip1(int s_id, ship& ob);
	friend bool get(ship* a, ship* b);
	friend class shipInfo;


};


void user1(ship& ob);
void user3(ship& ob);
void user4(ship& ob);
void user5(ship& ob);
void user6(ship& ob);
void user7(ship& ob);
void user8(ship& ob);
void user9(ship& ob);
void user10(ship& ob);
void filter(List<Greed::coords>& ob);
class Control//this class will control everything regarding the game
{
protected:
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
	List<Greed::cannon>& getCannonList(int key)
	{
		if (key == 2369)
		{
			unique_lock<mutex> lk(m2);
			return cannon_list;
		}


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
	friend int main();
	friend void filter();
	friend class Greed::cannon;


};

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
		EXPLOSION, CANNON_FIRE, NA = -1
	};


	double cx(double x)//callibration functions for x and y
	{
		return x + origin_x;
	}
	double cy(double y)
	{
		return y + origin_y;
	}
	bool checkCollision(int sid, const Greed::bullet& ob);
	static long double getTotalTime();
	bool check_game_over(deque<ship*>& pl1);
	void findWinner(deque<ship*>& l);
	void callable(Mutex* mutx, int code[rows][columns], Map& map_ob);//taking the ship object so as to access the list of the player
};
