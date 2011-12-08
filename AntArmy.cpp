#include "AntArmy.h"

#include "State.h"

AntArmy::AntArmy(void)
{
}


AntArmy::~AntArmy(void)
{
}

void AntArmy::updateAnts(State & state, std::vector<Location> newLocations)
{
	std::map<Location, Ant>::iterator mIter = antArmyMap.begin();
	while (mIter != antArmyMap.end())
	{
		bool isStillAlive = false;
		std::vector<Location>::iterator vIter = newLocations.begin();
		while (vIter != newLocations.end())
		{
			if ((*mIter).first == (*vIter))
			{
				isStillAlive = true;
				vIter = newLocations.erase(vIter);
				mIter++;
				break;
			}
			vIter++;
		}
		if (!isStillAlive)
		{
			// Get rid of the ant but advance the iterator to the next position.
			antArmyMap.erase(mIter++); // Post increment so that erase will happen to the original value before the increment happens.
		}
	}

	std::vector<Location>::iterator vIter = newLocations.begin();
	while (vIter != newLocations.end())
	{
		Ant newAnt((*vIter), state); // Gonna have to make recruitmentCenter function to create appropriate ant type.
		antArmyMap[(*vIter)] = newAnt;
		vIter++;
	}
}