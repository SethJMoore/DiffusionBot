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
	std::vector<Ant>::iterator mIter = myAntArmy.begin();
	while (mIter != myAntArmy.end()) // Go through each ant in my army.
	{
		bool isStillAlive = false;
		std::vector<Location>::iterator vIter = newLocations.begin();
		while (vIter != newLocations.end()) // Check against the newly updated ants to see if still alive.
		{
			if ((*mIter).location == (*vIter))
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
			// Get rid of the ant if it's not found, but advance the iterator to the next position.
			mIter = myAntArmy.erase(mIter); // Post increment so that erase will happen to the original value before the increment happens.
		}
	}

	std::vector<Location>::iterator vIter = newLocations.begin();
	while (vIter != newLocations.end()) // Add new ants to my army.
	{
		Ant newAnt((*vIter), state); // Gonna have to make recruitmentCenter function to create appropriate ant type.
		myAntArmy.push_back(newAnt);
		vIter++;
	}
}

void AntArmy::chooseMoves(State & state)
{
	std::vector<Ant>::iterator mIter = myAntArmy.begin();
	while (mIter != myAntArmy.end())
	{
		(*mIter).chooseMove(state);
		mIter++;
	}
}