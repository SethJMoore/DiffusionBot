#include "State.h"

using namespace std;

//constructor
State::State()
{
    gameover = 0;
    turn = 0;
    bug.open("debug.txt");
};

//deconstructor
State::~State()
{
    bug.close();
};

//sets the state up
void State::setup()
{
    grid = vector<vector<Square> >(rows, vector<Square>(cols, Square()));
};

//resets all non-water squares to land and clears the bots ant vector
void State::reset()
{
    myAnts.clear();
    enemyAnts.clear();
    myHills.clear();
    enemyHills.clear();
    food.clear();
    for(int row=0; row<rows; row++)
        for(int col=0; col<cols; col++)
            if(!grid[row][col].isWater)
                grid[row][col].reset();
};

//outputs move information to the engine
void State::makeMove(const Location &loc, int direction)
{
    cout << "o " << loc.row << " " << loc.col << " " << CDIRECTIONS[direction] << endl;

    Location nLoc = getLocation(loc, direction);
    grid[nLoc.row][nLoc.col].ant = grid[loc.row][loc.col].ant;
    grid[loc.row][loc.col].ant = -1;
};

bool State::isSafeMove(Location & fromHere, int direction)
{
	bool safe;
	Location testLoc = getLocation(fromHere, direction);
	if (grid[testLoc.row][testLoc.col].ant == 0)
	{
		safe = false;
	}
	else
	{
		safe = true;
	}
	return safe;
}

//returns the euclidean distance between two locations with the edges wrapped
double State::distance(const Location &loc1, const Location &loc2)
{
    int d1 = abs(loc1.row-loc2.row),
        d2 = abs(loc1.col-loc2.col),
        dr = min(d1, rows-d1),
        dc = min(d2, cols-d2);
    return sqrt(float(dr*dr + dc*dc));
};

//returns the new location from moving in a given direction with the edges wrapped
Location State::getLocation(const Location &loc, int direction)
{
	if (direction == 4)
	{
		return loc;
	}
	else
	{
		return Location( (loc.row + DIRECTIONS[direction][0] + rows) % rows,
			             (loc.col + DIRECTIONS[direction][1] + cols) % cols );
	}
};

/*
    This function will update update the lastSeen value for any squares currently
    visible by one of your live ants.

    BE VERY CAREFUL IF YOU ARE GOING TO TRY AND MAKE THIS FUNCTION MORE EFFICIENT,
    THE OBVIOUS WAY OF TRYING TO IMPROVE IT BREAKS USING THE EUCLIDEAN METRIC, FOR
    A CORRECT MORE EFFICIENT IMPLEMENTATION, TAKE A LOOK AT THE GET_VISION FUNCTION
    IN ANTS.PY ON THE CONTESTS GITHUB PAGE.
*/
void State::updateVisionInformation()
{
    std::queue<Location> locQueue;
    Location sLoc, cLoc, nLoc;

    for(int a=0; a<(int) myAnts.size(); a++)
    {
        sLoc = myAnts[a];
        locQueue.push(sLoc);

        std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
        grid[sLoc.row][sLoc.col].isVisible = 1;
		grid[sLoc.row][sLoc.col].hasBeenSeen = true;
        visited[sLoc.row][sLoc.col] = 1;

        while(!locQueue.empty())
        {
            cLoc = locQueue.front();
            locQueue.pop();

            for(int d=0; d<TDIRECTIONS; d++)
            {
                nLoc = getLocation(cLoc, d);

                if(!visited[nLoc.row][nLoc.col] && distance(sLoc, nLoc) <= viewradius)
                {
                    grid[nLoc.row][nLoc.col].isVisible = 1;
					grid[nLoc.row][nLoc.col].hasBeenSeen = true;
                    locQueue.push(nLoc);
                }
                visited[nLoc.row][nLoc.col] = 1;
            }
        }
    }
};

/*
    This is the output function for a state. It will add a char map
    representation of the state to the output stream passed to it.

    For example, you might call "cout << state << endl;"
*/
ostream& operator<<(ostream &os, const State &state)
{
    for(int row=0; row<state.rows; row++)
    {
        for(int col=0; col<state.cols; col++)
        {
            if(state.grid[row][col].isWater)
                os << '%';
            else if(state.grid[row][col].isFood)
                os << '*';
            else if(state.grid[row][col].isHill)
                os << (char)('A' + state.grid[row][col].hillPlayer);
            else if(state.grid[row][col].ant >= 0)
                os << (char)('a' + state.grid[row][col].ant);
            else if(state.grid[row][col].isVisible)
                os << '.';
            else
                os << '?';
        }
        os << endl;
    }

    return os;
};

//input function
istream& operator>>(istream &is, State &state)
{
    int row, col, player;
    string inputType, junk;

    //finds out which turn it is
    while(is >> inputType)
    {
        if(inputType == "end")
        {
            state.gameover = 1;
            break;
        }
        else if(inputType == "turn")
        {
            is >> state.turn;
            break;
        }
        else //unknown line
            getline(is, junk);
    }

    if(state.turn == 0)
    {
        //reads game parameters
        while(is >> inputType)
        {
            if(inputType == "loadtime")
                is >> state.loadtime;
            else if(inputType == "turntime")
                is >> state.turntime;
            else if(inputType == "rows")
                is >> state.rows;
            else if(inputType == "cols")
                is >> state.cols;
            else if(inputType == "turns")
                is >> state.turns;
            else if(inputType == "player_seed")
                is >> state.seed;
            else if(inputType == "viewradius2")
            {
                is >> state.viewradius;
                state.viewradius = sqrt(state.viewradius);
            }
            else if(inputType == "attackradius2")
            {
                is >> state.attackradius;
                state.attackradius = sqrt(state.attackradius);
            }
            else if(inputType == "spawnradius2")
            {
                is >> state.spawnradius;
                state.spawnradius = sqrt(state.spawnradius);
            }
            else if(inputType == "ready") //end of parameter input
            {
                state.timer.start();
                break;
            }
            else    //unknown line
                getline(is, junk);
        }
    }
    else
    {
        //reads information about the current turn
        while(is >> inputType)
        {
            if(inputType == "w") //water square
            {
                is >> row >> col;
                state.grid[row][col].isWater = 1;
				state.grid[row][col].foodStrength = 0;
				state.grid[row][col].enemyHillStrength = 0;
				state.grid[row][col].myHillStrength = 0;
            }
            else if(inputType == "f") //food square
            {
                is >> row >> col;
                state.grid[row][col].isFood = 1;
				state.grid[row][col].foodStrength += 1000;
                state.food.push_back(Location(row, col));
            }
            else if(inputType == "a") //live ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].ant = player;
                if(player == 0)
				{
                    state.myAnts.push_back(Location(row, col));
					state.grid[row][col].foodStrength = 0;
					state.grid[row][col].myAntsStrength += 1000;
				}
                else
				{
                    state.enemyAnts.push_back(Location(row, col));
					state.grid[row][col].enemyStrength += 1000;
				}
            }
            else if(inputType == "d") //dead ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].deadAnts.push_back(player);
            }
            else if(inputType == "h")
            {
                is >> row >> col >> player;
                state.grid[row][col].isHill = 1;
                state.grid[row][col].hillPlayer = player;
                if(player == 0)
				{
                    state.myHills.push_back(Location(row, col));
					state.grid[row][col].myHillStrength += 1000;
				}
                else
				{
                    state.enemyHills.push_back(Location(row, col));
					state.grid[row][col].enemyHillStrength += 1000;
				}

            }
            else if(inputType == "players") //player information
                is >> state.noPlayers;
            else if(inputType == "scores") //score information
            {
                state.scores = vector<double>(state.noPlayers, 0.0);
                for(int p=0; p<state.noPlayers; p++)
                    is >> state.scores[p];
            }
            else if(inputType == "go") //end of turn input
            {
                if(state.gameover)
                    is.setstate(std::ios::failbit);
                else
                    state.timer.start();
                break;
            }
            else //unknown line
                getline(is, junk);
        }
    }

    return is;
};

void State::calculateDiffusionMap(std::vector<std::vector<Square> > oldGrid)
{
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			Square thisSquare = oldGrid[y][x];
			foodDiffusion(thisSquare, y, x, oldGrid);
			neverSeenDiffusion(thisSquare, y, x, oldGrid);
			enemyHillDiffusion(thisSquare, y, x, oldGrid);
			enemyDiffusion(thisSquare, y, x, oldGrid);
			unseenDiffusion(thisSquare, y, x, oldGrid);
			myAntsDiffusion(thisSquare, y, x, oldGrid);
			myHillsDiffusion(thisSquare, y, x, oldGrid);
			//bug << thisSquare.enemyHillStrength % 1000;
		}
		//bug << endl;
	}
};

void State::foodDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid)
{
	if ((!thisSquare.isWater) && (thisSquare.ant != 0))
	{
		int oldFoodStrength = thisSquare.foodStrength;
		grid[y][x].foodStrength = oldFoodStrength + int((.20) * (sumOfFoodStrengths(oldFoodStrength, oldGrid, y, x)));
	}
};

int State::sumOfFoodStrengths(int oldFoodStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x)
{
	int sum = 0;
	if (!oldGrid[(y + rows - 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + rows - 1) % rows][x].foodStrength - oldFoodStrength);
	}
	if (!oldGrid[y][(x + 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + 1) % cols].foodStrength - oldFoodStrength);
	}
	if (!oldGrid[(y + 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + 1) % rows][x].foodStrength - oldFoodStrength);
	}
	if (!oldGrid[y][(x + cols - 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + cols - 1) % cols].foodStrength - oldFoodStrength);
	}
	return sum;
};

void State::neverSeenDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid)
{
	if (!thisSquare.hasBeenSeen)
	{
		grid[y][x].neverSeenStrength = 1000;
	}
	else if ((!thisSquare.isWater) && (thisSquare.ant != 0))
	{
		int oldNeverSeenStrength = thisSquare.neverSeenStrength;
		grid[y][x].neverSeenStrength = oldNeverSeenStrength + int((.20) * (sumOfNeverSeenStrengths(oldNeverSeenStrength, oldGrid, y, x)));
	}
	else
	{
		grid[y][x].neverSeenStrength = 0;
	}
};

int State::sumOfNeverSeenStrengths(int oldNeverSeenStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x)
{
	int sum = 0;
	if (!oldGrid[(y + rows - 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + rows - 1) % rows][x].neverSeenStrength - oldNeverSeenStrength);
	}
	if (!oldGrid[y][(x + 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + 1) % cols].neverSeenStrength - oldNeverSeenStrength);
	}
	if (!oldGrid[(y + 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + 1) % rows][x].neverSeenStrength - oldNeverSeenStrength);
	}
	if (!oldGrid[y][(x + cols - 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + cols - 1) % cols].neverSeenStrength - oldNeverSeenStrength);
	}
	return sum;
};

void State::enemyHillDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid)
{
	if (!thisSquare.isWater)
	{
		int oldEnemyHillStrength = thisSquare.enemyHillStrength;
		grid[y][x].enemyHillStrength = oldEnemyHillStrength + int((.15) * (sumOfEnemyHillStrengths(oldEnemyHillStrength, oldGrid, y, x)));
	}
};

int State::sumOfEnemyHillStrengths(int oldEnemyHillStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x)
{
	int sum = 0;
	if (!oldGrid[(y + rows - 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + rows - 1) % rows][x].enemyHillStrength - oldEnemyHillStrength);
	}
	if (!oldGrid[y][(x + 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + 1) % cols].enemyHillStrength - oldEnemyHillStrength);
	}
	if (!oldGrid[(y + 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + 1) % rows][x].enemyHillStrength - oldEnemyHillStrength);
	}
	if (!oldGrid[y][(x + cols - 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + cols - 1) % cols].enemyHillStrength - oldEnemyHillStrength);
	}
	return sum;
};

void State::enemyDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid)
{
	if (!thisSquare.isWater)
	{
		int oldEnemyStrength = thisSquare.enemyStrength;
		grid[y][x].enemyStrength = oldEnemyStrength + int((.09) * (sumOfEnemyStrengths(oldEnemyStrength, oldGrid, y, x)));
	}
};

int State::sumOfEnemyStrengths(int oldEnemyStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x)
{
	int sum = 0;
	if (!oldGrid[(y + rows - 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + rows - 1) % rows][x].enemyStrength - oldEnemyStrength);
	}
	if (!oldGrid[y][(x + 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + 1) % cols].enemyStrength - oldEnemyStrength);
	}
	if (!oldGrid[(y + 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + 1) % rows][x].enemyStrength - oldEnemyStrength);
	}
	if (!oldGrid[y][(x + cols - 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + cols - 1) % cols].enemyStrength - oldEnemyStrength);
	}
	return sum;
};

void State::unseenDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid)
{
	if ((!thisSquare.isVisible) && (!thisSquare.isWater))
	{
		grid[y][x].unseenStrength = 1000;
	}
	else if ((!thisSquare.isWater) && (thisSquare.ant != 0))
	{
		int oldUnseenStrength = thisSquare.unseenStrength;
		grid[y][x].unseenStrength = oldUnseenStrength + int((.15) * (sumOfUnseenStrengths(oldUnseenStrength, oldGrid, y, x)));
	}
};

int State::sumOfUnseenStrengths(int oldUnseenStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x)
{
	int sum = 0;
	if (!oldGrid[(y + rows - 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + rows - 1) % rows][x].unseenStrength - oldUnseenStrength);
	}
	if (!oldGrid[y][(x + 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + 1) % cols].unseenStrength - oldUnseenStrength);
	}
	if (!oldGrid[(y + 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + 1) % rows][x].unseenStrength - oldUnseenStrength);
	}
	if (!oldGrid[y][(x + cols - 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + cols - 1) % cols].unseenStrength - oldUnseenStrength);
	}
	return sum;
};

void State::myAntsDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid)
{
	if (!thisSquare.isWater)
	{
		int oldMyAntsStrength = thisSquare.myAntsStrength;
		grid[y][x].myAntsStrength = oldMyAntsStrength + int((.15) * (sumOfMyAntsStrengths(oldMyAntsStrength, oldGrid, y, x)));
	}
};

int State::sumOfMyAntsStrengths(int oldMyAntsStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x)
{
	int sum = 0;
	if (!oldGrid[(y + rows - 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + rows - 1) % rows][x].myAntsStrength - oldMyAntsStrength);
	}
	if (!oldGrid[y][(x + 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + 1) % cols].myAntsStrength - oldMyAntsStrength);
	}
	if (!oldGrid[(y + 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + 1) % rows][x].myAntsStrength - oldMyAntsStrength);
	}
	if (!oldGrid[y][(x + cols - 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + cols - 1) % cols].myAntsStrength - oldMyAntsStrength);
	}
	return sum;
};

void State::myHillsDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid)
{
	if (!thisSquare.isWater)
	{
		int oldMyHillsStrength = thisSquare.myHillStrength;
		grid[y][x].myHillStrength = oldMyHillsStrength + int((.04) * (sumOfMyHillsStrengths(oldMyHillsStrength, oldGrid, y, x)));
	}
};

int State::sumOfMyHillsStrengths(int oldMyHillsStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x)
{
	int sum = 0;
	if (!oldGrid[(y + rows - 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + rows - 1) % rows][x].myHillStrength - oldMyHillsStrength);
	}
	if (!oldGrid[y][(x + 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + 1) % cols].myHillStrength - oldMyHillsStrength);
	}
	if (!oldGrid[(y + 1) % rows][x].isWater)
	{
		sum += (oldGrid[(y + 1) % rows][x].myHillStrength - oldMyHillsStrength);
	}
	if (!oldGrid[y][(x + cols - 1) % cols].isWater)
	{
		sum += (oldGrid[y][(x + cols - 1) % cols].myHillStrength - oldMyHillsStrength);
	}
	return sum;
};