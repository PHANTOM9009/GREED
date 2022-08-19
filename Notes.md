# Notes:

## 1. MAP of the game:

Map is represented as a 2D matrix having dimensions "12 by 24" i.e the Map has 12 rows and 24 columns.
The rows are distributed along the Y-axis and the columns are spread across the X-axis. Starting from
the top left corner of the screen.

The Map is distributed in tiles where every tile is a block. The top left block is internally depicted by 
Greed::coords(0,0) i.e 0th row and 0th column. moving horizontly the column increases and moving vertically
the row increases. The maximum value of the row is 11 and of column is 23. Where the last row i.e 11th row
is made of land hence ship can't be placed there. so the movable coords in the map varies from (0,0) to (10,23)
as depicted in the picture below.

![picture](https://github.com/PHANTOM9009/GREED/blob/master/map_1%20attributes/image2.png)

elements of the Map:

A tile in the Map can be a cannon, Water or land. Cost of moving through a water tile is 1 fuel.
Cost of moving through land is -1, i.e you can't take your ship on or through Land. 
since a cannon is placed on a land tile hence its impenetrable too.

class attribute stores the property of every tile in the map, whenever you want to know the property of a tile
just call the function ship::whatsHere(Greed::coords,int) with the appropriate parameter to know the property of the
tile.

use case:
if you want to find a destination for your ship, you can store the attribute of every tile in the map by iterating
over the 2D matrix and call ship::whatsHere function to know what is there

## 2. Navigation of the ship:

There are functions that dictates the laws of navigation that can be found in documentation of the API.
you can do the following things:
 2.1 navigate ship to a particular destination:

  to do this you first need to select the Greed::coords of the destination. make sure that the coords are valid  
  and are in the range of the map.
 
  then call the function ship::setTarget(Greed::coords) and pass the destination coords. This function will return
	an object of path_attribute which contains two things about the path: 1. fuel that will be spent on taking that path
	2. a List of coords that your ship will take to reach the destination.
	
	you can make decision on the basis of these two parameters whether you want to take that path or not.

NOTE: the path returned is the shortest and the most optimal path which is curated by the API. Mind you, that
the path returned depends solely on your local Map, i.e if you have set the cost of some tiles high then the API
won't choose those tiles in the final path. So make sure that your local Map is up-to-date and according to your
requirements.

If you chose to take that path, then you need to fill your path buffer with the coords that were returned before
this can be done with a simple function call to ship::setPath(List<Greed::coords>) just pass the returned List of 
coords to this function and your ship is all set to reach that destination with the most optimal path possible.

NOTE: if you don't wish to use the API path finding algorithm then you are free to create your own. In that case
you don't need to call the first function i.e setTarget() for creation of List of coords. Rather create your own
List of coords and just call the second function i.e setPath() to set the path buffer to your's curated path.
This game is highly modifyable.

2.2 Sail your ship leisurely:

	you can sail your ship in 8 directions. to do this call the function ship::sail(Direction,int) the first
parameter takes the direction in which you want to sail and the second parameter takes the number of tiles you 
want to sail.

NOTE: This does not guarantee the shortest path but the straight path.

2.3 Chase another ship:

	This mode is just like autopilot mode. If you want to chase another ship then you can call the function
ship::chaseShip(int) and pass the id of the ship that you wish to chase.

How this works?
 Your ship's navigation is automated after the call to this function, i.e you can't control your ship's navigation
your ship will temporarily stop only when it reaches the destination ship's radius. if your ship goes out of its radius
then the ship will start chasing the target ship again.

NOTE: While chasing another ship the path used will again be the shortest path and depends solely on your local Map


The best way to learn more about this function is to try it out!

## 3. Concept of radius of the ship in the Map:

   The area in which the ship can fire is its radius. The concept of Radius is twofold:

   The ship can be at either of the two places:
	1. Lying completely on a tile.In this case the radius of the ship will be 2 tiles as clearly shown in the picture
 
        2. Ship is lying on two tiles.in this case the ship will have a front radius and rear radius, as depicted in the image



    ![image](https://github.com/PHANTOM9009/GREED/blob/master/map_1%20attributes/radius.png)
## 4. Firing:
	Every ship has 2 cannons attached, one at the front and another at the rear. You can fire from any one of them at a time.

To fire at a ship:
	call the function ship::fireCannon(cannon can,int s_id,ShipSide side), cannon can is the front or rear cannon of your ship
int s_id is the target ship's id, ShipSide side is the side of the target ship that you wish to fire at.
Usually side is passed as ShipSide::FRONT when you are above the ship or side is passed as ShipSide::REAR when
you are chasing the ship. The best way to learn this is to try this out!

To fire at a cannon:
	call the function ship::fireAtCannon(int c_id,cannon can), int c_id is the cannon id at which you have to fire
cannon can is the side of your cannon which you want to fire. The difference between REAR and FRONT cannons is of radius in which they can fire
if ship is situated between two tiles then its FRONT cannon and REAR cannon radii are different. So in order to fire your cannon
the target must be in radius which calls for an optimal choice for selecting the side to fire the cannon.
	
To clear the fire buffer:
	to clear the fire buffer or to stop firing at once call the function ship::ceaseFire(). Read "How does firing work?" module to understand better


 How does firing work ?
	
	when you call a function to fire on a ship or a cannon, an entry is pushed in the firing buffer of the user
that entry is taken by the graphics thread, it may be possible that firing doesn't happen at the same instant
when the firing function is called. This happens due to variation of frame rate of user's function and graphics frame rate.

hence if you are firing at a ship or a cannon keep on checking if the target is dead or not, if dead call the function 
ship::ceaseFire() which will clear your firing buffer hence no bullet will be wasted.


## 5. Events:
 	
	An Event API is also provided with the game, which makes programmaing of the algorithms more  trivial.
whenever an event happens it is added in the event queue, which can be checked by the user's algorithm and
can take action on that particular event.

Types of Events:

1. Current Events
2. passive Events

1. Current Events or can be called discrete events:
	Events which take place discretely and happen only once in a while are put in this category.
Current Events includes all the events that are defined in "enum class EventType". They work just like notifications, it alerts the user when an
event has happend, this queue doesn't tell if that event is still happening, this task is performed by passive Events.

How to use current Events?

Current event queue is solely maintained by the user, whereas the API adds events in this queue as they happen, but to remove them from the queue
is the responsibility of the user.

call to ship::getNextEvent(Event e), sets the data of the event that has happend in the variable "Event e", and pops that event from the queue.
the user then can check the variable e for the information of the event. If you didn't understand it here, refer to the examples where it is clearly depicted.

2. Passive Events:
   Events that keep on happening are put in this queue. understand this with an example:
I want to check if a ship is in my radius or not, Current Event queue will notify me for the first time when that ship comes in my radius
but if that ship is still in your radius then that event is a continous event over some time, since this would flood the current event queue 
hence these kinds of events can be checked in passive Events.

Events that are passive include the following(because they are continous):

1. shipsInMyRadius
2. IamInShipRadius
3. IamInCannonRadius
4. CannonsInMyRadius
5. LowHealth
6. NoAmmo
7. LowAmmo
8. NoFuel
9. LowFuel

unlike current events this queue is entirely managed by the API i.e insertion and deletion of the events are done by the API itself.
whenever an Event is outdated i.e it has stopped happening it is automatically removed from the queue.
Events like fire are not included in this queue because they are not continous and are discrete.


usage of this is as follows:
while(1)
{
	Sleep(50);
 deque<Event> q = ob.getPassiveEvent(); //to get the passive event queue
		for (int i = 0; i < q.size(); i++)//iterate over the queue
		{
			if (q[i].eventType == Event::EventType::ShipsInMyRadius)//check for the events
			{
				
				ob.fireCannon(cannon::FRONT, q[i].radiusShip.getShipId()[0], ShipSide::FRONT);
				

			}
			if (q[i].eventType == Event::EventType::CannonsInMyRadius)
			{
				ob.fireAtCannon(q[i].radiusCannon.getCannonId()[0], cannon::FRONT);
			}
			
		}
}
The above code will fire on the ship untill it is in its radius.


ship::getPassiveEvent() doesn't return a reference of the original queue due to race conditions, rather it is a copy of the queue at that instant
So if you are deciding to use this make sure you get a new copy for yourself before checking the events.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------










	


