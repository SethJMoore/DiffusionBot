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
    endTurn();

    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
		state.calculateDiffusionMap();
        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
    state.bug << "turn " << state.turn << ":" << endl;
    state.bug << state << endl;

    //picks out moves for each ant
    for(int ant=0; ant<(int)state.myAnts.size(); ant++)
    {
		int chosenD = 4;
        for(int d=0; d<TDIRECTIONS; d++)
        {
            Location testLoc = state.getLocation(state.myAnts[ant], d);
			Location bestLoc = state.getLocation(state.myAnts[ant], chosenD);
			state.bug << "ant " << ant << ": " << testLoc.row << testLoc.col << state.grid[testLoc.row][testLoc.col].foodStrength << endl;

			if(	(state.grid[testLoc.row][testLoc.col].foodStrength + state.grid[testLoc.row][testLoc.col].neverSeenStrength) >
				(state.grid[bestLoc.row][bestLoc.col].foodStrength + state.grid[bestLoc.row][bestLoc.col].neverSeenStrength))
            {
				chosenD = d;
            }
        }
		if (chosenD != 4)
		{
			state.makeMove(state.myAnts[ant], chosenD);
		}
    }

    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
