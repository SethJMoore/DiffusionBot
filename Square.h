#pragma once

#include <vector>

/*
    struct for representing a square in the grid.
*/
struct Square
{
    bool isVisible, isWater, isHill, isFood, hasBeenSeen;
    int ant, hillPlayer;
	int foodStrength, enemyHillStrength, enemyStrength, unseenStrength, neverSeenStrength, myAntsStrength, myHillStrength;
    std::vector<int> deadAnts;

    Square()
    {
        isVisible = isWater = isHill = isFood = hasBeenSeen =  false;
        ant = hillPlayer = -1;
		foodStrength = enemyHillStrength = enemyStrength = unseenStrength = neverSeenStrength = myAntsStrength = myHillStrength = 0;
    };

    //resets the information for the square except water information
    void reset()
    {
        isVisible = 0;
        isHill = 0;
        isFood = 0;
        ant = hillPlayer = -1;
        deadAnts.clear();
    };
};

