
//#include "main_lilb.hpp"
#include<user_lib.hpp>

	int find_ship_to_kill1(deque<shipInfo>& shipList, int myid)
	{
		int mini = INT_MAX;
		int index = 0;
		for (int i = 0; i < shipList.size(); i++)
		{
			if (shipList[i].getDiedStatus() == 0 && i != myid)
			{
				if (shipList[i].getCurrentHealth() < mini)
				{
					mini = shipList[i].getCurrentHealth();
					index = i;
				}
			}
		}
		return index;

	}

	void GreedMain(ship& ob)
	{
		deque<shipInfo> shipList = ob.getShipList();
		int index = find_ship_to_kill1(shipList, ob.getShipId());
		if (index >= 0)
			ob.chaseShip(index);

		while (1)
		{

			if (ob.frame_rate_limiter())
			{

				Event e;
				ob.getNextCurrentEvent(e);
				deque<shipInfo> shipList = ob.getShipList();
				if (index >= 0 && shipList[index].getDiedStatus() == 1)
				{
					index = find_ship_to_kill1(shipList, ob.getShipId());

					ob.chaseShip(index);
				}
				if (e.eventType == Event::EventType::LowHealth)
				{
					//cout << "\n health is==>" << e.lowHealth.getHealth();
					ob.upgradeHealth(20);
				}
				if (e.eventType == Event::EventType::ShipCollision)
				{
					cout << "\n ship 2 has collided";
				}
				deque<Event> q = ob.getPassiveEvent();
				for (int i = 0; i < q.size(); i++)
				{
					if (q[i].eventType == Event::EventType::ShipsInMyRadius)
					{
						//if (frames % 60 == 0)
						//{

						ob.fireCannon(cannon::FRONT, q[i].radiusShip.getShipId()[0], ShipSide::FRONT);
						//}

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
				/*
				if (ob.getShipList()[1].getDiedStatus() == 1)
				{

					ob.ceaseFire();
				}
				*/
			}

		}


	}
























//#include<user_lib.hpp>
/*
void user1(ship& ob)
{
	Greed::path_attribute p=ob.setTarget(Greed::coords(5,5));
	ob.setPath(p.getPath());
	/*
	ob.threshold_health = 50;
	sf::Clock clock;
	sf::Time tt;
	int val = -1;
	int frame = 0;
	int frameRateLimit = 120;
	double elapsed = 0;
	while (1)
	{

		tt = clock.restart();
		elapsed += tt.asSeconds();
		frame = elapsed * frameRateLimit;
		if (frame != val)
		{

			val = frame;
			if (ob.getCurrentHealth() < 50)
			{
			ob.upgradeHealth(5);
			}
			/*
			deque<Event> q = ob.getPassiveEvent();
			for (int i = 0; i < q.size(); i++)
			{
				if (q[i].eventType == Event::EventType::LowHealth)
				{
					ob.upgradeHealth(5);
				}
			}


		}


	}




}

*/