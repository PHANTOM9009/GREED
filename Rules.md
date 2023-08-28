# Rules of the game:
 
1. Last standing ship wins and gets +100 in overall score

2. in case there are one or more ships alive and are unable to kill each other  or the timer of the game runs away
	the tie will be broken by the score of the respective ship (both the ships will get +100)

## firing rules:
 
1. +2 score on ever bullet hit on another ship or a cannon

2. health is reduced by the power( reduces by -5 default)  of the bullet hit on a ship or a cannon

3. +2 gold from the gold reserve of the target ship to the firing ship

4. on defeating a ship 30% of gold of defeated ship is transferred to the killer ship

5. on defeated by a cannon a ship looses 30% of its gold

6. all the fuel and ammo of the defeated ship is transferred to the killer ship

7. on defeating a ship the killer ship receives a +100 in score

8. on defeating a cannon the ship receives +100 on score and +1000 in gold

## initial Resources given:

1. money: 1000
2. fuel: 50
3. ammo: 50
4. power of the bullet: 5
5. health: 200

## Conversion rules:
  
   1. Gold to Fuel: call the function "ship::Greed_upgradeFuel(int n)" to increase the fuel in lieu of Gold.
   	1 Fuel is equal to 5 Gold coins.
   2. Gold to Health: call the function "ship::Greed_upgradeHealth(int n)" to increase the Health in lieu of Gold.
    	1 Health is equal to 5 Gold coins.
	
   3. Gold to Ammo: call the function "ship::Greed_upgradeAmmo(int n)" to increase the Ammo in lieu of Gold.
    	1 Ammo is equal to 1 Gold coin.
   	
 
