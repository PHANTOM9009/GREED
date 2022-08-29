

void user3(ship& ob)
{

	ob.chaseShip(1);
	cout << "\n hi this is message";
	ob.threshold_health = 50;
	vector<Greed::coords> l = ob.getRadiusCoords_ship(0);

	while (1)
	{

		Sleep(5);
		//Sleep(500);
	   //if(ob.current_event.size()>0)
	   //cout<<"\n event for ship 0==>"<<(int)ob.getNextEvent().eventType;
	   //ob.fireCannon(cannon::FRONT, 1, ShipSide::FRONT);

		Event e;
		ob.getNextCurrentEvent(e);
		if (e.eventType == Event::EventType::LowHealth)
		{
			//cout << "\n health is==>" << e.lowHealth.getHealth();
			ob.upgradeHealth(20);
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
		if (ob.getShipList()[1].getDiedStatus() == 1)
		{

			ob.ceaseFire();
		}

	}


}