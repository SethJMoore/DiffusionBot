#include "Bot.h"

using namespace std;

//constructor
Bot::Bot()
{

};

//plays a single game of Ants.
void Bot::playGame()
{
    //reads the game parameters and sets up
    cin >> state;
    state.setup();
	state.diffusionOut << state.rows << " " << state.cols << endl;
    endTurn();

    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
		state.myAntArmy.updateAnts(state);
		for (int i = 0; i < 20; i++) // Diffuse the map for a few turns.
		{
			state.calculateDiffusionMap(state.grid);
		}
        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
    state.bug << "turn " << state.turn << ":" << endl;
    state.bug << state << endl;
	state.diffusionMapToFile(state.diffusionOut);

    //picks out moves for each ant
    for(int ant=0; ant<(int)state.myAnts.size(); ant++)
    {
		int chosenD = 4;
		state.bug << "ant " << ant << ": " << "FoodStrength" << state.grid[state.getLocation(state.myAnts[ant], chosenD).row][state.getLocation(state.myAnts[ant], chosenD).row].foodStrength << endl;
        for(int d=0; d<TDIRECTIONS; d++)
        {
            Location testLoc = state.getLocation(state.myAnts[ant], d);
			Location bestLoc = state.getLocation(state.myAnts[ant], chosenD);
			//state.bug	<< "ant " << ant << ": " 
			//			<< "Row" << testLoc.row << " Col" << testLoc.col << " FoodStrength" << state.grid[testLoc.row][testLoc.col].foodStrength << endl;
			if (calculateValue(testLoc) > calculateValue(bestLoc))
            {
				chosenD = d;
            }
        }
		if ((chosenD != 4) && state.isSafeMove(state.myAnts[ant], chosenD))
		{
			state.makeMove(state.myAnts[ant], chosenD);
		}
    }

    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

int Bot::calculateValue(Location & loc)
{
	int value = 0;
	value += state.grid[loc.row][loc.col].foodStrength;
	value += state.grid[loc.row][loc.col].neverSeenStrength;
	value += state.grid[loc.row][loc.col].enemyHillStrength;
	value += state.grid[loc.row][loc.col].unseenStrength;
	//value -= state.grid[loc.row][loc.col].myHillStrength / 4;
	//value -= state.grid[loc.row][loc.col].enemyStrength / 4;
	return value;
};

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
