#include "GuardAnt.h"

#include "State.h"

GuardAnt::GuardAnt(void)
{
}


GuardAnt::~GuardAnt(void)
{
}

void GuardAnt::chooseMove(State & state)
{
	int chosenD = 4;
	state.bug	<< "ant at " << location.row << " " << location.col << ": " << "FoodStrength"
				<< state.grid[state.getLocation(location, chosenD).row][state.getLocation(location, chosenD).row].foodStrength << std::endl;
    for(int d=0; d<TDIRECTIONS; d++)
    {
        Location testLoc = state.getLocation(location, d);
		Location bestLoc = state.getLocation(location, chosenD);
		//state.bug	<< "ant " << ant << ": " 
		//			<< "Row" << testLoc.row << " Col" << testLoc.col << " FoodStrength" << state.grid[testLoc.row][testLoc.col].foodStrength << endl;
		if (calculateValue(testLoc, state) > calculateValue(bestLoc, state))
        {
			chosenD = d;
        }
    }
	if ((chosenD != 4) && state.isSafeMove(location, chosenD))
	{
		state.makeMove(location, chosenD);
		location = state.getLocation(location, chosenD);
	}
	else if (idleTurns > 3)
	{
		chosenD = rand() % 4;
		if (state.isSafeMove(location, chosenD))
		{
			state.makeMove(location, chosenD);
			location = state.getLocation(location, chosenD);
		}
		idleTurns++;
	}
	else
	{
		idleTurns++;
	}
}

int GuardAnt::calculateValue(Location & loc, State & state)
{
	int value = 0;
	value += state.grid[loc.row][loc.col].foodStrength;
	value += state.grid[loc.row][loc.col].enemyHillStrength;
	value += state.grid[loc.row][loc.col].unseenStrength;
	//value -= state.grid[loc.row][loc.col].myHillStrength / 4;
	value += state.grid[loc.row][loc.col].enemyStrength * 2;
	if (state.grid[loc.row][loc.col].myHillStrength < 25)
	{
		value = state.grid[loc.row][loc.col].enemyStrength;
	}
	return value;
};