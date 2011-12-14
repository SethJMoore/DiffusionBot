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
		state.myAntArmy.updateAnts(state, state.myAnts);
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

	state.myAntArmy.chooseMoves(state);

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
