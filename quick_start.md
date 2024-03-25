
    
    

# Writing your first algorithm:
 
 ## Template:
 Please look at the template for the cpp program below.
 
```c++

/*1*/ #include<greed_offline.hpp>//includes all the utilites of the game
//include other libraries that user wants
using namespace std;
/*2*/ void GreedMain(ship& ob)//this is the main function that is called to control your ship, ship &ob is the handler object to your battleship
  {
        try
        {
                //write initializing logic
                /*for example:
                * set the threshold values of health,ammo,fuel to trigger the events
                * give your ship a destination
                */
                while (1)//algo loop
                {
                /*3*/	if (ob.frame_rate_limiter())//function to maintain the number of times this loop will run in 1 second. It cannot be changed :)
                        {
                                //write your beautiful code here.....
                        }
                }
        }
        catch(...)
        {
                cout<<"\n exception caught";
        }
}
/*NOTE:
* the user is requested to use exception handling in the given function, if not all exceptions are handled it will terminate the process on
* which the function runs.
*/

```

**Explanation:**

At the first line of the template, we are including a library <user_lib.hpp> this hpp contains the declarations of the required classes necessary to handle your battleship.
So, its important to include this in your cpp file for your program to compile without any errors.
You can include other standard libraries, but you cannot include third party modules. (support for third party modules will come soon)

Next we come to the function **void GreedMain(ship &ob)** , You can create as many supporting functions, classes as you want, but This function must be present in your code,
as the game logic calls this function to control your battleship, consider it as the **void main()** of C++, i.e this should be your main driver function for your algorithm.

Next we see the arguement passed to **void GreedMain(ship &ob)**, ob is the handler object to your battleship, game logic will pass **ob** to your funtion.
 
 **(you can see the documentation of class ship [here](Documentation.md))**
 
 Soon after entering this function you can write initializing logic for your battleship, or give your battleship an initial target, we will see this shortly.
 
 Next we see an infinite while loop, This loop will contain the main logic of your alogrithm, this portion of the code will run repeatedly to control your battleship actions.
 
 There is an if condition to maintain the frame rate of your algo loop, this condition is necessary for your code to function properly.
 
 **You must have noticed that I have used exception handling in this function, you are requested to do the same, because if your code throws an unhandled exception it will terminate your algorithm, and your battleship will be helpless
 and a sitting duck for other battleships :(**
 
 ## Your first algorithm:
 
 Before we dive into the algorithm you must read the [documentation](documentation.md).
 
 #### Before we start, I want you to know what our battleship will do, then we will program it accordingly:
          1. It should select its target battleship based on a simple heuristic function.
          2. It should dodge cannons.
          3. It should upgrade health and ammo, if they are below threshold value
          4. It should attack whoever comes in its radius (in a more better algo you may not want to do this)
              
**Here is the first algorithm:**

```c++

#include<user_lib.hpp>
#include<iostream>
#include<vector>

	int find_ship_to_kill(deque<shipInfo>& shipList,ship& ob)
	{
		/*
		* this function finds a target ship to attack.
		* logic:
		* first compiles shipList to check all ships one by one.
		* using a heuristic function to give a score to all the battleships and finding the ship having a minimum score
		*/

		int mini = INT_MAX;
		int index = 0;
		for (int i = 0; i < shipList.size(); i++)//iterate over shipList which contains information about all the ships in the game
		{
			if (shipList[i].getDiedStatus() == 0 && i != ob.getShipId()) //checking if the ship is dead or not
			{
				int score = 0.6 * shipList[i].getCurrentHealth() + 0.4 * ob.getDistance(i);//calculating score of each ship
				if (score < mini)
				{
					mini = score;//a heuristic to find the best prey
					index = i;
				}
			}
		}
		return index;//returning the id of the ship to attack

	}

	void GreedMain(ship& ob)
	{
		//setting the aim

		/*
		* initializing logic
		*/
		deque<shipInfo> shipList = ob.getShipList(); // to get the information about all the ships
		int index = find_ship_to_kill(shipList, ob);//calling the function to find the target
		if (index >= 0)
			ob.Greed_chaseShip(index); //instructing your ship to chase the target ship

		vector <Greed::cannon> cannonList = ob.getCannonList(); //to get the information about all the cannons in the map
		for (int j = 0; j < cannonList.size(); j++)//iterating over the list of cannons
		{


			vector<Greed::coords> arr = ob.getRadiusCoords_cannon(j);//this function returns the tiles which are in the radius of the cannon in question
			for (int i = 0; i < arr.size(); i++)
			{
				ob.Greed_updateCost(arr[i], 5);//updating costs of all such tiles
			}
		}

		while (1)
		{

			if (ob.frame_rate_limiter())
			{

				//ob.chaseShip(2);
				deque<shipInfo> shipList = ob.getShipList();// get information about the ships 
				Event e;//decare an Event object
				ob.getNextCurrentEvent(e);//get the next active Event
				if (index >= 0 && shipList[index].getDiedStatus() == 1)//check if the current target ship is dead
				{
					/*
					* if the previously targetted ship is destroyed we will look for other ship to target
					*/
					index = find_ship_to_kill(shipList, ob);
					ob.Greed_chaseShip(index);
				}
				if (e.eventType == Event::EventType::ShipCollision)//checking for an active Event
				{
					//write collision code here


				}
				if (e.eventType == Event::EventType::LowHealth)//if health is low LowHealth event is triggered
				{
					ob.upgradeHealth(20);
				}
				deque<Event> q = ob.getPassiveEvent();//to get the passive events queue

				for (int i = 0; i < q.size(); i++)// iterate over passive event queue
				{
					if (q[i].eventType == Event::EventType::ShipsInMyRadius)//event  is triggered if a ship comes in your radius
					{
						for (int j = 0; j < q[i].radiusShip.getShipId().size(); j++)
						{
							/*
							* if a ship comes in your radius fire!
							*/
							ob.Greed_fireCannon(cannon::FRONT, q[i].radiusShip.getShipId()[j], ShipSide::FRONT);
						}
					}
					if (q[i].eventType == Event::EventType::CannonsInMyRadius)
					{
						/*
						* if a cannon comes in your radius Fire!
						*/
						ob.Greed_fireAtCannon(q[i].radiusCannon.getCannonId()[0], cannon::FRONT);
					}

				}


				if (ob.getCurrentAmmo() < 5)//another way to check for resources if you don't want to use Events
				{
					ob.Greed_upgradeAmmo(10);
				}


			}

		}


	}


```
#### You have two ways to compile your codes:
        1. This is the easiest, just open the GREED, upload your algorithm to check for any errors. Errors are logged in a file "log.txt" in the directory which has the game exe.
        2. To compile this code on any editor or IDE you first need to include SFML and TGUI in your project and then compile your code.
          
         
         
    
             
 
 
 
 
 
 
 
 
 


