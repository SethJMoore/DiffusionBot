#pragma once

#include <map>
#include <vector>

#include "Ant.h"
#include "Location.h"

struct State;

class AntArmy
{
public:
	AntArmy(void);
	~AntArmy(void);

	void updateAnts(State & state, std::vector<Location> newLocations);

	void chooseMoves(State & state);
	std::vector<Ant> myAntArmy;
};

