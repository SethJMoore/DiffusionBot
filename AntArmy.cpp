#include "AntArmy.h"

#include <typeinfo>

#include "State.h"
#include "GuardAnt.h"

AntArmy::AntArmy(void)
{
}


AntArmy::~AntArmy(void)
{
}

void AntArmy::updateAnts(State & state, std::vector<Location> newLocations)
{
	std::vector<Ant *>::iterator mIter = myAntArmy.begin();
	while (mIter != myAntArmy.end()) // Go through each ant in my army.
	{
		bool isStillAlive = false;
		std::vector<Location>::iterator vIter = newLocations.begin();
		while (vIter != newLocations.end()) // Check against the newly updated ants to see if still alive.
		{
			if ((*(*mIter)).location == (*vIter))
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
			delete *mIter;
			mIter = myAntArmy.erase(mIter); // Post increment so that erase will happen to the original value before the increment happens.
		}
	}

	std::vector<Location>::iterator vIter = newLocations.begin();
	while (vIter != newLocations.end()) // Add new ants to my army.
	{
		//Ant newAnt((*vIter), state); // Gonna have to make recruitmentCenter function to create appropriate ant type.
		//myAntArmy.push_back(newAnt);
		myAntArmy.push_back(recruitNewAnt((*vIter), state));
		vIter++;
	}
}

void AntArmy::chooseMoves(State & state)
{
	std::vector<Ant *>::iterator mIter = myAntArmy.begin();
	while (mIter != myAntArmy.end())
	{
		(*(*mIter)).chooseMove(state);
		mIter++;
	}
}

Ant * AntArmy::recruitNewAnt(Location loc, State & state)
{
	int numberOfGuards = 0;
	std::vector<Ant *>::iterator mIter = myAntArmy.begin();
	while (mIter != myAntArmy.end())
	{
		if (typeid(*(*mIter)) == typeid(GuardAnt))
		{
			numberOfGuards++;
		}
		mIter++;
	}

	if (numberOfGuards < (myAntArmy.size() / 10) && numberOfGuards < (state.myHills.size() * 4))
	{
		GuardAnt *newAnt = new GuardAnt(loc, state);
		return newAnt;
	}
	else
	{
		Ant *newAnt = new Ant(loc, state);
		return newAnt;
	}
}