#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <stdint.h>

#include "Timer.h"
#include "Bug.h"
#include "Square.h"
#include "Location.h"
#include "AntArmy.h"

/*
    constants
*/
const int TDIRECTIONS = 4;
const char CDIRECTIONS[4] = {'N', 'E', 'S', 'W'};
const int DIRECTIONS[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };      //{N, E, S, W}

/*
    struct to store current state information
*/
struct State
{
    /*
        Variables
    */
    int rows, cols,
        turn, turns,
        noPlayers;
    double attackradius, spawnradius, viewradius;
    double loadtime, turntime;
    std::vector<double> scores;
    bool gameover;
    int64_t seed;

	AntArmy myAntArmy;

    std::vector<std::vector<Square> > grid;
    std::vector<Location> myAnts, enemyAnts, myHills, enemyHills, food;

    Timer timer;
    Bug bug;
	//std::ofstream diffusionOut;

    /*
        Functions
    */
    State();
    ~State();

    void setup();
    void reset();

    void makeMove(const Location &loc, int direction);
	bool isSafeMove(Location & fromHere, int direction);

    double distance(const Location &loc1, const Location &loc2);
    Location getLocation(const Location &startLoc, int direction);

    void updateVisionInformation();
	
	void calculateDiffusionMap(std::vector<std::vector<Square> > oldGrid);

	void foodDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid);
	int sumOfFoodStrengths(int oldFoodStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x);

	void neverSeenDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid);
	int sumOfNeverSeenStrengths(int oldNeverSeenStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x);

	void enemyHillDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid);
	int sumOfEnemyHillStrengths(int oldEnemyHillStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x);
	
	void enemyDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid);
	int sumOfEnemyStrengths(int oldFoodStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x);
	
	void unseenDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid);
	int sumOfUnseenStrengths(int oldFoodStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x);
	
	void myAntsDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid);
	int sumOfMyAntsStrengths(int oldFoodStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x);
	
	void myHillsDiffusion(Square thisSquare, int y, int x, std::vector<std::vector<Square> > & oldGrid);
	int sumOfMyHillsStrengths(int oldFoodStrength, std::vector<std::vector<Square> > & oldGrid, int y, int x);

	//void diffusionMapToFile(std::ofstream &os);
};

std::ostream& operator<<(std::ostream &os, const State &state);
std::istream& operator>>(std::istream &is, State &state);


