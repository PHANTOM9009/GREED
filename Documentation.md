# DOCUMENTATION:
## 1. Class ship:
	public members:
	
	1. double threshold_health : threshold value of the health of the ship after which low_health event is triggered
	
	2. double threshold_ammo : threshold value of ammo after which low_ammo event is triggered
	
	3. double threshold_fuel : threshold value of fuel after which low_fuel event is triggered.
	
	public functions:
	1. Greed::abs_pos getAbsolutePosition(ShipSide s = ShipSide::FRONT)
		-> returns the Greed::abs_pos object containing the absolute position of the ship's FRONT or REAR side
			as passed in function argument.
			By default it returns the absolute position in the world of the Ships's FRONT part.

	2. void Greed_updateCost(Greed::coords ob, double new_cost)
		-> updates the cost of the coord "ob" as passed as a parameter to the new_cost in the local map of the player
			These changes are not made in the original map, but in your local map for your navigation.
			the more the cost of a tile, your ship has less probability of taking that path.
   			the navigation path is calculated using your local map because the cost of each tile can be tuned by the player.
      			The global map with default cost is used to deduct your fuel, fuel is deducted according to the global/default costs
	 		for all the players.

	3. vector<int> cannonsInMyRadius()
    	-> returns a std::vector of cannon id's which are in the radius of the player ship.
			This function checks for both the FRONT and REAR radius of the ship and considers the cannon in radius
			if either of the condition is true
	4. vector<int> shipsInMyRadius()
		-> returns a std::vector of ship id's which are in the radius of the player ship.
			This function checks for both the FRONT and REAR radius of the ship and considers the ship in radius
			if either of the condition is true, i.e if the ship is in FRONT or REAR radius.
		
	5. vector<int> cannonsIamInRadiusOf()
		-> return a std::vector of the cannon id's whose radius you are in. It checks both FRONT and REAR part of 
			your ship and adds it if either of your ship's part is in the radius of the cannon.

	6. vector<int> shipsIamInRadiusOf()
		-> return a std::vector of the ship id's whose radius you are in. It checks both FRONT and REAR part of 
			your ship and adds it if either of your ship's part is in the radius of the ship.

	7. bool Greed_fireCannon(cannon can, int s_id, ShipSide ship)
		-> fires your cannon.
			cannon can: fire either FRONT or REAR cannon.
			int s_id: id of the ship you want to fire at.
			ShipSide ship: side of the target ship at which you want to fire.
			
	
    	8. bool Greed_fireAtCannon(int c_id, cannon can)
		-> fires at the cannon.
			int c_id: id of the cannon you want to fire at.
			cannon can: fire either your FRONT or REAR cannon.

	9. vector<Greed::coords> getRadiusCoords_ship(int s_id)
		-> returns the std::vector of coords which are in the radius of the ship whose id is passed as a parameter
			to the function.
			int s_id: id of the ship whose radius coords has to be returned.
	
	
	15. deque<shipInfo> getShipList()
		-> returns a list of ships which are in the game
	
	16. int getShipRadius()
		-> returns the firing radius of the ship

 	17. double getCurrentFuel()
        	-> returns the current fuel that the ship has
  
   	18. Direction getShipDirection()
		-> returns the direction of the ship

	19. int getShipId()
		-> returns your ship id

 	20. Greed::coords getCurrentTile()
		-> returns the current tile your ship is on.
			by default returns the tile on which the front
			part of the ship is on.

	21. Greed::coords getCurrentRearTile()
		-> returns the tile on which ship's rear part is.
 
	22. double getCurrentHealth()
		-> returns the health of the ship

	23. int getDiedStatus()
		-> returns 1 if the ship is defeated else 0

	24. int getCurrentAmmo()
		-> returns the current Ammo 

	25. int getTotalShips()
		-> returns the total ships that are playing in the game
	
	26. double getCurrentGold()
		-> returns the current gold that the ship has

	27. bool Greed_setPath(List<Greed::coords> ob, int state = 1)
		->to set the path that the ship will take.
		  List<Greed::coords> ob=> list of coords returned by setTarget function
		  int state=> is 1 by default.

	28. bool Greed_sail(Direction d, int tiles)
		-> for sailing the ship in direction d, tiles number of times.
			Direction d=> direction in which the ship moves
			tiles=>number of tiles to move in that direction

	
	31. bool isCannonInRadius(int c_id, ShipSide side = ShipSide::FRONT)
		-> returns true if cannon is in your radius
			int c_id=> id of the cannon to check
			ShipSide side=> reference side of the ship from where it will be checked

	32. bool isInShipRadius(int s_id, Greed::coords ob, ShipSide side = ShipSide::FRONT)
		-> returns true if ob is in the radius of the ship having ship id "s_id" from the reference side "side"
	
	33. bool isInShipRadius(int s_id, ShipSide myside = ShipSide::FRONT, ShipSide opponent_side = ShipSide::FRONT)
		-> returns true if: your ship is in the radius of another ship
			int s_id=> id of the ship whose radius you want to check
			ShipSide myside=> side of your ship that you want to check
			ShipSide opponent_side=> side of opponent ship
	
	34. bool isShipInMyRadius(int s_id, ShipSide myside = ShipSide::FRONT)
		-> returns true if: s_id ship is in your radius
			int s_id=> ship you want to check
			ShipSide myside=> your reference side from where you want to check
	
	35. vector<Greed::cannon> getCannonList()
		-> returns the vector of objects of the class Greed::cannon which contains information about the cannons

	36. bool isShipMoving(int s_id)
		-> returns if the ship is moving or not
			int s_id=> id of the ship whose motion you have to check
	
	37. bool isShipMoving()
		->returns true if your ship is moving

	38. bool anchorShip()
		-> stops the ship movement and clears the path buffer
	
	39. bool Greed_chaseShip(int s_id)
		-> function to chase a ship with ship id=> s_id
			Note: once you have called this function the ship is transferred to autopilot mode i.e it will take the best routes according to the cost of the tiles set by you in local map,
			the ship will only stop if it has reached in the radius of target ship, to come out of auto pilot mode call the function anchor ship, this gives you back the navigation controls of the ship
   		You will get out of the autopilot mode in the following scenarios:
     		1. You have collided with any object
       		2. The ship you were chasing was destroyed
	 	3. You call the function anchorShip()

	40. double getDistance(int s_id)
		-> returns the eucledian distance between your ship and the ship passed in the function
		int s_id=> ship from which the distance has to be measured
	
	41. bool Greed_upgradeHealth(int n)
		-> this function upgrades your health by n, by consuming the money required for "n" health
			int n=> addition in health you want
			returns: if it has upgraded health or not
	
	42. bool Greed_upgradeAmmo(int n)
		-> this function upgrades your ammo by n, by consuming the money required for "n" ammo
			int n=> addition in ammo you want
			returns: if it has upgraded ammo or not
			
	43. bool Greed_upgradeFuel(int n)
		-> 	this function upgrades your Fuel by n, by consuming the money required for "n" Fuel
			int n=> addition in Fuel you want
			returns: if it has upgraded Fuel or not

	44. attribute ship::whatsHere(Greed::coords ob, int m)
		-> to check the attributes of the tile passed as "Greed::coords ob".
			Greed::coords ob=> coordinates of the tile about which you want to know
			int m=> m=0 if you want to refer to the global map
					m=1 if you want to refer to the local map
	45. void frame_rate_limiter()
		-> call this function in the `event handling loop of your game inside an if condition` 
		   so that it can control the speed of your loop and run your algorithm smoothly.
	46. shipInfo getShipList()
		-> returns a shipInfo object (class definition could be found below) which contains the information of all the ships playing the game
		
	47. vector<Greed::coords> getRadiusCoords_cannon(int c_id)
		-> returns a vector of Greed::coords which are in the radius of the cannon passed as id
			int c_id : id of the cannon of which you want to know the radius.
		
 

## 2. class Greed::abs_pos
	public data members:
	1. double x: x coordinate
	2. double y: y coordinate

## 3. class Greed::bullet
     public data members:
	1. int id: id of the bullet
	2. double power: power of the bullet
	3. int launch_ship: id of the ship from which the bullet is launched
	4. int launch_cannon: id of the cannon from which bullet was fired
	5. int target_ship: the id of the ship for which the bullet was fired if it wasn't fired for the ship then its -1
	6. int hit_ship: the id of the ship that the bullet hit.
	7. int target_cannon: the id of the cannon at which the bullet was fired
	8. int hit_cannon: the id of the cannon at which the bullet hit
	9. bool isActive: to check if the bullet is still active
	10. bool isSuccess: to check if the bullet hit the desired object
	11. cannon can: side of the firing ship
	12. ShipSide s: side of the enemy ship at which the bullet is fired

	public functions:
		1. bool isBulletActive()
		-> returns if the bullet is active'
		
		2. 	bool didBulletHit()
			-> returns if the bullet was successfull or not
		
		3. int getBulletId()
			-> returns the bullet id 
		
		4. double getBulletPower()
			-> returns the bullet power
	
		5. 	double getBulletDamage()
			-> returns the damage inflicted to the target by the bullet
	
		6. int getLaunchShipId()
			-> returns the launch ship id

		7. int getTargetShipId()
			-> returns the target ship id

## 4. class Greed::cannon
	public functions:
	
	1. int getCannonId()
		->returns the cannon id
	
	2. int getCannonHealth()
		-> returns the health of the cannon

	3. Greed::coords getCannonPosition()
		->returns the tile on which the cannon is kept

	4. double getCannonMoney()
		-> returns the money that the cannon has

	5. bool isCannonDead()
		-> returns if the cannon is dead or not

## 5. class Greed::coords
	public data members:
		int r: denotes the row of the tile
		int c: denotes the column of the tile

## 6. class path_attribute
	public data members:
		1. double resources: the fuel that would be spent if you take this path
		2. List<Greed::coords> target: the list of coords that your ship will take to reach the destination
	
	public data functions:
		1. double getPathCost()
			-> returns the cost of the path
		2. List<Greed::coords> getPath()
			-> returns the path that will be taken by the ship

## 7. class Event
	public data members:
	1. long double timestamp: time in seconds when the event happend
	
	2. Event::EventType eventType : object of enum class EventType

	Radius_ship radiusShip;
	Radius_cannon radiusCannon;
	ShipFire shipFire;
	CannonFire cannonFire;
	LowHealth lowHealth;
	LowAmmo lowAmmo;
	LowFuel lowFuel;
	ShipCollision shipCollision;



	enum class EventType

		public data members:
		
		ShipsInMyRadius: enum of the event when a ship comes in your radius
		
		IamInShipRadius: enum of the event when your ship comes in the radius of another ship
		
		IamInCannonRadius: enum of the event when your ship comes in the radius of the cannon
		
		CannonsInMyRadius: enum of the event when cannons come in your radius
		
		ShipFire: enum of the event when some ship fires at you
			
		CannonFire: enum of the event when some cannon fires at you
		
		LowHealth: enum of the event when your health is low
		
		NoAmmo: enum of the event when your ammo is zero
		
		LowAmmo: enum of the event when your ammo is low
		
		NoFuel: enum of the event when your fuel is zero
		
		LowFuel: enum of the event when your fuel is low
		
		ShipCollision: enum of the event when your ship collides with another ship
		
		NA = -1: no event has taken place

	public classes:
		1. Radius_ship:
			-> this class contains the data corresponding to the event of "ShipsInMyRadius" and "IamInShipRadius"
				

			public member functions:
				1. vector<int> getShipId()
					-> function that returns a std::vector of ship id's which are either in your radius or you are in their radius depending on the specific event
		
		2. Radius_cannon
			->this class contains the data corresponding to the event of "IamInCannonsRadius" and "CannonsInMyRadius"
			
			public member functions:
				1.vector<int> getCannonId()
					->function that returns a std::vector of id's of the cannons which are either in your radius or you are in their radius.
	
		3. ShipFire
			->this class contains the data corresponding to "ShipFire" event.
			
			public member functions:
			1. unordered_map<int,Greed::bullet> getShipId()
				-> function returns a std::unordered_map<int,Greed::bullet> where the first parameter of the map is the id of ship that has fired and second parameter is an object of type Greed::bullet which contains the information about the bullet that was fired.
		
		4. CannonFire
			->this contains the data corresponding to the event of CannonFire
			
			public member functions:
			
			1. unordered_map<int,Greed::bullet> getInfo()
				-> function returns a std::unordered_map<int,Greed::bullet> where the first parameter of the map is the id of cannon that has fired and second parameter is an object of type Greed::bullet which contains the information about the bullet that was fired.
		
		5. LowHealth
			->this class contains the data corresponding to the "LowHealth" event
		
			public member functions:
			
			1. double getHealth():
				->returns the current health of the ship

		6. LowAmmo
		   ->this class contains the data corresponding to the "LowAmmo" event

		
			public member functions:
			
			1. double getAmmo():
				->returns the current Ammo of the ship

		7. LowFuel:

		   ->this class contains the data corresponding to the "LowFuel" event
		
			public member functions:
			
			1. double getFuel():
				->returns the current health of the ship


		8. shipCollision:
			
			-> this class contains information about "ShipCollision" event
			
			public member functions:
			1. vector<int> getShipId()
				-> returns a std::vector of the id's of the ships with which your ship collided

## 8. class attribute:
	
	public member functions:

	1. double getCost()
		->returns cost of the tile set by you in local Map, or set globally in global map

	2. int getShipId()
		->returns the ship id of the ship there
	
	3. int getCannonId()
		->returns the cannon_id of the cannon at that tile.

## 9. enum class map:
	members:
	globalMap
	localMap

## 10. enum class Direction:
	NORTH, SOUTH, EAST, WEST, NORTH_WEST, NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NA = -1

## 11. enum class cannon:
 	FRONT, REAR, NA = -1

## 12. enum class ShipSide:
	FRONT, REAR, MIDDLE, NA = -1
## 13. class shipInfo:
	  public data function:
	  	1. int getTotalShips()
			-> returns the total number of ships playing in the game
		2. int getShipId()
			-> returns the id of the ship 
		3. int getShipRadius()
			-> returns the radius of the ship asked
		4. double getCurrentHealth()
			-> returns the health of the ship
		5. double getCurrentGold()
			-> returns the gold of the ship
		6. double getCurrentFuel()
			-> returns the  fuel of the ship
		7. int getDiedStatus()
			-> returs 0 if ship is alive else returs 1
		8. Direction getShipDirection()
			-> returns the direction in which the ship is going
		9. Greed::coords getCurrentTile()
			-> retuns the front tile of the ship
		10. Greed::coords getCurrentRearTile()
			-> returns the rear tile of the ship
		11. Greed::abs_pos getRealAbsolutePosition()
			-> returns the Greed::abs_pos of the top left corner of the ship

	 


	
	
