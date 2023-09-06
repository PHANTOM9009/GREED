#include<user_lib.hpp>
#include<iostream>
#include<vector>
	int find_ship_to_kill(deque<shipInfo>& shipList, int myid, ship& ob)
	{
		int mini = INT_MAX;
		int index = 0;
		for (int i = 0; i < shipList.size(); i++)
		{
			if (shipList[i].getDiedStatus() == 0 && i != myid)
			{
				int score = 0.6 * shipList[i].getCurrentHealth() + 0.4 * ob.getDistance(i);
				if (score < mini)
				{
					mini = score;//a heuristic to find the best prey
					index = i;
				}
			}
		}
		return index;

	}

	void GreedMain(ship& ob)
	{
		//setting the aim
		deque<shipInfo> shipList = ob.getShipList();
		int index = find_ship_to_kill(shipList, ob.getShipId(), ob);
		if (index >= 0)
			ob.chaseShip(index);
		
		while (1)
		{

			if (ob.frame_rate_limiter())
			{

				//ob.chaseShip(2);
				deque<shipInfo> shipList = ob.getShipList();
				Event e;
				ob.getNextCurrentEvent(e);
				if (index >= 0 && shipList[index].getDiedStatus() == 1)
				{
					index = find_ship_to_kill(shipList, ob.getShipId(), ob);
					ob.chaseShip(index);
				}
				if (e.eventType == Event::EventType::ShipCollision)
				{
					//write collision code here
					index = find_ship_to_kill(shipList, ob.getShipId(), ob);
					ob.chaseShip(index);

				}
				if (e.eventType == Event::EventType::LowHealth)
				{
					ob.upgradeHealth(20);
				}
				deque<Event> q = ob.getPassiveEvent();

				for (int i = 0; i < q.size(); i++)
				{
					if (q[i].eventType == Event::EventType::ShipsInMyRadius)
					{
						for (int j = 0; j < q[i].radiusShip.getShipId().size(); j++)
						{
							ob.fireCannon(cannon::FRONT, q[i].radiusShip.getShipId()[j], ShipSide::FRONT);
						}
					}
					if (q[i].eventType == Event::EventType::CannonsInMyRadius)
					{
						ob.fireAtCannon(q[i].radiusCannon.getCannonId()[0], cannon::FRONT);
					}

				}


				if (ob.getCurrentAmmo() < 5)
				{
					ob.upgradeAmmo(10);
				}
			

			}

		}


	}

