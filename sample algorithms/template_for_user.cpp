#include<user_lib.hpp>//includes all the utilites of the game
//include other libraries that user wants
using namespace std;
void agent(ship& ob)//this function takes a handler(ob) to the ship so that the user can control the player
{
	//write initializing logic
	/*for example:
	* set the threshold values of health,ammo,fuel to trigger the events
	* give your ship a destination
	*/
	while (1)//algo loop
	{
		if (ob.frame_rate_limiter())//function to maintain the number of times this loop will run in 1 second. It cannot be changed :)
		{
			//write your beautiful code here.....
		}
	}
}
/*NOTE:
* the user is requested to use exception handling in the given function, if not all exceptions are handled it will terminate your thread on
* which the function  runs.
*/